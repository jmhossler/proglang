/*
 * Author: John Hossler
 */
#include <stdio.h>
#include "../types/types.h"
#include "../lexer/lexer.h"
#include "../environment/env.h"
#include "../parser/parser.h"

int sameVariable(Lexeme *,Lexeme *);

Lexeme *lookup(Lexeme *variable, Lexeme *env) {
  if(variable->type != ID) {
    return lexeme(NIL);
  }
  while(env != NULL) {
    Lexeme *table = car(env);
    Lexeme *vars = car(table);
    Lexeme *vals = cdr(table);
    while(vars != NULL && vals != NULL && strcmp(vars->type,NIL) && strcmp(vals->type,NIL)) {
      if(sameVariable(variable,car(vars))) {
        return car(vals);
      }
      vars = cdr(vars);
      vals = cdr(vals);
    }
    env = cdr(env);
  }

  fprintf(stderr,"Variable %s is undefined\n",displayLexeme(*variable));
  exit(1);

  return lexeme(NIL);
}

Lexeme *update(Lexeme *variable, Lexeme *value, Lexeme *env) {
  while(env != NULL) {
    Lexeme *table = car(env);
    Lexeme *vars = car(table);
    Lexeme *vals = cdr(table);
    while(strcmp(vars->type,NIL)) {
      if(sameVariable(variable,car(vars))) {
        setCar(vals,value);
        return value;
      }
      vars = cdr(vars);
      vals = cdr(vals);
    }
    env = cdr(env);
  }

  fprintf(stderr,"Variable %s is undefined\n",displayLexeme(*variable));
  exit(1);

  return NULL;
}

Lexeme *insert(Lexeme *variable, Lexeme *value, Lexeme *env) {
  Lexeme *table = car(env);
  Lexeme *vars = car(table);
  Lexeme *vals = cdr(table);
  table->left = cons(JOIN,variable,vars);
  if(value != NULL) {
    table->right = cons(JOIN,value,vals);
  } else {
    table->right = cons(JOIN,lexeme(NIL),vals);
  }
  return value;
}

Lexeme *extend(Lexeme *variables, Lexeme *values, Lexeme *env) {
  Lexeme *new = cons(ENV,cons(VALUES,variables,values),env);
  Lexeme *this = lexeme(ID);
  Lexeme *__context = lexeme(ID);
  __context->sval = "__context";
  this->sval = "this";
  insert(this,new,new);
  insert(__context,env,new);

  return new;
}

Lexeme *create() {
  return extend(lexeme(NIL),lexeme(NIL),NULL);
}

int sameVariable(Lexeme *var1, Lexeme *var2) {
  return !strcmp(var1->sval,var2->sval);
}

void displayEnv(Lexeme *env) {
  Lexeme *table = car(env);
  Lexeme *vars = car(table);
  Lexeme *vals = cdr(table);
  printf("Table:\n");
  while(vars != NULL && strcmp(vars->type,NIL)) {
    printf("Var: %s\tVal: %s\n",displayLexeme(*(car(vars))),displayLexeme(*(car(vals))));
    vars = cdr(vars);
    vals = cdr(vals);
  }
}
