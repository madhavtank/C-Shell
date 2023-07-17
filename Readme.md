# C-shell
A unix shell implemented completely in C with many functionalities

# More added features
- Autocomplete feature
- Piping
- I/O redirection
- Some user defined commands

## Usage 
1. run make
2. execute a.out (run ./a.out)

## Assumptions

1. The length of any path or command is at max 1000.
2. Autocomplete works only in current directory

### Display Prompt
---> Prompt displays username, system name and current working direcotry

### Semi-colon separated commands
---> Shell is able to execute multiple semi-colon separated commands as well.

### Redirection,piping and signals
---> as in linux shell

### bg, fg and jobs
---> as asked in pdf

## Builtin Commands

---> Implemented as mentioned in Assignment pdf

    1.cd
    2.pwd
    3.echo
    4.ls
    5.history
    6.discover
    7.pifno

## Other Commands

### Foregroung Processes:

- You can execute any system command with or without arguments as a foreground process but you will have to wait till it executed. After its completion, the prompt will also show the time it took to get executed.

- In case of multiple foreground processes, the time taken by the foreground process that ended last will be printed.

### Background Process:

- By appending '&' to any command(with at least one space), the command is executed as a background process and the PID of newly created child process is printed.

- When such command is executed completely, the shell will also print whether the process is exited normally or abnormally.

## File Structure:

```
2021101108_Assignment2/

    makefile

    Readme.md-->Conriains description of shell

    a.out-->executable that will be generated when make is executed

    z.txt-->text file that stores last 20 commands

    1.h and functions.h-->header files containing Declaration of functions and some global arrays

    main.c-->Conatains the main function of the shell

    rest files contain the functions for the commands according to their names.