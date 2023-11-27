# More More Keypad! Serial Communication Protocol

This folder contains the [Protocol Buffers](https://protobuf.dev/) definitions for the serial communication protocol. These are written using [`proto3` syntax](https://protobuf.dev/programming-guides/proto3/).

**NOTE**: For `string` and arrays to work reliably, they need to be constrained in size. This is done by adding a `.options` file next to the `.proto` file with the same name. More details can be found [here](https://jpa.kapsi.fi/nanopb/docs/reference.html#defining-the-options-in-a-.options-file).
