#include <stdio.h>
#include <stdlib.h>
#include "evaluation/eval.h"
#include "lexer/lexer.h"
#include "types/types.h"
#include "parser/parser.h"
#include "environment/env.h"

Lexeme *createBuiltin();

int main(int argc, char **argv) {
  if(argc != 2) {
    fprintf(stderr,"Usage:\tcompile <filename>\n");
    exit(1);
  }
  FILE *fp = fopen(argv[1],"r");
  FILE *stdlib = fopen("stdlib/stdlib.sux","r");
  printf("Parsing program\n");
  Lexeme *tree = parse(fp);
  printf("Parsing complete\n");
  //displayTree(tree,"");

  Lexeme *outer = createBuiltin();
  eval(parse(stdlib),outer);
  Lexeme *global = extend(NULL,NULL,outer);
  Lexeme *result = eval(tree,global);
  //displayEnv(global);

  printf("Result: %s\n",displayLexeme(*result));

  return 0;
}

Lexeme *createBuiltin() {
  Lexeme *print = lexeme(ID);
  print->sval = "print";
  Lexeme *defPrint = lexeme(BUILTIN);
  defPrint->sval = "print";
  defPrint->builtin = evalPrint;
  Lexeme *include = lexeme(ID);
  include->sval = "include";
  Lexeme *defInclude = lexeme(BUILTIN);
  defInclude->sval = "include";
  defInclude->builtin = evalInclude;
  Lexeme *cons = lexeme(ID);
  cons->sval = "cons";
  Lexeme *defCons = lexeme(BUILTIN);
  defCons->builtin = evalCons;
  Lexeme *cdr = lexeme(ID);
  cdr->sval = "cdr";
  Lexeme *defCdr = lexeme(BUILTIN);
  defCdr->builtin = evalCdr;
  Lexeme *car = lexeme(ID);
  car->sval = "car";
  Lexeme *defCar = lexeme(BUILTIN);
  defCar->builtin = evalCar;
  Lexeme *env = create();
  insert(print,defPrint,env);
  insert(include,defInclude,env);
  insert(cons,defCons,env);
  insert(cdr,defCdr,env);
  insert(car,defCar,env);
  return env;
}
