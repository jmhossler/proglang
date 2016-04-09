/*
 * Author: John Hossler
 */
#ifndef PARSER_CS403_JH_2016
#define PARSER_CS403_JH_2016
#include "lexer.h"

typedef struct parser {
  FILE *fp;
  struct lexeme *previous;
  struct lexeme *pending;
  FILE *out;
  int line;
} Parser;

struct lexeme *parse(FILE *fp);

void displayTree(struct lexeme *,char *);
int typeEqual(struct lexeme *,struct lexeme *);

#endif
