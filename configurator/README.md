# More More Keypad! Configurator

This folder contains the configurator application for the [More More Keypad!](https://github.com/Ace4896/more-more-keypad), a 3-key analog keypad for osu! themed after [MORE MORE JUMP!](https://www.sekaipedia.org/wiki/MORE_MORE_JUMP!) from Project Sekai.

## Setting Up Development Environment

The configurator project is setup using Rust. Install the following:

- [Stable Rust](https://www.rust-lang.org/learn/get-started) (tested on 1.74.0)
- On Linux, a few other dependencies are needed:
  - [egui Dependencies](https://github.com/emilk/eframe_template/#testing-locally)
  - [serialport-rs Dependencies](https://github.com/serialport/serialport-rs#dependencies)

Then use one of the following commands to run the project:

```bash
# GUI
cargo run               # Run GUI in debug mode
cargo run --release     # Run GUI in release mode

# Interactive CLI
cargo run -- --cli      # Run interactive CLI
```
