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

The minimal version of a shell

## Features

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
git clone https://github.com/your-username/repo-name.git
```
than cd to the repository and run make command to build the Minishell program:
```bash
make
```
for the bonuses use

```bash
make bonus
```

## Debugging
```bash
DEBUG=1 make re
```

## Usage
