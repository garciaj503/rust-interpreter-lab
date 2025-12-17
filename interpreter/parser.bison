%{
#define YYDEBUG 1
#include <stdio.h>
#include <stdlib.h>
#include "expr.h"
#include <string.h>
extern int yylineno;
extern char *yytext;
extern int yylex();
extern void yyerror(const char*);
struct stmt * parser_result = 0;
%}

%debug

%union {
  struct stmt *stmt;
  struct expr *expr;
  struct decl *decl;
  const char *string;
  struct type *type;
};

%type <expr> expr primary postfix expr_list opt_init range_expr array_literal print_args opt_args func_call TOKEN_FUNCTION_LEN TOKEN_FUNCTION_ISQRT
%type <stmt> stmt stmt_list block if_stmt while_stmt for_stmt print_stmt assign_stmt
%type <decl> decl_stmt
%type <type> opt_mut opt_type

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
%token <string> TOKEN_STRING
%token <string> TOKEN_IDENT
%token <string> TOKEN_NUMBER
%token TOKEN_EOF
%token TOKEN_ERROR
%token TOKEN_RANGE

%left TOKEN_RANGE
%left TOKEN_LESS_THAN TOKEN_GREATER_THAN
%left TOKEN_ADD TOKEN_SUBTRACT
%left TOKEN_MULTIPLY TOKEN_DIVIDE
%left TOKEN_DOT
%right UMINUS  /* unary minus */

%%

/* Here is the grammar: program is the start symbol. */

program
  : stmt_list             { parser_result = $1; }
  ;

stmt_list
  : /* empty */           { $$ = 0; }
  | stmt_list stmt {
      if ($1) {
        struct stmt *s = $1;
        while(s->next) s = s->next;
        s->next = $2;
        $$ = $1;
      } else {
        $$ = $2;
      }
    }
  ;

stmt
  : decl_stmt                   { $$ = stmt_create(STMT_DECL, $1, 0, 0, 0, 0, 0, 0); }
  | assign_stmt                 { $$ = $1; }
  | print_stmt                  { $$ = $1; }
  | if_stmt                     { $$ = $1; }
  | while_stmt                  { $$ = $1; }
  | for_stmt                    { $$ = $1; }
  | block                       { $$ = $1; }
  | expr TOKEN_SEMICOLON        { $$ = stmt_create(STMT_EXPR, 0, 0, $1, 0, 0, 0, 0); }
  ;

decl_stmt
  : TOKEN_KEYWORD_LET opt_mut TOKEN_IDENT opt_type opt_init TOKEN_SEMICOLON { 
      $$ = decl_create(expr_create_name($3), $2 ? $2 : $4, $5, 0, 0);
  }
  ;

opt_mut
  : /* empty */       { $$ = 0; }
  | TOKEN_KEYWORD_MUT { $$ = type_create(TYPE_MUT, 0); }
  ;

opt_type
  : /* empty */                         { $$ = 0; }
  | TOKEN_COLON TOKEN_INTEGER_IDENT     { $$ = type_create(TYPE_INTEGER, 0); }
  ;

opt_init
  : /* empty */                         { $$ = 0; }
  | TOKEN_ASSIGNMENT expr               { $$ = $2; }
  ;

assign_stmt
  : TOKEN_IDENT TOKEN_ASSIGNMENT expr TOKEN_SEMICOLON {
      struct expr *variable = expr_create(EXPR_ASSIGN, expr_create_name($1), $3) ;
      $$ = stmt_create(STMT_ASSIGN, 0, 0, variable, 0, 0, 0, 0);
    }
  | TOKEN_IDENT TOKEN_LEFT_BRACKET expr TOKEN_RIGHT_BRACKET TOKEN_ASSIGNMENT expr TOKEN_SEMICOLON {
      struct expr *subscript = expr_create(EXPR_SUBSCRIPT, expr_create_name($1), $3);
      struct expr *assign_expr = expr_create(EXPR_ASSIGN, subscript, $6);
      $$ = stmt_create(STMT_ASSIGN, 0, 0, assign_expr, 0, 0, 0, 0);
    }   
  ;

print_args
  : /* empty */                                 { $$ = 0; }
  | expr_list                                   { $$ = $1; }
  ;

expr_list
  : expr                              { $$ = $1; }
  | expr_list TOKEN_COMMA expr        { $$ = expr_create(EXPR_ARG_LIST, $1, $3); }
  ;

print_stmt
  : TOKEN_KEYWORD_PRINTLN_NOT TOKEN_LEFT_PARENTHESIS print_args TOKEN_RIGHT_PARENTHESIS TOKEN_SEMICOLON    { $$ =   stmt_create(STMT_PRINT, 0, 0, $3, 0, 0, 0, 0); }
  ;

if_stmt
  : TOKEN_KEYWORD_IF expr block 	{ $$ = stmt_create(STMT_IF, 0, 0, $2, 0, $3, 0, 0); }
  ;

while_stmt
  : TOKEN_KEYWORD_WHILE expr block {
      $$ = stmt_create(STMT_WHILE, 0, 0, $2, 0, $3, 0, 0);
      if ($3) $3->parent_kind = STMT_WHILE;
  }
  ;

for_stmt
  : TOKEN_KEYWORD_FOR TOKEN_IDENT TOKEN_KEYWORD_IN range_expr block {
      struct decl *iter = decl_create(expr_create_name($2), 0, 0, 0, 0);
      $$ = stmt_create(STMT_FOR, iter, 0, $4, 0, $5, 0, 0);
      if ($5) $5->parent_kind = STMT_FOR;
  }
  ;

range_expr
  : expr TOKEN_RANGE expr    { $$ = expr_create(EXPR_RANGE, $1, $3); }
  ;

array_literal
  : TOKEN_LEFT_BRACKET expr_list TOKEN_RIGHT_BRACKET    { $$ = expr_create(EXPR_ARRAY, $2, 0); }
  ;

/* { stmt* } */
block
  : TOKEN_LEFT_CURLY_BRACE stmt_list TOKEN_RIGHT_CURLY_BRACE    { $$ = $2; }
  ;

/* Expressions */
expr
  : expr TOKEN_ADD expr                                   { $$ = expr_create(EXPR_ADD, $1, $3); }
  | expr TOKEN_SUBTRACT expr                              { $$ = expr_create(EXPR_SUBTRACT, $1, $3); }
  | expr TOKEN_MULTIPLY expr                              { $$ = expr_create(EXPR_MULTIPLY, $1, $3); }
  | expr TOKEN_DIVIDE expr                                { $$ = expr_create(EXPR_DIVIDE, $1, $3); }
  | expr TOKEN_LESS_THAN expr                             { $$ = expr_create(EXPR_LESS_THAN, $1, $3); }
  | expr TOKEN_GREATER_THAN expr                          { $$ = expr_create(EXPR_GREATER_THAN, $1, $3); }
  | TOKEN_SUBTRACT expr %prec UMINUS                      { $$ = expr_create(EXPR_SUBTRACT, expr_create_integer_literal(0), $2); }
  | TOKEN_LEFT_PARENTHESIS expr TOKEN_RIGHT_PARENTHESIS   { $$ = $2; }
  | primary                                               { $$ = $1; }
  ;

postfix
  : TOKEN_IDENT                                               { $$ = expr_create_name($1); }
  | TOKEN_IDENT TOKEN_LEFT_BRACKET expr TOKEN_RIGHT_BRACKET   { $$ = expr_create(EXPR_SUBSCRIPT, expr_create_name($1), $3); }
  | TOKEN_IDENT TOKEN_DOT TOKEN_FUNCTION_ISQRT                { $$ = expr_create(EXPR_ISQRT, expr_create_name($1), 0); }
  | TOKEN_IDENT TOKEN_DOT TOKEN_FUNCTION_LEN                  { $$ = expr_create(EXPR_LEN, expr_create_name($1), 0); }
  | func_call                                                 { $$ = $1; }
  ;

primary
  : TOKEN_NUMBER {
      if (strchr($1, '.')) {
        $$ = expr_create_float_literal(atof($1));
      } else {
        $$ = expr_create_integer_literal(atoi($1));
      }
    }
  | TOKEN_STRING { $$ = expr_create_string_literal($1); }
  | postfix { $$ = $1; }
  | range_expr { $$ = $1; }
  | array_literal { $$ = $1; }
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
  : /* empty */  { $$ = 0; }
  | expr_list    { $$ = $1; }         
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
