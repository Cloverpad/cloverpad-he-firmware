use thiserror::Error;

/// Represents an error that can occur while interacting with the keypad.
#[derive(Error, Debug)]
pub enum Error {
    #[error("I/O error occurred: {}", .0)]
    IO(#[from] std::io::Error),
}
