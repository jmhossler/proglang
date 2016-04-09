#include <stdio.h>
#include "types.h"
#include "lexer.h"
#include "env.h"
#include "parser.h"

int sameVariable(Lexeme *,Lexeme *);

Lexeme *lookup(Lexeme *variable, Lexeme *env) {
  //printf("Environment:\n");
  //printf("Looking for %s\n",displayLexeme(*variable));
  if(variable->type != ID) {
    return lexeme(NIL);
  }
  while(env != NULL) {
    //displayEnv(env);
    //printf("Looking in environment\n");
    Lexeme *table = car(env);
    Lexeme *vars = car(table);
    Lexeme *vals = cdr(table);
    //printf("Table: %s\n",displayLexeme(*table));
    //printf("Vars: %s\n",displayLexeme(*vars));
    //printf("Vals: %s\n",displayLexeme(*vals));
    while(vars != NULL && vals != NULL && strcmp(vars->type,NIL) && strcmp(vals->type,NIL)) {
      //printf("Searching vars and vals\n");
      if(sameVariable(variable,car(vars))) {
        //printf("Found var %s as %s\n",displayLexeme(*variable),displayLexeme(*(car(vals))));
        return car(vals);
      }
      vars = cdr(vars);
      vals = cdr(vals);
    }
    //printf("Searching for a thing\n");
    env = cdr(env);
  }

  fprintf(stderr,"Variable %s is undefined\n",displayLexeme(*variable));

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

  return NULL;
}

Lexeme *insert(Lexeme *variable, Lexeme *value, Lexeme *env) {
  Lexeme *table = car(env);
  //printf("Inserting %s into table\n",displayLexeme(*variable));
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
