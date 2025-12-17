%{
#define YYDEBUG 1
#include <stdio.h>
extern int yylineno;
void yyerror(const char*);
int yylex();
%}

%debug
%define parse.error detailed

%token TOKEN_ADD TOKEN_SUBTRACT TOKEN_MULTIPLY TOKEN_DIVIDE
%token TOKEN_ASSIGNMENT
%token TOKEN_GREATER_THAN TOKEN_LESS_THAN
%token TOKEN_LEFT_PARENTHESIS TOKEN_RIGHT_PARENTHESIS
%token TOKEN_LEFT_CURLY_BRACE TOKEN_RIGHT_CURLY_BRACE
%token TOKEN_LEFT_BRACKET TOKEN_RIGHT_BRACKET
%token TOKEN_KEYWORD_WHILE TOKEN_KEYWORD_IF
%token TOKEN_KEYWORD_LET TOKEN_KEYWORD_MUT
%token TOKEN_KEYWORD_FOR TOKEN_KEYWORD_IN
%token TOKEN_KEYWORD_PRINTLN_NOT
%token TOKEN_INTEGER_IDENT
%token TOKEN_FUNCTION_ISQRT TOKEN_FUNCTION_LEN
%token TOKEN_SEMICOLON TOKEN_COLON TOKEN_COMMA TOKEN_DOT
%token TOKEN_STRING
%token TOKEN_IDENT
%token TOKEN_NUMBER
%token TOKEN_EOF
%token TOKEN_ERROR
%token TOKEN_RANGE

%left TOKEN_LESS_THAN TOKEN_GREATER_THAN
%left TOKEN_ADD TOKEN_SUBTRACT
%left TOKEN_MULTIPLY TOKEN_DIVIDE
%left TOKEN_DOT
%right UMINUS  /* unary minus */

%%

/* Here is the grammar: program is the start symbol. */

program
  : stmt_list
  ;

stmt_list
  : /* empty */
  | stmt_list stmt
  ;

stmt
  : decl_stmt
  | assign_stmt
  | print_stmt
  | if_stmt
  | while_stmt
  | for_stmt
  | block
  | expr TOKEN_SEMICOLON
  ;

decl_stmt
  : TOKEN_KEYWORD_LET opt_mut TOKEN_IDENT opt_type opt_init TOKEN_SEMICOLON
  ;
opt_mut
  : /* empty */
  | TOKEN_KEYWORD_MUT
  ;
opt_type
  : /* empty */
  | TOKEN_COLON TOKEN_INTEGER_IDENT             
  ;
opt_init
  : /* empty */
  | TOKEN_ASSIGNMENT expr
  ;

assign_stmt
  : TOKEN_IDENT TOKEN_ASSIGNMENT expr TOKEN_SEMICOLON
  | TOKEN_IDENT TOKEN_LEFT_BRACKET expr TOKEN_RIGHT_BRACKET TOKEN_ASSIGNMENT expr TOKEN_SEMICOLON
  ;

print_args
  : /* empty */
  | TOKEN_STRING
  | TOKEN_STRING TOKEN_COMMA expr_list
  | expr_list
  ;
expr_list
  : expr
  | expr_list TOKEN_COMMA expr
  ;

print_stmt
  : TOKEN_KEYWORD_PRINTLN_NOT TOKEN_LEFT_PARENTHESIS print_args TOKEN_RIGHT_PARENTHESIS TOKEN_SEMICOLON
  ;

if_stmt
  : TOKEN_KEYWORD_IF expr block
  ;

while_stmt
  : TOKEN_KEYWORD_WHILE expr block
  ;

for_stmt
  : TOKEN_KEYWORD_FOR TOKEN_IDENT TOKEN_KEYWORD_IN range_expr block
  ;

range_expr
  : expr TOKEN_RANGE expr
  ;

array_literal
  : TOKEN_LEFT_BRACKET expr_list TOKEN_RIGHT_BRACKET
  ;

/* { stmt* } */
block
  : TOKEN_LEFT_CURLY_BRACE stmt_list TOKEN_RIGHT_CURLY_BRACE
  ;

/* Expressions */
expr
  : expr TOKEN_ADD expr
  | expr TOKEN_SUBTRACT expr
  | expr TOKEN_MULTIPLY expr
  | expr TOKEN_DIVIDE expr
  | expr TOKEN_LESS_THAN expr
  | expr TOKEN_GREATER_THAN expr
  | TOKEN_SUBTRACT expr %prec UMINUS
  | TOKEN_LEFT_PARENTHESIS expr TOKEN_RIGHT_PARENTHESIS
  | primary
  ;

postfix
  : TOKEN_IDENT
  | TOKEN_IDENT TOKEN_LEFT_BRACKET expr TOKEN_RIGHT_BRACKET
  | TOKEN_IDENT TOKEN_DOT TOKEN_FUNCTION_ISQRT
  | TOKEN_IDENT TOKEN_DOT TOKEN_FUNCTION_LEN      
  | func_call
  ;

primary
  : TOKEN_NUMBER
  | postfix
  | range_expr
  | array_literal
  ;

/* Support either scanner style:
   - TOKEN_FUNCTION_LEN as a single token "len()" with no args
   - TOKEN_KEYWORD_LEN '(' expr ')'
   - TOKEN_IDENT '(' expr? ')' for isqrt/len modeled as identifiers
   - TOKEN_FUNCTION_ISQRT similarly (with 0 or 1 arg depending on scanner) */
func_call
  : TOKEN_FUNCTION_LEN
  | TOKEN_FUNCTION_ISQRT
  | TOKEN_IDENT TOKEN_LEFT_PARENTHESIS opt_args TOKEN_RIGHT_PARENTHESIS
  ;

opt_args
  : /* empty */
  | expr_list
  ;

%%

/*
This function will be called by bison if the parse should
encounter an error.  In principle, "str" will contain something
useful.  In practice, it often does not.
*/

void yyerror(const char* msg) {
    fprintf(stderr, "Error | Line: %d\n%s\n", yylineno, msg);
}
