use std::{fmt::Display, io::Read, time::Duration};

use prost::Message;
use serialport::SerialPortType;

use crate::{
    error::Result,
    models::{Command, Response},
};

/// Represents a keypad that can be interacted with on the host.
#[derive(Clone, Debug)]
pub struct Keypad {
    pub variant: KeypadVariant,
    pub com_port: String,
}

/// Represents the different keypad variants.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub enum KeypadVariant {
    /// Harukapad Revision 1
    HarukapadR1,
}

impl Keypad {
    /// The baud rate (in bps) to use when communicating with a keypad's serial interface.
    pub const BAUD_RATE: u32 = 115200;

    /// Finds the list of available keypads on the host.
    pub fn find_available_keypads() -> Result<Vec<Keypad>> {
        log::debug!("Querying available keypads...");

        let available_keypads = serialport::available_ports()?
            .into_iter()
            .filter_map(|port_info| match &port_info.port_type {
                SerialPortType::UsbPort(usb_info) => {
                    KeypadVariant::from_vid_pid(usb_info.vid, usb_info.pid).map(|variant| Keypad {
                        variant,
                        com_port: port_info.port_name,
                    })
                }
                _ => None,
            })
            .collect::<Vec<_>>();

        log::debug!("Found {} keypad(s)", available_keypads.len());
        Ok(available_keypads)
    }

    pub fn send_command(&self, command: &Command) -> Result<Response> {
        let command_bytes = command.encode_to_vec();

        // Open a connection to this keypad's serial port
        // Some additional flags are needed for the communication to not stall:
        // - DTR (Data Terminal Ready)
        // - RTS (Request to Send)
        log::debug!("Connecting to serial port {}...", &self.com_port);

        let mut serial_port = serialport::new(&self.com_port, Self::BAUD_RATE)
            .data_bits(serialport::DataBits::Eight)
            .parity(serialport::Parity::None)
            .stop_bits(serialport::StopBits::One)
            .timeout(Duration::from_secs(3))
            .open()?;
        serial_port.write_data_terminal_ready(true)?;
        serial_port.write_request_to_send(true)?;

        log::debug!("Connected to serial port {}", &self.com_port);

        // Send the command to the keypad
        log::debug!(
            "Sending command {:?} ({} bytes)...",
            &command,
            command_bytes.len()
        );

        serial_port.write_all(&command_bytes)?;

        log::debug!("Sent command {:?}", &command);

        // Attempt to read and parse the response
        // First, read how many bytes are in the response (unsigned 32-bit integer in little endian)
        // Then, read the response bytes
        log::debug!("Reading response from keypad...");

        let mut response_length_bytes = [0; 4];
        serial_port.read_exact(&mut response_length_bytes)?;
        let response_length = u32::from_le_bytes(response_length_bytes);
        log::debug!("Response Length: {}", response_length);

        let mut response_bytes = vec![0; response_length as usize];
        serial_port.read_exact(response_bytes.as_mut_slice())?;
        let response = Response::decode(response_bytes.as_slice())?;
        log::info!("Response: {:?}", response);

        Ok(response)
    }
}

impl Display for KeypadVariant {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        use KeypadVariant::*;

        match *self {
            HarukapadR1 => write!(f, "Harukapad Rev. 1"),
        }
    }
}

impl KeypadVariant {
    /// The list of all available keypad variants.
    pub const ALL: [KeypadVariant; 1] = [KeypadVariant::HarukapadR1];

    /// Gets the USb Vendor ID / Product ID combination for this [`KeypadVariant`].
    pub const fn get_vid_pid(&self) -> (u16, u16) {
        use KeypadVariant::*;

        // TODO: Arduino-Pico seems to hardcode PID to 0x9005?
        // Probably needs a custom firmware
        match self {
            HarukapadR1 => (0x1005, 0x9005),
        }
    }

    /// Attempts to convert the USB Vendor ID / Product ID combination into a [`KeypadVariant`].
    pub fn from_vid_pid(vid: u16, pid: u16) -> Option<KeypadVariant> {
        for variant in Self::ALL {
            if variant.get_vid_pid() == (vid, pid) {
                return Some(variant);
            }
        }

        None
    }
}
