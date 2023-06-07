// prerequisites: Install TurboPFor library libic under /usr/lib or /usr/local/lib  
#![allow(non_upper_case_globals)]
#![allow(non_camel_case_types)]
#![allow(non_snake_case)]
//include!(concat!(env!("OUT_DIR"), "/bindings.rs"));
include!("bindings.rs");

#[cfg(test)]
mod tests;
