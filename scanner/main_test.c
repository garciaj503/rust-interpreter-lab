#include "token.h"
// https://github.com/sheredom/utest.h/blob/master/utest.h
#include "utest.h"
#include <stdio.h>

typedef struct yy_buffer_state *YY_BUFFER_STATE;
extern void yyrestart(FILE * input_file);
extern YY_BUFFER_STATE yy_scan_buffer(char *str, int i);
extern void yy_delete_buffer(YY_BUFFER_STATE buffer);
extern FILE *yyin;
extern int yylex();
extern char *yytext;

UTEST_MAIN();

struct token_st {
  token_t t;
  char *p;
};

UTEST(scanner, Identifier) {
  token_t t;
  // Must include the null character to terminate input
  char string[] = "numbers\0"; 
  YY_BUFFER_STATE buffer = yy_scan_buffer(string, sizeof(string));

  ASSERT_EQ(TOKEN_IDENT, (t = yylex()));
  ASSERT_STREQ(string, yytext);

  ASSERT_EQ(TOKEN_EOF, (t = yylex()));
  ASSERT_STREQ("", yytext);

  yy_delete_buffer(buffer);
}

UTEST(scanner, Assignment) {
  token_t t;
  // Must include the null character to terminate input
  char string[] = "=\0"; 
  YY_BUFFER_STATE buffer = yy_scan_buffer(string, sizeof(string));

  ASSERT_EQ(TOKEN_ASSIGNMENT, (t = yylex()));
  ASSERT_STREQ(string, yytext);

  ASSERT_EQ(TOKEN_EOF, (t = yylex()));
  ASSERT_STREQ("", yytext);

  yy_delete_buffer(buffer);
}

UTEST(scanner, WhileKeyword) {
  token_t t;
  // Must include the null character to terminate input
  char string[] = "while\0"; 
  YY_BUFFER_STATE buffer = yy_scan_buffer(string, sizeof(string));

  ASSERT_EQ(TOKEN_KEYWORD_WHILE, (t = yylex()));
  ASSERT_STREQ(string, yytext);

  ASSERT_EQ(TOKEN_EOF, (t = yylex()));
  ASSERT_STREQ("", yytext);

  yy_delete_buffer(buffer);
}

UTEST(scanner, SQRTFunction) {
  token_t t;
  // Must include the null character to terminate input
  char string[] = "isqrt()\0"; 
  YY_BUFFER_STATE buffer = yy_scan_buffer(string, sizeof(string));

  ASSERT_EQ(TOKEN_FUNCTION_ISQRT, (t = yylex()));
  ASSERT_STREQ(string, yytext);

  ASSERT_EQ(TOKEN_EOF, (t = yylex()));
  ASSERT_STREQ("", yytext);

  yy_delete_buffer(buffer);
}

UTEST(scanner, LENFunction) {
  token_t t;
  // Must include the null character to terminate input
  char string[] = "len()\0"; 
  YY_BUFFER_STATE buffer = yy_scan_buffer(string, sizeof(string));

  ASSERT_EQ(TOKEN_FUNCTION_LEN, (t = yylex()));
  ASSERT_STREQ(string, yytext);

  ASSERT_EQ(TOKEN_EOF, (t = yylex()));
  ASSERT_STREQ("", yytext);

  yy_delete_buffer(buffer);
}

UTEST(scanner, ForKeyword) {
  token_t t;
  // Must include the null character to terminate input
  char string[] = "for\0"; 
  YY_BUFFER_STATE buffer = yy_scan_buffer(string, sizeof(string));

  ASSERT_EQ(TOKEN_KEYWORD_FOR, (t = yylex()));
  ASSERT_STREQ(string, yytext);

  ASSERT_EQ(TOKEN_EOF, (t = yylex()));
  ASSERT_STREQ("", yytext);

  yy_delete_buffer(buffer);
}

UTEST(scanner, Digit) {
  token_t t;
  // Must include the null character to terminate input
  char string[] = "23476\0"; 
  YY_BUFFER_STATE buffer = yy_scan_buffer(string, sizeof(string));

  ASSERT_EQ(TOKEN_NUMBER, (t = yylex()));
  ASSERT_STREQ(string, yytext);

  ASSERT_EQ(TOKEN_EOF, (t = yylex()));
  ASSERT_STREQ("", yytext);

  yy_delete_buffer(buffer);
}

UTEST(scanner, HelloWorld) {
  struct token_st tokens[] = {
    {TOKEN_LEFT_CURLY_BRACE, "{"},
    {TOKEN_KEYWORD_PRINTLN_NOT, "println!"},
    {TOKEN_LEFT_PARENTHESIS, "("},
    {TOKEN_STRING, "\"Hello world!\""},
    {TOKEN_RIGHT_PARENTHESIS, ")"},
    {TOKEN_SEMICOLON, ";"},
    {TOKEN_RIGHT_CURLY_BRACE, "}"},
    {TOKEN_EOF, ""}
  };

  yyin = fopen("samples/HelloWorld.rs", "r");
  yyrestart(yyin);
  ASSERT_TRUE(yyin);

  int index = 0;
  token_t t;
  do {
    t = yylex();
    ASSERT_EQ(tokens[index].t, t);
    ASSERT_STREQ(tokens[index].p, yytext);
    ++index;
  } while (t != TOKEN_EOF);
}

UTEST(scanner, QuadraticFunction) {
  struct token_st tokens[] = {
    {TOKEN_LEFT_CURLY_BRACE, "{"},
    {TOKEN_KEYWORD_LET, "let"},
    {TOKEN_IDENT, "a"},
    {TOKEN_ASSIGNMENT, "="},
    {TOKEN_NUMBER, "1"},
    {TOKEN_SEMICOLON, ";"},

    {TOKEN_KEYWORD_LET, "let"},
    {TOKEN_IDENT, "b"},
    {TOKEN_ASSIGNMENT, "="},
    {TOKEN_SUBTRACT, "-"},
    {TOKEN_NUMBER, "3"},
    {TOKEN_SEMICOLON, ";"},

    {TOKEN_KEYWORD_LET, "let"},
    {TOKEN_IDENT, "c"},
    {TOKEN_ASSIGNMENT, "="},
    {TOKEN_NUMBER, "2"},
    {TOKEN_SEMICOLON, ";"},

    {TOKEN_KEYWORD_LET, "let"},
    {TOKEN_IDENT, "discriminant"},
    {TOKEN_COLON, ":"},
    {TOKEN_INTEGER_IDENT, "i32"},
    {TOKEN_ASSIGNMENT, "="},
    {TOKEN_IDENT, "b"},
    {TOKEN_MULTIPLY, "*"},
    {TOKEN_IDENT, "b"},
    {TOKEN_SUBTRACT, "-"},
    {TOKEN_NUMBER, "4"},
    {TOKEN_MULTIPLY, "*"},
    {TOKEN_IDENT, "a"},
    {TOKEN_MULTIPLY, "*"},
    {TOKEN_IDENT, "c"},
    {TOKEN_SEMICOLON, ";"},


    {TOKEN_KEYWORD_LET, "let"},
    {TOKEN_IDENT, "root1"},
    {TOKEN_ASSIGNMENT, "="},
    {TOKEN_LEFT_PARENTHESIS, "("},
    {TOKEN_SUBTRACT, "-"},
    {TOKEN_IDENT, "b"},
    {TOKEN_ADD, "+"},
    {TOKEN_IDENT, "discriminant"},
    {TOKEN_DOT, "."},
    {TOKEN_FUNCTION_ISQRT, "isqrt()"},
    {TOKEN_RIGHT_PARENTHESIS, ")"},
    {TOKEN_DIVIDE, "/"},
    {TOKEN_LEFT_PARENTHESIS, "("},
    {TOKEN_NUMBER, "2"},
    {TOKEN_MULTIPLY, "*"},
    {TOKEN_IDENT, "a"},
    {TOKEN_RIGHT_PARENTHESIS, ")"},
    {TOKEN_SEMICOLON, ";"},

    {TOKEN_KEYWORD_LET, "let"},
    {TOKEN_IDENT, "root2"},
    {TOKEN_ASSIGNMENT, "="},
    {TOKEN_LEFT_PARENTHESIS, "("},
    {TOKEN_SUBTRACT, "-"},
    {TOKEN_IDENT, "b"},
    {TOKEN_SUBTRACT, "-"},
    {TOKEN_IDENT, "discriminant"},
    {TOKEN_DOT, "."},
    {TOKEN_FUNCTION_ISQRT, "isqrt()"},
    {TOKEN_RIGHT_PARENTHESIS, ")"},
    {TOKEN_DIVIDE, "/"},
    {TOKEN_LEFT_PARENTHESIS, "("},
    {TOKEN_NUMBER, "2"},
    {TOKEN_MULTIPLY, "*"},
    {TOKEN_IDENT, "a"},
    {TOKEN_RIGHT_PARENTHESIS, ")"},
    {TOKEN_SEMICOLON, ";"},


    {TOKEN_KEYWORD_PRINTLN_NOT, "println!"},
    {TOKEN_LEFT_PARENTHESIS, "("},
    {TOKEN_STRING, "\"Two distinct real roots:\""},
    {TOKEN_RIGHT_PARENTHESIS, ")"},
    {TOKEN_SEMICOLON, ";"},

    {TOKEN_KEYWORD_PRINTLN_NOT, "println!"},
    {TOKEN_LEFT_PARENTHESIS, "("},
    {TOKEN_STRING, "\"{}\""},
    {TOKEN_COMMA, ","},
    {TOKEN_IDENT, "root1"},
    {TOKEN_RIGHT_PARENTHESIS, ")"},
    {TOKEN_SEMICOLON, ";"},

    {TOKEN_KEYWORD_PRINTLN_NOT, "println!"},
    {TOKEN_LEFT_PARENTHESIS, "("},
    {TOKEN_STRING, "\"{}\""},
    {TOKEN_COMMA, ","},
    {TOKEN_IDENT, "root2"},
    {TOKEN_RIGHT_PARENTHESIS, ")"},
    {TOKEN_SEMICOLON, ";"},

    {TOKEN_RIGHT_CURLY_BRACE, "}"},
    {TOKEN_EOF, ""}
  };

  yyin = fopen("samples/QuadraticFunction.rs", "r");
  yyrestart(yyin);
  ASSERT_TRUE(yyin);

  int index = 0;
  token_t t;
  do {
    t = yylex();
    ASSERT_EQ(tokens[index].t, t);
    ASSERT_STREQ(tokens[index].p, yytext);
    ++index;
  } while (t != TOKEN_EOF);
}

UTEST(scanner, Sorting) {
  struct token_st tokens[] = {
    {TOKEN_LEFT_CURLY_BRACE, "{"},
    {TOKEN_KEYWORD_LET, "let"},
    {TOKEN_KEYWORD_MUT, "mut"},
    {TOKEN_IDENT, "numbers"},
    {TOKEN_ASSIGNMENT, "="},
    {TOKEN_LEFT_BRACKET, "["},
    {TOKEN_NUMBER, "42"},
    {TOKEN_COMMA, ","},
    {TOKEN_NUMBER, "7"},
    {TOKEN_COMMA, ","},
    {TOKEN_NUMBER, "19"},
    {TOKEN_COMMA, ","},
    {TOKEN_NUMBER, "3"},
    {TOKEN_COMMA, ","},
    {TOKEN_NUMBER, "25"},
    {TOKEN_COMMA, ","},
    {TOKEN_NUMBER, "14"},
    {TOKEN_RIGHT_BRACKET, "]"},
    {TOKEN_SEMICOLON, ";"},
 
    {TOKEN_KEYWORD_PRINTLN_NOT, "println!"},
    {TOKEN_LEFT_PARENTHESIS, "("},
    {TOKEN_STRING, "\"Before sorting:\""},
    {TOKEN_RIGHT_PARENTHESIS, ")"},
    {TOKEN_SEMICOLON, ";"},
 
    {TOKEN_KEYWORD_PRINTLN_NOT, "println!"},
    {TOKEN_LEFT_PARENTHESIS, "("},
    {TOKEN_STRING, "\"{:?}\""},
    {TOKEN_COMMA, ","},
    {TOKEN_IDENT, "numbers"},
    {TOKEN_RIGHT_PARENTHESIS, ")"},
    {TOKEN_SEMICOLON, ";"},
 
    {TOKEN_KEYWORD_LET, "let"},
    {TOKEN_KEYWORD_LEN, "len"},
    {TOKEN_ASSIGNMENT, "="},
    {TOKEN_IDENT, "numbers"},
    {TOKEN_DOT, "."},
    {TOKEN_FUNCTION_LEN, "len()"},
    {TOKEN_SEMICOLON, ";"},
 
    {TOKEN_KEYWORD_FOR, "for"},
    {TOKEN_IDENT, "i"},
    {TOKEN_KEYWORD_IN, "in"},
    {TOKEN_NUMBER, "0"},
    {TOKEN_DOT, "."},
    {TOKEN_DOT, "."},
    {TOKEN_KEYWORD_LEN, "len"},
    {TOKEN_LEFT_CURLY_BRACE, "{"},
 
    {TOKEN_KEYWORD_FOR, "for"},
    {TOKEN_IDENT, "j"},
    {TOKEN_KEYWORD_IN, "in"},
    {TOKEN_NUMBER, "0"},
    {TOKEN_DOT, "."},
    {TOKEN_DOT, "."},
    {TOKEN_KEYWORD_LEN, "len"},
    {TOKEN_SUBTRACT, "-"},
    {TOKEN_IDENT, "i"},
    {TOKEN_SUBTRACT, "-"},
    {TOKEN_NUMBER, "1"},
    {TOKEN_LEFT_CURLY_BRACE, "{"},
 
    {TOKEN_KEYWORD_IF, "if"},
    {TOKEN_IDENT, "numbers"},
    {TOKEN_LEFT_BRACKET, "["},
    {TOKEN_IDENT, "j"},
    {TOKEN_RIGHT_BRACKET, "]"},
    {TOKEN_GREATER_THAN, ">"},
    {TOKEN_IDENT, "numbers"},
    {TOKEN_LEFT_BRACKET, "["},
    {TOKEN_IDENT, "j"},
    {TOKEN_ADD, "+"},
    {TOKEN_NUMBER, "1"},
    {TOKEN_RIGHT_BRACKET, "]"},
    {TOKEN_LEFT_CURLY_BRACE, "{"},
 
    {TOKEN_KEYWORD_LET, "let"},
    {TOKEN_IDENT, "a"},
    {TOKEN_ASSIGNMENT, "="},
    {TOKEN_IDENT, "numbers"},
    {TOKEN_LEFT_BRACKET, "["},
    {TOKEN_IDENT, "j"},
    {TOKEN_RIGHT_BRACKET, "]"},
    {TOKEN_SEMICOLON, ";"},
 
    {TOKEN_KEYWORD_LET, "let"},
    {TOKEN_IDENT, "b"},
    {TOKEN_ASSIGNMENT, "="},
    {TOKEN_IDENT, "numbers"},
    {TOKEN_LEFT_BRACKET, "["},
    {TOKEN_IDENT, "j"},
    {TOKEN_ADD, "+"},
    {TOKEN_NUMBER, "1"},
    {TOKEN_RIGHT_BRACKET, "]"},
    {TOKEN_SEMICOLON, ";"},
 
    {TOKEN_IDENT, "numbers"},
    {TOKEN_LEFT_BRACKET, "["},
    {TOKEN_IDENT, "j"},
    {TOKEN_RIGHT_BRACKET, "]"},
    {TOKEN_ASSIGNMENT, "="},
    {TOKEN_IDENT, "b"},
    {TOKEN_SEMICOLON, ";"},
 
    {TOKEN_IDENT, "numbers"},
    {TOKEN_LEFT_BRACKET, "["},
    {TOKEN_IDENT, "j"},
    {TOKEN_ADD, "+"},
    {TOKEN_NUMBER, "1"},
    {TOKEN_RIGHT_BRACKET, "]"},
    {TOKEN_ASSIGNMENT, "="},
    {TOKEN_IDENT, "a"},
    {TOKEN_SEMICOLON, ";"},
    {TOKEN_RIGHT_CURLY_BRACE, "}"},
    {TOKEN_RIGHT_CURLY_BRACE, "}"},
    {TOKEN_RIGHT_CURLY_BRACE, "}"},
 
    {TOKEN_KEYWORD_PRINTLN_NOT, "println!"},
    {TOKEN_LEFT_PARENTHESIS, "("},
    {TOKEN_STRING, "\"After sorting:\""},
    {TOKEN_RIGHT_PARENTHESIS, ")"},
    {TOKEN_SEMICOLON, ";"},
 
    {TOKEN_KEYWORD_PRINTLN_NOT, "println!"},
    {TOKEN_LEFT_PARENTHESIS, "("},
    {TOKEN_STRING, "\"{:?}\""},
    {TOKEN_COMMA, ","},
    {TOKEN_IDENT, "numbers"},
    {TOKEN_RIGHT_PARENTHESIS, ")"},
    {TOKEN_SEMICOLON, ";"},
    {TOKEN_RIGHT_CURLY_BRACE, "}"},
    {TOKEN_EOF, ""}
  };
 
  yyin = fopen("samples/Sorting.rs", "r");
  yyrestart(yyin);
  ASSERT_TRUE(yyin);
 
  int index = 0;
  token_t t;
  do {
    t = yylex();
    ASSERT_EQ(tokens[index].t, t);
    ASSERT_STREQ(tokens[index].p, yytext);
    ++index;
  } while (t != TOKEN_EOF);
}