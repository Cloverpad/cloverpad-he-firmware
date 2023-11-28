use std::io::Result;

fn main() -> Result<()> {
    // Rebuild if protobuf defintions have changed
    // The .options files are specific to nanopb, so no need to rebuild if they change
    println!("cargo:rerun-if-changed=../../protocol/*.proto");

    prost_build::compile_protos(&["../../protocol/protocol-commands.proto"], &["../../protocol"])?;
    Ok(())
}
