/*
 * Author: John Hossler
 */
#include <stdio.h>
#include "lexer.h"
#include "types.h"
#include "env.h"

void displayEnv(Lexeme *);

int main(int argc, char **argv) {

  printf("Creating a new environment\n");
  Lexeme *global = create();
  printf("The envronment is:\n");
  displayEnv(global);
  Lexeme *x = lexeme(ID);
  x->string = "x";
  Lexeme *val = lexeme(INTEGER);
  val->integer = 3;

  printf("Inserting variable x with value 3 into environment\n");
  insert(x,val,global);

  printf("The environment is:\n");
  displayEnv(global);

  printf("Extending the environment with y:4 and z:\"hello\"\n");
  Lexeme *y = lexeme(ID);
  Lexeme *z = lexeme(ID);
  y->string = "y";
  z->string = "z";
  Lexeme *yval = lexeme(INTEGER);
  yval->integer = 4;
  Lexeme *zval = lexeme(STRING);
  zval->string = "hello";

  Lexeme *local = create();
  insert(y,yval,local);
  insert(z,zval,local);
  Lexeme *table = car(local);

  local = extend(car(table),cdr(table),global);
  printf("The local environment is:\n");
  displayEnv(local);
  printf("The environment is:\n");
  displayEnv(global);

  return 0;
}

void displayEnv(Lexeme *env) {
  Lexeme *table = car(env);
  Lexeme *vars = car(table);
  Lexeme *vals = cdr(table);
  printf("Table:\n");
  if(cdr(env) != NULL) {
    printf("encapsulating environment: %s\n",displayLexeme(*(cdr(env))));
  }
  while(vars != NULL) {
    printf("Var: %s\tVal: %s\n",displayLexeme(*(car(vars))),displayLexeme(*(car(vals))));
    vars = cdr(vars);
    vals = cdr(vals);
  }
}
