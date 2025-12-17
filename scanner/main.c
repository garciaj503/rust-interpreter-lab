#include "token.h"
#include <stdio.h>
#include <stdlib.h>

extern FILE *yyin;
extern int yylex();
extern char *yytext;

int main(int argc, char *argv[]) {
  const char *filename = "samples/hello.py"; // Default filename

  // Check if a filename is provided as a command-line argument
  if (argc > 1) {
    filename = argv[1];
  }

  // Open the file
  yyin = fopen(filename, "r");
  if (!yyin) {
    printf("could not open file!\n");
    return 1;
  }
  while (1) {
    token_t t = yylex();
    if (t == TOKEN_EOF)
      break;
    printf("token: %d text: %s\n", t, yytext);
  }
}
