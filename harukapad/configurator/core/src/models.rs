include!(concat!(env!("OUT_DIR"), "/protocol.rs"));

pub mod keypad;

pub use keypad::*;
