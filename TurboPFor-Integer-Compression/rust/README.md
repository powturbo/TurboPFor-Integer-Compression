# Rust bindings for TurboPFor-Integer-Compression

This is a wrapper for [TurboPFor Integer Compression](https://github.com/powturbo/TurboPFor-Integer-Compression).

## Installation
1- Prerequistes: build and install the TuroPFor library libic.a into /usr/local/lib  

2 - Use the provided src/bindings.rs file or generate a new rust bindings.rs file.
```shell
cargo build
```
3 - Test 
```shell
cargo test
```
## Examples
 - see the tests.rs file in the src folder
 - a list of public functions are available in the bindings.rs file
 
## Reference
- [bindgen automatically generates Rust FFI bindings to C and C++ libraries.](https://rust-lang.github.io/rust-bindgen/)
