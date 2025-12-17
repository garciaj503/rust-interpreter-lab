%{
#include "token.h"
%}

%option nounput
%option noinput
%option noyywrap
%option yylineno

DIGIT [0-9]
LETTER [a-zA-Z]
IDENT [a-z]+[0-9]*
STRING  \"(\\.|[^"\\])*\"

%%

(" "|\t)     /* skip whitespace */
"//".*          { /* skip comments */ }
"fn main()"      /* skip main funcion */
"+"             { return TOKEN_ADD; }
"-"             { return TOKEN_SUBTRACT; }
"*"             { return TOKEN_MULTIPLY; }
"/"             { return TOKEN_DIVIDE; }
"="             { return TOKEN_ASSIGNMENT; }
">"             { return TOKEN_GREATER_THAN; }
"<"             { return TOKEN_LESS_THAN; }
"("             { return TOKEN_LEFT_PARENTHESIS; }
")"             { return TOKEN_RIGHT_PARENTHESIS; }
"{"             { return TOKEN_LEFT_CURLY_BRACE; }
"}"             { return TOKEN_RIGHT_CURLY_BRACE; }
"["             { return TOKEN_LEFT_BRACKET; }
"]"             { return TOKEN_RIGHT_BRACKET; }
"while"         { return TOKEN_KEYWORD_WHILE; }
"if"            { return TOKEN_KEYWORD_IF; }
"let"           { return TOKEN_KEYWORD_LET; }
"mut"           { return TOKEN_KEYWORD_MUT; }
"for"           { return TOKEN_KEYWORD_FOR; }
"in"            { return TOKEN_KEYWORD_IN; }
"println!"      { return TOKEN_KEYWORD_PRINTLN_NOT; }
"i32"           { return TOKEN_INTEGER_IDENT; }
"isqrt()"       { return TOKEN_FUNCTION_ISQRT; }
"len()"         { return TOKEN_FUNCTION_LEN; }
"\n"            { /* ignore newlines */ }
";"             { return TOKEN_SEMICOLON; }
":"             { return TOKEN_COLON; }
","             { return TOKEN_COMMA; }
".."            { return TOKEN_RANGE; }
"."             { return TOKEN_DOT; }
{STRING}        { return TOKEN_STRING; }
{IDENT}         { return TOKEN_IDENT; }
{DIGIT}+        { return TOKEN_NUMBER; }
.               { return TOKEN_ERROR; }
""              { return TOKEN_EOF; }

%%    

/*
flex calls yywrap() whenever it reaches the end of the current file.
If yywrap returns false to indicate the end of the program.
It could alternatively open up another file and return true,
so that flex would keep going.

-- "\n" { return TOKEN_NEWLINE; } we won't use this for now
-- Changed the second rule to not escape

*/
