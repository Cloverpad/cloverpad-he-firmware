# More More Keypad! Software

This repository contains the software for the [More More Keypad!](https://github.com/Ace4896/more-more-keypad), a 3-key analog keypad for osu! themed after [MORE MORE JUMP!](https://www.sekaipedia.org/wiki/MORE_MORE_JUMP!) from Project Sekai.

The repository layout is as follows:

- [`configurator`](./configurator): Configurator application for the keypad.
- [`firmware`](./firmware): Arduino-based firmware for the keypad.
- [`protocol`](./protocol): Serial protocol definitions used by the firmware and configurator.

## Setting Up Development Environment

Both the configurator and firmware rely on [Protocol Buffers](https://protobuf.dev/)' code generator to compile correctly.

- Windows (via [`scoop`](https://github.com/ScoopInstaller/Scoop)): `scoop install protobuf`
- Fedora: `dnf install protobuf-compiler`
- Manual:
  - Download the latest release of Protocol Buffers from [GitHub](https://github.com/protocolbuffers/protobuf/releases) - pick the asset that matches your OS
  - Unzip the contents to a folder, e.g. `~/.protobuf`
  - Add a `PROTOC` environment variable that points to the `protoc` binary, e.g. `PROTOC=~/.protobuf/bin/protoc`

Once this is done, follow the READMEs for the [firmware](./firmware/) and [configurator](./configurator/) to setup each project.
