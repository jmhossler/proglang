#include <stdio.h>
#include <stdlib.h>
#include "eval.h"
#include "lexer.h"
#include "parser.h"
#include "env.h"

int main(int argc, char **argv) {
  if(argc != 2) {
    fprintf(stderr,"Usage:\ttestEval <filename>\n");
    exit(1);
  }
  FILE *fp = fopen(argv[1],"r");
  printf("Parsing program\n");
  Lexeme *tree = parse(fp);
  printf("Parsing complete\n");

  Lexeme *global = create();
  Lexeme *result = eval(tree,global);
  displayEnv(global);

  printf("Result: %s\n",displayLexeme(*result));

  return 0;
}
