// Test file based on https://github.com/paulll/rust-turbopfor
extern crate rand;

use std::convert::TryFrom;
use std::convert::TryInto;
use std::vec;

use super::*;

fn decode_block(file_data: &[u8]) -> Vec<u32> {
    println!("dec size = {}", file_data.len());

    if file_data.len() == 0 {
        return vec![];
    }

    if file_data.len() == 2*4 {
        let s = u32::from_le_bytes(file_data[0..4].try_into().unwrap());
        if s == 1 {
            return vec![u32::from_le_bytes(file_data[4..8].try_into().unwrap())];
        }
    }

    let amount = u32::from_le_bytes(file_data[0..4].try_into().unwrap()) ;
    let mut decoded_data = vec![0u32; (amount + 32).try_into().unwrap()];

    unsafe {
        let _ = p4nd1dec128v32(file_data.as_ptr().add(4) as *mut ::std::os::raw::c_uchar, amount.try_into().unwrap(), decoded_data.as_mut_ptr() as *mut u32);
        decoded_data.resize(amount.try_into().unwrap(), 0);
    }

    return decoded_data;
}

fn encode_block(data: &[u32]) -> Vec<u8> {
    println!("enc size = {}", data.len());

    if data.len() == 0 {
        return vec![];
    }

    if data.len() == 1 {
        let mut result = vec![0u8; 8];
        let (s, d) = result.split_at_mut(4);
        s.copy_from_slice(&1u32.to_le_bytes());
        d.copy_from_slice(&data[0].to_le_bytes());
        return result;
    }

    let mut encoded_data = vec![0u8+4; data.len() * 4];
    let amount = data.len() as u32;
    let (ed_left, _) = encoded_data.split_at_mut(4);
    ed_left.copy_from_slice(&amount.to_le_bytes());

    unsafe {
        let size = p4nd1enc128v32(data.as_ptr() as *mut u32, data.len(), encoded_data.as_mut_ptr().add(4) as *mut ::std::os::raw::c_uchar);
        encoded_data.resize(size+4, 0);
    }

    return encoded_data;
}

fn test_block(size_cap: usize) {
    let mut source = Vec::with_capacity(size_cap);
    for i in 0..size_cap {
        if rand::random() {
            source.push(u32::try_from(i+1).unwrap());
        }
    }

    if source.is_empty() {
        source = vec![1u32, 2]
    }

    let encoded = encode_block(&source[..]);
    let decoded = decode_block(&encoded[..]);
    assert_eq!(&source[..], &decoded[..]);
}

#[test]
fn sample_p4nd1_128v32() {
    let block_sizes = vec![1u32, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 4096, 16384, 65536, 262144, 1048576];

    for _ in 1..10 {
        for size in &block_sizes[..] {
            println!("testing with cap = {}", size+0);
            test_block((size+0).try_into().unwrap());
        }
    }
}
