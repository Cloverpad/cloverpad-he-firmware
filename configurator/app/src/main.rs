#![cfg_attr(not(debug_assertions), windows_subsystem = "windows")]

use egui::{IconData, ViewportBuilder};
use more_more_keypad_core::models::{self, keypad::Keypad, Command, Echo, FirmwareVersion};

mod app;

fn main() -> anyhow::Result<()> {
    env_logger::init();

    let available_keypads = Keypad::find_available_keypads()?;
    if let Some(keypad) = available_keypads.first() {
        log::info!(
            "Found {} keypad with serial port {}",
            keypad.variant,
            &keypad.com_port
        );

        let echo_command = Command {
            r#type: Some(models::command::Type::Echo(Echo {
                value: "Hello world!".to_string(),
            })),
        };

        keypad.send_command(&echo_command)?;

        // let firmware_command = Command {
        //     r#type: Some(models::command::Type::FirmwareVersion(FirmwareVersion {}))
        // };

        // keypad.send_command(&firmware_command)?;
    } else {
        log::info!("No keypads found");
    }

    Ok(())

    // eframe::run_native(
    //     "com.ace4896.moremorekeypad.configurator",
    //     eframe::NativeOptions {
    //         viewport: ViewportBuilder::default()
    //             .with_title("More More Keypad! Configurator")
    //             .with_icon(get_window_icon()),
    //         ..Default::default()
    //     },
    //     Box::new(|_| Box::new(app::MainApp::default())),
    // )
}

/// Gets the window icon as [`egui::IconData`].
fn get_window_icon() -> IconData {
    const ICON_DATA_BYTES: &'static [u8] = include_bytes!("../assets/mmj-icon-256.png");
    let image = image::load_from_memory(ICON_DATA_BYTES)
        .expect("Unable to load window icon")
        .into_rgba8();

    let (width, height) = image.dimensions();
    let rgba = image.into_raw();

    IconData {
        rgba,
        width,
        height,
    }
}
