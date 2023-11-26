use thiserror::Error;

/// Type alias for [`std::result::Result`] which uses [`Error`] as the error type.
pub type Result<T> = std::result::Result<T, Error>;

/// Represents an error that can occur while interacting with the keypad.
#[derive(Error, Debug)]
pub enum Error {
    #[error("I/O error occurred: {}", .0)]
    IO(#[from] std::io::Error),

    #[error("Serial communication error occurred: {}", .0)]
    Serial(#[from] serialport::Error),
}
