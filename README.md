# IT007: Operating System - Custom Shell Interface

#### Instructors: MS. Nguyen Thanh Thien

## Overview

This project implements a custom shell interface in C, designed for the IT007 - Operating System course. It allows users to execute commands, manage processes, and navigate the file system through a custom command-line interface.

## Features

*   **Command Execution:** Executes standard Unix commands.
*   **Process Management:** Supports running programs in the foreground and background.
*   **Directory Navigation:** Allows users to change directories.
*   **Simple Built-in Commands:** Implements basic built-in commands (e.g., `cd`, `exit`).
*   **Support Redirection and Pipeline:** Implement procedure solving input, output and pipeline (e.g., `>`, `<`, `|`)

## Files

*   **`23521467.c`:**  The main C source file containing the implementation of the custom shell.
*   **`BTLab6.pdf`:**  A lab assignment instructions and requirements.
*   **`README.md`:** Providing information about the project.

## Usage

1.  **Compilation:**

    ```
    gcc 23521467.c -o shell
    ```

2.  **Execution:**

    ```
    ./shell
    ```

3.  **Commands:**

    *   Type commands and press Enter to execute them.
    *   Use `cd` to change directories.
    *   Type `exit` to quit the shell.

## Implementation Details

The shell implementation likely involves the following components:

*   **Command Parsing:** Parsing user input into command and arguments.
*   **Process Creation:** Using `fork()` to create child processes for command execution.
*   **Command Execution:** Using `exec()` to replace the child process with the requested program.
*   **Signal Handling:** Handling signals for job control (e.g., Ctrl+C, Ctrl+Z).
*   **Input/Output Redirection:** Support input and output redirection (e.g., `>` , `<`).
*   **Piping:** Implement piping to chain commands together (e.g., `ls | grep .txt`).

## Potential Improvements

*   **More Built-in Commands:** Implement additional built-in commands for enhanced functionality (e.g., `history`, `alias`).
*   **Error Handling:** Improve error handling and provide more informative error messages.
*   **Job Control:** Enhance job control capabilities, such as bringing background processes to the foreground.
