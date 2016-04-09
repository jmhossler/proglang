#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "lexer.h"

int main(int argc, char **argv) {
  if(argc != 2) {
    fprintf(stderr,"Usage:\trecognizer <filename>\n");
    exit(1);
  }
  FILE *fp = fopen(argv[1],"r");

  printf("Beginning parsing\n");
  Lexeme *tree = parse(fp);
  printf("Parsing completed!\n");

  displayTree(tree,"");
  return 0;
}
