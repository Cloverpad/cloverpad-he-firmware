use clap::Parser;
use cloverpad_core::models::{self, Keypad};

/// Starts the interactive shell for configuring keypads.
pub fn start_interactive_shell() -> anyhow::Result<()> {
    // Attempt to select the first available keypad as the current one
    let mut current_keypad = Keypad::find_available_keypads()?.into_iter().take(1).next();

    // Start the main loop for the interactive shell
    // NOTE: clap needs an additional parameter at the start, otherwise it won't parse correctly
    let base_args = [""];
    let mut rl = rustyline::DefaultEditor::new()?;
    println!("\nMore More Keypad! Configurator");

    loop {
        let prompt = if let Some(keypad) = &current_keypad {
            format!(
                "Current Keypad: {} ({})\n>> ",
                keypad.variant, &keypad.com_port
            )
        } else {
            ">> ".to_string()
        };

        let input = rl.readline(&prompt)?;
        println!();

        if let Some(input_args) = shlex::split(&input) {
            match ShellCommand::try_parse_from(
                base_args
                    .into_iter()
                    .chain(input_args.iter().map(|s| s.as_str())),
            ) {
                Ok(command) => command.execute(&mut rl, &mut current_keypad)?,
                Err(e) => eprintln!("{e}"),
            }

            println!();
        }
    }
}

/// Interactive shell for configurating the More More Keypad!
#[derive(Debug, Parser)]
pub enum ShellCommand {
    /// Exits the interactive shell
    Exit,

    /// Prints a list of available keypads and displays a prompt to select one
    SelectKeypad,

    /// Gets the firmware version of the currently selected keypad
    FirmwareVersion,
}

impl ShellCommand {
    /// Executes this shell command.
    pub fn execute(
        &self,
        rl: &mut rustyline::DefaultEditor,
        current_keypad: &mut Option<Keypad>,
    ) -> anyhow::Result<()> {
        match self {
            ShellCommand::Exit => std::process::exit(0),
            ShellCommand::SelectKeypad => Self::select_keypad(rl, current_keypad)?,
            ShellCommand::FirmwareVersion => Self::firmware_version(current_keypad),
        }

        Ok(())
    }

    /// Executes the command for selecting a keypad from the available list.
    fn select_keypad(
        rl: &mut rustyline::DefaultEditor,
        current_keypad: &mut Option<Keypad>,
    ) -> anyhow::Result<()> {
        match Keypad::find_available_keypads() {
            Ok(keypads) => {
                if keypads.is_empty() {
                    println!("No keypads found");
                } else {
                    println!("Select a keypad from the list below (or type 'c' to cancel):");
                    for (i, keypad) in keypads.iter().enumerate() {
                        println!("{}) {}", i, keypad);
                    }

                    let selected_keypad = loop {
                        let input = rl.readline_with_initial(">> ", ("0", ""))?;

                        if input.eq_ignore_ascii_case("c") {
                            break None;
                        }

                        match input.parse::<usize>() {
                            Ok(index) if index < keypads.len() => {
                                break keypads.get(index).cloned();
                            }
                            _ => {}
                        }
                    };

                    if selected_keypad.is_some() {
                        *current_keypad = selected_keypad;
                    }
                }
            }
            Err(e) => eprintln!("An error occurred while searching for keypads: {}", e),
        }

        Ok(())
    }

    /// Helper function for printing an error that occurred on the keypad.
    fn print_keypad_error(response: cloverpad_core::error::Result<models::Response>) {
        match response {
            Ok(models::Response { code, .. }) => {
                eprintln!(
                    "An error occurred while processing the command: {:?}",
                    models::ResponseCode::try_from(code)
                );
            }
            Err(e) => eprintln!("Unable to send command: {:?}", e),
        }
    }

    /// Executes the "GetFirmwareVersion" command.
    fn firmware_version(current_keypad: &mut Option<Keypad>) {
        if let Some(keypad) = current_keypad {
            let version_command = models::Command {
                r#type: Some(models::command::Type::FirmwareVersion(
                    models::FirmwareVersion {},
                )),
            };

            match keypad.send_command(&version_command) {
                Ok(models::Response {
                    code: 0,
                    data: Some(models::response::Data::FirmwareVersion(version_response)),
                }) => {
                    println!("Firmware Version: {}", version_response.version)
                }
                e => Self::print_keypad_error(e),
            }
        } else {
            println!("Please select a keypad first");
        }
    }
}
