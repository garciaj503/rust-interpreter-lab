# Flex Scanner

This folder contains the files needed to build a scanner.
It uses the `flex` tool.
`scanner.flex` contains the token definitions and what flex should return when it matches the given lexemes.
`token.h` contains an enum containing all the tokens.

## Build Process

To build the `scanner.c` run the following **flex** command.

```sh
flex -o scanner.c scanner.flex
```

Then build the `main.c` and `scanner.c` using **gcc**.

```sh
gcc main_test.c scanner.c -o scanner_test.out
```

Then execute the test suite using `./scanner_test.out`.

```sh
./scanner_test.out
```

## Scanner Only

```sh
flex -o scanner.c scanner.flex
gcc main.c scanner.c -o scanner.out
./scanner.out
```

## Using Make

Alternatively you can use the supplied make file.

* Build scanner.c - `make scanner.c`
* Build main program - `make`
* Build test program - `make test`
* Clean up build files - `make clean`