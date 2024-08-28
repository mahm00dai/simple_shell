# Simple Shell

## Contributors

See the list of [AUTHORS](./AUTHORS) who contributed to this project.

## Overview

The Simple Shell project implements a basic command-line shell in C, simulating the behavior of a UNIX shell. The shell handles command input, tokenizes commands, executes them, and supports built-in commands such as `exit` and `env`. It operates in both interactive and non-interactive modes, providing a prompt when running interactively.

## Features

- **Command Execution**: Executes commands using `execve`.
- **Tokenization**: Splits command strings into arguments.
- **Built-in Commands**: Supports `exit` to terminate the shell and `env` to print environment variables.
- **Error Handling**: Provides error messages for failed commands.
- **Memory Management**: Ensures proper handling and deallocation of memory to avoid leaks.

## Files

### `main.h`

This header file contains the function prototypes and external declarations used across the project. It includes:

- **Function Prototypes**:
  - `void simple_shell(void);`
  - `char *read_command(void);`
  - `int process_command(char *command);`
  - `int tokenize_command(char *command, char *argv[]);`
  - `void execute_command(char **argv);`
  - `void print_env(void);`
  - `void handle_error(char *command);`
  - `void exit_shell(void);`
- **External Declarations**:
  - `extern char **environ;`

### `shell.c`

This file contains the entry point of the shell program and the core function for running the shell:

- **`main`**: Entry point of the program.
- **`simple_shell`**: Main function that displays a prompt, reads commands, processes them, and continues until `Ctrl+D` or `exit` is encountered.

### `command_utils.c`

This file includes functions for processing and executing commands:

- **`process_command`**: Processes and executes commands, handling built-in commands like `exit` and `env`.
- **`tokenize_command`**: Tokenizes the command string into an array of arguments.
- **`execute_command`**: Executes a command using `execve` and handles errors if the command fails.

### `command_utils_helpers.c`

This file contains helper functions used by `command_utils.c`:

- **`print_env`**: Prints the environment variables to the standard output.
- **`handle_error`**: Handles errors by printing error messages to the standard error.

### `read_command.c`

This file provides the function to read user input:

- **`read_command`**: Reads a command from standard input. It handles EOF and allocates memory for the command string.

## Compilation

To compile the shell, use the following command:

```bash
gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o hsh
