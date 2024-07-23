<h1 align="center">
	🧩 Minishell
</h1>

<p align="center">
	<b><i>As beautiful as a shell</i></b><br>
</p>

## Table of Contents

- [Introduction](#introduction)
- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)

## Introduction
This program was created by Aiko Achappui and Tal Kashi as part of the 42 school curriculum. Minishell is a minimal version of a shell that supports both Linux and macOS.

For parsing and execution, we utilized an Abstract Syntax Tree (AST). The AST processes commands recursively until it reaches the simplest level(smallest leaves), known as a "simple command," such as
```bash
cat file.c
```


## Features

1. Logical Operators: Supports logical operators such as && (AND) and || (OR).
2. Allows the use of multiple pipes (|) for chaining commands.
3. Supports parenthesis () to create subshells for grouped command execution.
4. Redirections:
	- Input redirection with <.
	- Output redirection with >.
	- Heredoc redirection with <<.
	- Append redirection with >>.

5. Environment Variables: Use $ to access and manipulate environment variables within commands.
6. Wildcard Support: Supports the use of * wildcards to match patterns within the current directory.( Examples include .* and *c**a, without using relative paths.)
7. Quoting Mechanisms: Single quotes (') to treat enclosed text as a literal string.
8. Double quotes (") to cancel metadata charachters.
9. Builtin Functions: Implements several built-in shell functions:
	- echo: Print arguments to the standard output.
	- pwd: Print the current working directory.
	- env: Display the environment variables.
	- cd: Change the current directory.
	- export: Set environment variables.
	- unset: Unset environment variables.
	- exit: Exit the shell.

## Explanations

a short explanation of the process the line goes through :
1. tokenizer: returning a pointer to a head of a linked list. a basic "split" function that defines the type of the token. its skips whitespaces and stops when encountering one of the following : "||" "&&" "|" "(" ")".
2. syntax analyzer
checks before running the programm that the order of the operators makes sences, (refering from bash) and relatively to the position of the operator inside the line.
3. tree maker :
looks for the righest operator he finds ( || or && ) and with each one it encounter, it creates another 2 childs. than recursively calling the function, searching for the pipes and for the parenthesis (for parenthesis it forks once than a parenthesis have only one child), than looking for the smallest unites of command lines.
a command line includes the redirections.
(include here the template)


## Installation

In order to run minishell, first clone the project using

```bash
git clone git@github.com:Talkashi1111/mini_shell.git
```
than cd to the repository and run make command to build the Minishell program:


- Regular build:
	```sh
	make
	```
- You can also test for memory leaks using your own arguments or the default ones by running:

```bash
make leak
```
The project's Makefile automatically configures the memory leak detection tool based on the operating system:

<b>- Linux:</b> By default, the tool is set to valgrind.

<b>- Mac:</b>  The tool is set to leaks with the --atexit option.
## Debugging

- A debugging mode is available to visualize the command tree structure.
- Enable debugging by running `DEBUG=1 make re` during installation.

```bash
DEBUG=1 make re
```

3. **Run the Program**:
```bash
./minishell
```

---

Thank you for looking through our ***Minishell***! If you have any questions or issues, or ideas to improve it, don't hesitate to contact me ( or Aiko).
