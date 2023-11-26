use std::fmt::Display;

use serialport::SerialPortType;

use crate::error::Result;

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

        Ok(available_keypads)
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
