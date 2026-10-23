# s3 — Software Systems Shell

A shell written in C for Software Systems Assignment 1. The project currently implements basic command execution from Section 1 of the assignment.

## Features implemented so far

- Displays the `[s3]$` prompt and reads a command from the user.
- Splits commands into space-separated arguments using `strtok`, creating a null-terminated argument array.
- Uses `fork()` to create a child process for an external command.
- Uses `execvp()` in the child to run the command, searching the system's `PATH` for its executable.
- Waits for the child to finish before accepting the next command. This is handled by `reap()` in `main`, which calls `wait(NULL)`.
- Handles `exit` in the shell process so that it closes the shell.
- Ignores empty commands.
- Prints an error if `execvp()` fails, then exits the child process.

## Project files

| File | Purpose |
| --- | --- |
| `s3main.c` | Main loop: reads, parses and launches commands, then waits for completion. |
| `s3.c` | Implementations of the prompt, input, parsing and command-launching functions. |
| `s3.h` | Shared declarations, constants, library includes and the `reap()` helper. |
| `reference.txt` | Supporting reference notes. |

## Compile and run

From the project directory, compile with:

```sh
clang -Wall -Wextra s3main.c s3.c -o s3
```

This compiles and links both C source files into an executable named `s3`. The header is included by the source files and does not need to be compiled separately. Recompile after changing the source code.

Start the shell with:

```sh
./s3
```

The `./` tells the terminal to run the executable in the current directory.

Example commands to try at the shell prompt:

```text
[s3]$ whoami
[s3]$ pwd
[s3]$ ls
[s3]$ ls -l
[s3]$ exit
```

`whoami` has been manually tested and printed the current username. The other commands above are suggested checks.

## Current limitations and remaining work

Parsing currently splits on spaces only; quoted arguments and escaping are not supported. Input is limited by `MAX_LINE` (1024 bytes), and the argument array has `MAX_ARGS` (128) slots, including its terminating null pointer. End-of-input currently produces an error and exits.

The following assignment features have not yet been implemented:

- Input and output redirection (`<`, `>` and `>>`).
- The built-in `cd` command, including home and previous-directory support.
- A prompt showing the current working directory.
- Pipelines (`|`).
- Batched commands (`;`).
- Subshells and nested subshells.

No stretch features have been implemented yet.
