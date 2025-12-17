# Bison Interpreter

This folder contains a Bison Interpreter for a small set of Rust Programs. It build upon the files found in [Scanner](../scanner) and [Parser](../parser/)

## Build Process

To build the `scanner.c` run the following **flex** command.

```sh
flex -o scanner.c scanner.flex
```

The parser is built using **bison**.
Note this command also builds the `token.h`.

```sh
bison --defines=token.h --output=parser.c parser.bison
```

Then build the `main.c`, `expr.c`, `parcer.c` and `scanner.c` using **gcc**.
Note that you may get a few warnings that can be ignored.

```sh
gcc *.c -o interpreter.out
```

Then execute the main suite using `./interpreter.out` and pass in the input from your sample program.
For example here is a command for `samples/program.c`

```sh
./interpreter.out < samples/program.c
```

To execute the test suite, use the command `gcc main_test.c symbol_map.c expr.c scanner.c parser.c -o interpreter_test.out -lm` and then run `./interpreter_test.out`

Single line command for main suite:

```sh
flex -o scanner.c scanner.flex && \
bison --defines=token.h --output=parser.c parser.bison && \
gcc *.c -o interpreter.out && ./interpreter.out < samples/program.c
```

Single line command for test suite:

```sh
flex -o scanner.c scanner.flex && \
bison --defines=token.h --output=parser.c parser.bison && \
gcc main_test.c symbol_map.c expr.c scanner.c parser.c -o interpreter_test.out -lm &&  \
./interpreter_test.out
```

## Building with Make

To simplify the process a Makefile has been supplied.

* Build scanner - `make scanner.c`
* Build parser - `make parser.c parser.h` 
* Build main program - `make`
* Build test program - `make test`
* Clean up build files - `make clean`<br>
`./interpreter.out < samples/program.c`<br>
`./interpreter_test.out`

To remove the build files, use make clean.

```sh
make clean
```