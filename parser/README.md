# Bison Parser

This folder contains a Bison Parser for a small set of Rust prgrams. It builds upon the `scanner.flex` file found in [Scanner](../scanner)

## Build Process

To build the `scanner.c` run the following **flex** command.

```sh
flex -o scanner.c scanner.flex
```

The parser is built using **bison**.
Note this command also builds the `token.h`.

```sh
bison --defines=token.h --output=parser.c -v parser.bison
```

THe `-v` flag will create a `parser.output` which represents the grammar in text form.


### Running the Parser

Then build the `main.c`, `parcer.c` and `scanner.c` using **gcc**.
Note that you may get a few warnings that can be ignored.

```sh
gcc main.c scanner.c  parser.c -o parser.out
```

Then execute the test suite using `./parser.out` and pass in the input from your sample program.
For example here is a command for `samples/program.c`

```sh
./parser.out < samples/program.c
```


### Testing the Parser

Then build the `main_test.c`, `parcer.c` and `scanner.c` using **gcc**.
Note that you may get a few warnings that can be ignored.

```sh
gcc main_test.c scanner.c  parser.c -o parser_test.out
```

Then execute the test suite using `./parser_test.out`.

```sh
./parser_test.out
```


### Using Make

Alternatively you can use the supplied make file.

* Build scanner - `make scanner.c`
* Build parser - `make parser.c parser.h` 
* Build main program - `make`
* Build test program - `make test`
* Clean up build files - `make clean`