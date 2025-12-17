// https://github.com/sheredom/utest.h/blob/master/utest.h
#include "utest.h"
#include <stdio.h>

extern int yyparse();

typedef struct yy_buffer_state *YY_BUFFER_STATE;
extern void yyrestart(FILE *input_file);
extern YY_BUFFER_STATE yy_scan_buffer(char *str, int i);
extern YY_BUFFER_STATE yy_scan_string(char *str);
extern void yy_delete_buffer(YY_BUFFER_STATE buffer);
extern FILE *yyin;
extern int yylineno;

UTEST_MAIN();



UTEST(parser, math) {
  char string[] = "println!(1+8/4-3);\0"; 
  YY_BUFFER_STATE buffer = yy_scan_buffer(string, sizeof(string));
  yylineno = 1;
  ASSERT_EQ(yyparse(), 0);
  yy_delete_buffer(buffer);
}


UTEST(parser, missing_semi_colon) {
  char string[] = "println!(1+8/4-3)\0"; 
  YY_BUFFER_STATE buffer = yy_scan_buffer(string, sizeof(string));
  yylineno = 1;
  ASSERT_NE(yyparse(), 0);  // should FAIL
  yy_delete_buffer(buffer);
}

UTEST(parser, sample) {
  yyin = fopen("samples/program.c", "r");
  ASSERT_TRUE(yyin);
  yylineno = 1;
  yyrestart(yyin);
  ASSERT_EQ(yyparse(), 0);
  fclose(yyin);
}

UTEST(parser, variable_declaration) {
  char string[] = "let x: i32 = 5;\0";
  YY_BUFFER_STATE buffer = yy_scan_buffer(string, sizeof(string));
  yylineno = 1;
  ASSERT_EQ(yyparse(), 0);
  yy_delete_buffer(buffer);
}

UTEST(parser, mutable_variable_declaration) {
  char string[] = "let mut y: i32 = 10;\0";
  YY_BUFFER_STATE buffer = yy_scan_buffer(string, sizeof(string));
  yylineno = 1;
  ASSERT_EQ(yyparse(), 0);
  yy_delete_buffer(buffer);
}

UTEST(parser, assignment) {
  char string[] = "let mut x: i32 = 1; x = x + 3;\0";
  YY_BUFFER_STATE buffer = yy_scan_buffer(string, sizeof(string));
  yylineno = 1;
  ASSERT_EQ(yyparse(), 0);
  yy_delete_buffer(buffer);
}

UTEST(parser, print_basic) {
  char string[] = "println!(\"Hello\");\0";
  YY_BUFFER_STATE buffer = yy_scan_buffer(string, sizeof(string));
  yylineno = 1;
  ASSERT_EQ(yyparse(), 0);
  yy_delete_buffer(buffer);
}

UTEST(parser, if_statement) {
  char string[] = "if x < 5 { println!(x); }\0";
  YY_BUFFER_STATE buffer = yy_scan_buffer(string, sizeof(string));
  yylineno = 1;
  ASSERT_EQ(yyparse(), 0);
  yy_delete_buffer(buffer);
}

UTEST(parser, for_loop) {
  char string[] = "let arr: i32; for i in 0..len(arr) { println!(i); }\0";
  YY_BUFFER_STATE buffer = yy_scan_buffer(string, sizeof(string));
  yylineno = 1;
  ASSERT_EQ(yyparse(), 0);
  yy_delete_buffer(buffer);
}

UTEST(parser, array_index) {
  char string[] = "let mut arr: i32; arr[0] = 3; println!(arr[0]);\0";
  YY_BUFFER_STATE buffer = yy_scan_buffer(string, sizeof(string));
  yylineno = 1;
  ASSERT_EQ(yyparse(), 0);
  yy_delete_buffer(buffer);
}

UTEST(parser, sample_helloworld) {
  yyin = fopen("samples/HelloWorld.rs", "r");
  ASSERT_TRUE(yyin);
  yylineno = 1;
  yyrestart(yyin);
  ASSERT_EQ(yyparse(), 0);
  fclose(yyin);
}

UTEST(parser, sample_quadratic) {
  yyin = fopen("samples/QuadraticFunction.rs", "r");
  ASSERT_TRUE(yyin);
  yylineno = 1;
  yyrestart(yyin);
  ASSERT_EQ(yyparse(), 0);
  fclose(yyin);
}

UTEST(parser, sample_sorting) {
  yyin = fopen("samples/Sorting.rs", "r");
  ASSERT_TRUE(yyin);
  yylineno = 1;
  yyrestart(yyin);
  ASSERT_EQ(yyparse(), 0);
  fclose(yyin);
}
