#![cfg_attr(not(debug_assertions), windows_subsystem = "windows")]

use egui::{IconData, ViewportBuilder};

mod app;

fn main() -> eframe::Result<()> {
    env_logger::init();

    eframe::run_native(
        "com.ace4896.moremorekeypad.configurator",
        eframe::NativeOptions {
            viewport: ViewportBuilder::default()
                .with_title("More More Keypad! Configurator")
                .with_icon(get_window_icon()),
            ..Default::default()
        },
        Box::new(|_| Box::new(app::MainApp::default())),
    )
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
