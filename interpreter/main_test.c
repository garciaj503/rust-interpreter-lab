// https://github.com/sheredom/utest.h/blob/master/utest.h
#include "utest.h"
#include "expr.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* Clunky: Declare the parse function generated from parser.bison */
extern int yyparse();

/* Clunky: Declare the result of the parser from parser.bison */
extern struct stmt *parser_result;

typedef struct yy_buffer_state *YY_BUFFER_STATE;
extern void yyrestart(FILE *input_file);
extern YY_BUFFER_STATE yy_scan_buffer(char *str, int i);
extern YY_BUFFER_STATE yy_scan_string(char *str);
extern void yy_delete_buffer(YY_BUFFER_STATE buffer);
extern FILE *yyin;
extern int yylineno;

UTEST_MAIN();

char* read_file_dynamic(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("fopen");
        return NULL;
    }

    // Seek to the end of the file to determine its size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Allocate a buffer to hold the file contents
    char* buffer = (char*)malloc(file_size + 1);
    if (buffer == NULL) {
        perror("malloc");
        fclose(file);
        return NULL;
    }

    // Read the entire file into the buffer
    size_t read_size = fread(buffer, 1, file_size, file);
    if (read_size != file_size) {
        perror("fread");
        free(buffer);
        fclose(file);
        return NULL;
    }

    // Null-terminate the buffer
    buffer[file_size] = '\0';

    fclose(file);
    return buffer;
}

void redirect_stdout(const char *filename, int evalutate) {
  fflush(stdout);

  // Redirect stdout to a temporary file
  FILE *temp_file = fopen(filename, "w");
  if (temp_file == NULL) {
    perror("tmpfile");
    exit(EXIT_FAILURE);
  }
  int temp_fd = fileno(temp_file);
  int stdout_fd = dup(STDOUT_FILENO);
  dup2(temp_fd, STDOUT_FILENO);

  // Perform operations that generate output
  if (evalutate != 0) {
    stmt_evaluate(parser_result);
  } else {
    stmt_print(parser_result);
  }

  // Flush and close stdout
  fflush(stdout);
  dup2(stdout_fd, STDOUT_FILENO);
  close(stdout_fd);
}

struct InterpreterTestFile {
  const char *evaluated_file;
  const char *print_file;
  const char *test_file;
};

UTEST_F_SETUP(InterpreterTestFile) {
}

UTEST_F_TEARDOWN(InterpreterTestFile) {
  // and also assert and expect in teardown!
  yyin = fopen(utest_fixture->test_file, "r");
  yyrestart(yyin);
  ASSERT_TRUE(yyin);

  yylineno = 1;
  int result = yyparse();
  fclose(yyin);

  // Assert the result to test correctness
  ASSERT_EQ(result, 0);

  if (result == 0) {
    // Catch the standard output and compare with expected test result
    redirect_stdout("test_print.txt", 0);
    redirect_stdout("test_evaluate.txt", 1);
  }

  char *actual_print = read_file_dynamic("test_print.txt");
  char *expected_print = read_file_dynamic(utest_fixture->print_file);
  ASSERT_STREQ(expected_print, actual_print);

  char *actual_evaluate = read_file_dynamic("test_evaluate.txt");
  char *expected_evaluate = read_file_dynamic(utest_fixture->evaluated_file);
  ASSERT_STREQ(expected_evaluate, actual_evaluate);
}


UTEST_F(InterpreterTestFile, Addition) {
  utest_fixture->test_file = "samples/addition.rs";
  utest_fixture->print_file = "samples/outputs/addition_expected_print.txt";
  utest_fixture->evaluated_file = "samples/outputs/addition_expected_evaluate.txt";
}

UTEST_F(InterpreterTestFile, Subtract) {
  utest_fixture->test_file = "samples/subtract.rs";
  utest_fixture->print_file = "samples/outputs/subtract_expected_print.txt";
  utest_fixture->evaluated_file = "samples/outputs/subtract_expected_evaluate.txt";
}

UTEST_F(InterpreterTestFile, Multiplication) {
  utest_fixture->test_file = "samples/multiplication.rs";
  utest_fixture->print_file = "samples/outputs/multiplication_expected_print.txt";
  utest_fixture->evaluated_file = "samples/outputs/multiplication_expected_evaluate.txt";
}

UTEST_F(InterpreterTestFile, Division) {
  utest_fixture->test_file = "samples/division.rs";
  utest_fixture->print_file = "samples/outputs/division_expected_print.txt";
  utest_fixture->evaluated_file = "samples/outputs/division_expected_evaluate.txt";
}

UTEST_F(InterpreterTestFile, Branching) {
  utest_fixture->test_file = "samples/branching.rs";
  utest_fixture->print_file = "samples/outputs/branching_expected_print.txt";
  utest_fixture->evaluated_file = "samples/outputs/branching_expected_evaluate.txt";
}

UTEST_F(InterpreterTestFile, Array) {
  utest_fixture->test_file = "samples/array.rs";
  utest_fixture->print_file = "samples/outputs/array_expected_print.txt";
  utest_fixture->evaluated_file = "samples/outputs/array_expected_evaluate.txt";
}

UTEST_F(InterpreterTestFile, printFunction) {
  utest_fixture->test_file = "samples/print.rs";
  utest_fixture->print_file = "samples/outputs/print_expected_print.txt";
  utest_fixture->evaluated_file = "samples/outputs/print_expected_evaluate.txt";
}

UTEST_F(InterpreterTestFile, Looping) {
  utest_fixture->test_file = "samples/looping.rs";
  utest_fixture->print_file = "samples/outputs/looping_expected_print.txt";
  utest_fixture->evaluated_file = "samples/outputs/looping_expected_evaluate.txt";
}

UTEST_F(InterpreterTestFile, QuadraticFunction) {
  utest_fixture->test_file = "samples/QuadraticFunction.rs";
  utest_fixture->print_file = "samples/outputs/quadratic_function_expected_print.txt";
  utest_fixture->evaluated_file = "samples/outputs/quadratic_function_expected_evaluate.txt";
}

UTEST_F(InterpreterTestFile, Sorting) {
  utest_fixture->test_file = "samples/Sorting.rs";
  utest_fixture->print_file = "samples/outputs/sorting_expected_print.txt";
  utest_fixture->evaluated_file = "samples/outputs/sorting_expected_evaluate.txt";
}

