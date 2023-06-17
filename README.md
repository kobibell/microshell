# Microshell (MSH)

Microshell is a simple shell program written in C. It allows users to execute commands and navigate the file system. It supports built-in commands like "cd", "help", and "exit", as well as external commands.

## Files

### msh.c

This file contains the main program loop and the "execute_command" function. The main loop reads the user's input, removes the newline character, and calls the "execute_command" function. The "execute_command" function checks for built-in commands, pipes, and the presence of the "grep" command. Depending on the input, it either executes the built-in command, handles the pipes, or forks a new process and executes the external program.

### helper.c

This file contains helper functions for the MSH program:

- "builtin_cd(char \*path)": Changes the current working directory to the specified path.
- "builtin_help()": Displays the available built-in commands and their descriptions.
- "split_command(char \*command, char \*\*args)": Splits the input command into an array of arguments.
- "execute_pipes(char \*command)": Handles the execution of piped commands.

## Usage

1. Compile the program using the following command:

```bash
  make
```

2. Run the compiled program:

```bash
  ./msh
```

3. Enter commands at the prompt:

```bash
  cssc1432% cd /path/to/directory
  cssc1432% help
  cssc1432% exit

```

## Requirements

- C compiler (e.g., GCC)
- Standard C library

## Notes

- This program is a basic implementation and may not support all features of a full-fledged shell.
- Some commands may not work as expected or have limitations.
- This program has been tested on Linux systems.

## Contributors

- Kobi Bell
- Mark Duraid
