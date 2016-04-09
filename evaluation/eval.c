#include <stdlib.h>
#include <math.h>
#include "eval.h"
#include "../lexer/lexer.h"
#include "../parser/parser.h"
#include "../environment/env.h"
#include "../types/types.h"

/*-------- Declarations --------*/

Lexeme *evalStatementList(Lexeme *, Lexeme *);
Lexeme *evalFuncDef(Lexeme *, Lexeme *);
Lexeme *evalFuncCall(Lexeme *, Lexeme *);
Lexeme *evalBlock(Lexeme *, Lexeme *);
Lexeme *evalPlus(Lexeme *, Lexeme *);
Lexeme *evalAssign(Lexeme *, Lexeme *);
Lexeme *evalIf(Lexeme *, Lexeme *);
Lexeme *evalElse(Lexeme *, Lexeme *);
Lexeme *evalWhile(Lexeme *, Lexeme *);
Lexeme *evalIncrement(Lexeme *, Lexeme *);
Lexeme *evalTimes(Lexeme *, Lexeme *);
Lexeme *evalMinus(Lexeme *, Lexeme *);
Lexeme *evalDecrement(Lexeme *, Lexeme *);
Lexeme *evalDivide(Lexeme *, Lexeme *);
Lexeme *evalExp(Lexeme *, Lexeme *);
Lexeme *evalMod(Lexeme *, Lexeme *);
Lexeme *evalLess_than(Lexeme *, Lexeme *);
Lexeme *evalLT_EQ(Lexeme *, Lexeme *);
Lexeme *evalGreater_than(Lexeme *, Lexeme *);
Lexeme *evalGT_EQ(Lexeme *, Lexeme *);
Lexeme *evalEqual(Lexeme *, Lexeme *);
Lexeme *evalNot_equal(Lexeme *, Lexeme *);
Lexeme *evalStrict_equal(Lexeme *, Lexeme *);
Lexeme *evalStrict_nequal(Lexeme *, Lexeme *);
Lexeme *evalThunk(Lexeme *, Lexeme *);
Lexeme *evalOr(Lexeme *, Lexeme *);
Lexeme *evalAnd(Lexeme *, Lexeme *);
Lexeme *evalNot(Lexeme *, Lexeme *);
Lexeme *evalXor(Lexeme *, Lexeme *);
Lexeme *evalArray(Lexeme *, Lexeme *);
Lexeme *evalArrayCall(Lexeme *, Lexeme *);
Lexeme *evalVar(Lexeme *, Lexeme *);
Lexeme *evalId(Lexeme *, Lexeme *);
Lexeme *evalInt(Lexeme *, Lexeme *);
Lexeme *evalString(Lexeme *, Lexeme *);
Lexeme *evalBuiltin(Lexeme *, Lexeme *);
Lexeme *evalFuncArgs(Lexeme *, Lexeme *, Lexeme *);
Lexeme *evalDot(Lexeme *, Lexeme *);
Lexeme *evalNil(Lexeme *, Lexeme *);
Lexeme *evalExprList(Lexeme *, Lexeme *);
Lexeme *evalJoin(Lexeme *, Lexeme *);

/* Closure helper functions */
Lexeme *getParams(Lexeme *);
Lexeme *getDefScope(Lexeme *);
Lexeme *getFuncArgs(Lexeme *);
Lexeme *getClosure(Lexeme *, Lexeme *);

/* Helper functions */
int areEqual(Lexeme *,Lexeme *, char *);
Lexeme *thunk(Lexeme *, Lexeme *);
Lexeme **resizeLexArr(Lexeme **, size_t *size);

typedef Lexeme *(*evalFunction)(Lexeme *, Lexeme *);
evalFunction getEvalFunction(char *);

//Lexeme *getFuncCallName(Lexeme *);

/*------- DEFINITIONS -------*/

int areEqual(Lexeme *a, Lexeme *b, char *type) {
  return strcmp(a->type,type) == 0 && strcmp(b->type,type) == 0;
}

Lexeme *eval(Lexeme *tree, Lexeme *env) {
  if(tree == NULL) {
    return lexeme(NIL);
  }
  //printf("Eval %s\n",displayLexeme(*tree));
  //displayTree(tree,"");
  //displayEnv(env);
  evalFunction f = getEvalFunction(tree->type);
  if(f == NULL) {
    fprintf(stderr,"No evaluation function for type %s\n",tree->type);
    return lexeme(NIL);
  } else {
    return f(tree,env);
  }
}

Lexeme *evalStatementList(Lexeme *tree, Lexeme *env) {
  Lexeme *result = eval(car(tree),env);
  if(cdr(tree) != NULL) {
    result = eval(cdr(tree),env);
  }
  return result;
}

Lexeme *evalJoin(Lexeme *tree, Lexeme *env) {
  return tree;
}

Lexeme *evalInclude(Lexeme *tree, Lexeme *env) {
  Lexeme *eargs = evalExprList(tree,env);
  //displayTree(eargs,"");
  Lexeme *val = NULL;
  Lexeme *result = NULL;
  while(eargs != NULL) {
    val = car(eargs);
    if(strcmp(val->type,STRING) == 0) {
      FILE *fp = fopen(val->sval,"r");
      result = eval(parse(fp),env);
    } else {
      fprintf(stderr,"Cannot open %s\n",displayLexeme(*val));
    }
    eargs = cdr(eargs);
  }
  return result;
}

Lexeme *evalPrint(Lexeme *tree, Lexeme *env) {
  //printf("Starting print\n");
  Lexeme *eargs = evalExprList(tree,env);
  Lexeme *val = NULL;
  Lexeme *result = lexeme(STRING);
  result->sval = "print-done";
  result->ival = 10;
  //displayTree(eargs,"");
  while(eargs != NULL) {
    val = car(eargs);
    if(strcmp(val->type,INTEGER) == 0) {
      printf("%d",val->ival);
    } else if(strcmp(val->type,STRING) == 0) {
      printf("%s",val->sval);
    } else {
      fprintf(stderr,"UNKOWN TYPE FOR PRINT %s\n",val->type);
    }
    eargs = cdr(eargs);
  }
  //printf("Ending print\n");
  return result;
}

int eargsLength(Lexeme *tree) {
  if(tree == NULL) {
    return 0;
  } else {
    return 1 + eargsLength(cdr(tree));
  }
}

Lexeme *evalCons(Lexeme *tree, Lexeme *env) {
  Lexeme *eargs = evalExprList(tree,env);
  if(eargsLength(eargs) == 2) {
    return cons(JOIN,car(eargs),car(cdr(eargs)));
  } else {
    fprintf(stderr,"Incorrect number of arguments to Cons\n");
    return lexeme(NIL);
  }
}

Lexeme *evalCar(Lexeme *tree, Lexeme *env) {
  Lexeme *eargs = evalExprList(tree,env);
  if(eargsLength(eargs) == 1) {
    return car(eargs);
  } else {
    fprintf(stderr,"Incorrect number of arguments to Car\n");
    return lexeme(NIL);
  }
}

Lexeme *evalCdr(Lexeme *tree, Lexeme *env) {
  Lexeme *eargs = evalExprList(tree,env);
  if(eargsLength(eargs) == 1) {
    return cdr(eargs);
  } else {
    fprintf(stderr,"Incorrect number of arguments to Cdr\n");
    return lexeme(NIL);
  }
}

Lexeme *evalArr(Lexeme *tree, Lexeme *env) {
  Lexeme *eargs = evalExprList(tree,env);
  Lexeme *arr = lexeme(ARRAY);
  if(eargsLength(eargs) == 1) {
    if(typeEqual(car(eargs),lexeme(INTEGER))) {
      if(car(eargs)->ival >= 0) {
        arr->aval = malloc(sizeof(Lexeme *) * car(eargs)->ival);
        arr->ival = car(eargs)->ival;
      } else {
        fprintf(stderr,"Size must be greather than or equal to 0\n");
      }
    } else {
      fprintf(stderr,"Must initialize array with an integer\n");
    }
  } else {
    fprintf(stderr,"Must initialize array with an integer >= 0\n");
  }
  return arr;
}

//TODO put in .h, blah blah blah
Lexeme *evalAppend(Lexeme *tree, Lexeme *env) {
  Lexeme *eargs = evalExprList(tree,env);
  if(eargsLength(eargs) != 2) {
    fprintf(stderr,"Error\n");
    return lexeme(NIL);
  }
  Lexeme *arr = car(eargs);
  Lexeme *val = car(cdr(eargs));
  size_t newSize = arr->ival;
  if(typeEqual(arr,lexeme(ARRAY))) {
    arr->aval = resizeLexArr(arr->aval,&newSize);
    arr->aval[arr->ival] = val;
    arr->ival = arr->ival + 1;
  } else {
    fprintf(stderr,"Can only append to arrays\n");
  }
  return val;
}

Lexeme *evalNil(Lexeme *tree, Lexeme *env) {
  return lexeme(NIL);
}

Lexeme *evalFuncDef(Lexeme *tree, Lexeme *env) {
  return cons(CLOSURE,env,tree);
}

Lexeme *getClosure(Lexeme *tree, Lexeme *env) {
  if(strcmp(car(tree)->type,CLOSURE) == 0) {
    return car(tree);
  } else {
    return eval(car(tree),env);
  }
}

Lexeme *getParams(Lexeme *closure) {
  return car(cdr(closure));
}

Lexeme *getDefScope(Lexeme *closure) {
  return car(closure);
}

// TODO put signatures at top
Lexeme *thunk(Lexeme *tree, Lexeme *env) {
  return cons(THUNK,tree,env);
}

Lexeme *evalThunk(Lexeme *tree, Lexeme *env) {
  return eval(car(tree),cdr(tree));
}

Lexeme *evalFuncArgs(Lexeme *tree, Lexeme *params, Lexeme *env) {
  if((tree == NULL && params == NULL) || areEqual(tree,params,NIL)) {
    return lexeme(NIL);
  } else if(tree == NULL || !strcmp(tree->type,NIL)) {
    return lexeme(NIL);
  } else if(params == NULL || !strcmp(params->type,NIL)) {
    return lexeme(NIL);
  } else if(!strcmp(car(params)->sval,"@")) {
    Lexeme *list = evalExprList(tree,env);
    if(list == NULL) {
      return lexeme(NIL);
    } else {
      return cons(JOIN,evalExprList(tree,env),NULL);
    }
  } else if(typeEqual(car(params),lexeme(OPAREN))) {
    return cons(JOIN,thunk(car(tree),env),evalFuncArgs(cdr(tree),cdr(params),env));
  } else {
    return cons(JOIN,eval(car(tree),env),evalFuncArgs(cdr(tree),cdr(params),env));
  }
}

Lexeme *evalExprList(Lexeme *tree, Lexeme *env) {
  if(tree == NULL) {
    //printf("Finished eval expr list\n");
    return NULL;
  } else {
    //printf("Eval expr: %s\n",displayLexeme(*(eval(car(tree),env))));
    return cons(JOIN,eval(car(tree),env),evalExprList(cdr(tree),env));
  }
}

Lexeme *getBody(Lexeme *closure) {
  return cdr(cdr(closure));
}

Lexeme *evalFuncCall(Lexeme *tree, Lexeme *env) {
  //printf("Eval func %s\n",displayLexeme(*tree));
  Lexeme *closure = getClosure(tree,env);
  //printf("Closure: %s\n",displayLexeme(*closure));
  if(strcmp(closure->type,BUILTIN) == 0) {
    //printf("Here1\n");
    //printf("builtin: %s\n",closure->sval);
    return closure->builtin(cdr(tree),env);
  }
  //printf("eval prams\n");

  Lexeme *params = getParams(closure);
  //printf("Eval args\n");
  Lexeme *eargs = evalFuncArgs(cdr(tree),params,env);
  //printf("make scope\n");

  Lexeme *newScope = extend(params,eargs,getDefScope(closure));;
  //printf("New scope for funcCall:\n");
  //displayEnv(newScope);
  //printf("Closure bod:\n");
  //displayTree(getBody(closure),"");
  //printf("Func call to %s fin\n",displayLexeme(*tree));

  return eval(getBody(closure),newScope);
}

Lexeme *evalDot(Lexeme *tree, Lexeme *env) {
  //printf("Type of car: %s\n",displayLexeme(*(car(tree))));
  //printf("Type of cdr: %s\n",displayLexeme(*(cdr(tree))));
  Lexeme *valEnv = eval(car(tree),env);
  //printf("Environemnt lex: %s\n",displayLexeme(*valEnv));
  return eval(cdr(tree),valEnv);
}

Lexeme *evalBlock(Lexeme *tree, Lexeme *env) {
  Lexeme *result = NULL;
  while(tree != NULL) {
    result = eval(tree->left,env);
    tree = tree->right;
  }
  return result;
}

Lexeme **resizeLexArr(Lexeme **arr, size_t *size) {
  Lexeme **new = malloc(sizeof(Lexeme *) * (*size) * 2);
  size_t i;
  for(i = 0; i < *size; ++i) {
    new[i] = arr[i];
  }
  free(arr);
  *size = *size * 2;
  return new;
}

// TODO finish
// array type:
//            Lexeme *array = malloc(sizeof(Lexeme) * size);
Lexeme *evalArray(Lexeme *tree, Lexeme *env) {
  size_t size = 1, filled = 0;
  Lexeme *result = lexeme(ARRAY);
  Lexeme **values = malloc(sizeof(Lexeme *) * size);
  Lexeme *subtree = evalExprList(cdr(tree),env);
  while(subtree != NULL) {
    if(filled >= size) {
      values = resizeLexArr(values,&size);
    }
    values[filled++] = subtree->left;
    subtree = subtree->right;
  }
  result->ival = filled;
  result->aval = values;
  return result;
}

// TODO finish
Lexeme *evalArrayCall(Lexeme *tree, Lexeme *env) {
  Lexeme *expr = eval(cdr(tree),env);
  Lexeme *arr = eval(car(tree),env);
  Lexeme *result = lexeme(NIL);
  if(!strcmp(arr->type,ARRAY)) {
    if(!strcmp(expr->type,INTEGER)) {
      // TODO add test for size
      if(expr->ival < arr->ival) {
        result = arr->aval[expr->ival];
      } else {
        fprintf(stderr,"Index out of range, range %d, index %d\n",arr->ival,expr->ival);
      }
    } else {
      fprintf(stderr,"Cannot index with index type %s\n",displayLexeme(*expr));
      exit(1);
    }
  } else {
    fprintf(stderr,"Cannot index into %s\n",displayLexeme(*arr));
    exit(1);
  }
  return result;
}

Lexeme *evalVar(Lexeme *tree, Lexeme *env) {
  //printf("Uneval: %s\n",displayLexeme(*(cdr(tree))));
  //printf("Eval: %s\n",displayLexeme(*(eval(cdr(tree),env))));
  return insert(car(tree),eval(cdr(tree),env),env);
}

Lexeme *evalId(Lexeme *tree, Lexeme *env) {
  return lookup(tree,env);
}

Lexeme *evalInt(Lexeme *tree, Lexeme *env) {
  return tree;
}

Lexeme *evalString(Lexeme *tree, Lexeme *env) {
  return tree;
}

// TODO
//  for operator evals, add error checking
//  add sval,int;int,sval for +, *
Lexeme *evalPlus(Lexeme *tree, Lexeme *env) {
  Lexeme *left = eval(car(tree),env);
  Lexeme *right = eval(cdr(tree),env);
  Lexeme *result;
  if(areEqual(left,right,INTEGER)) {
    result = lexeme(INTEGER);
    result->ival = left->ival + right->ival;
  } else if(areEqual(left,right,STRING)) {
    result = lexeme(STRING);
    int size = left->ival + right->ival;
    char *new = malloc(sizeof(char) * size);
    int i = 0, j = 0;
    while(i < left->ival) {
      new[i] = left->sval[i];
      i++;
    }
    while(j < right->ival) {
      new[i+j] = right->sval[j];
      j++;
    }
    result->sval = new;
    result->ival = size;
  } else {
    fprintf(stderr,"Incompatible types for plus: %s and %s",displayLexeme(*left),displayLexeme(*right));
    result = NULL;
  }
  return result;
}

Lexeme *evalAssign(Lexeme *tree, Lexeme *env) {
  Lexeme *val = eval(cdr(tree),env);
  if(!strcmp((car(tree))->type,ID)) {
    return update(car(tree),val,env);
  } else if(!strcmp((car(tree))->type,DOT)) {
    Lexeme *nEnv = eval(car(car(tree)),env);
    Lexeme *id = cdr(car(tree));
    return update(id,val,nEnv);
  } else if(!strcmp((car(tree))->type,ARRAYCALL)) {
    Lexeme *ind = eval(cdr(car(tree)),env);
    Lexeme *arr = eval(car(car(tree)),env);
    if(!strcmp(ind->type,INTEGER)) {
      if(ind->ival < arr->ival) {
        arr->aval[ind->ival] = val;
      } else {
        fprintf(stderr,"Index out of range\n");
        return lexeme(NIL);
      }
    } else {
      fprintf(stderr,"Cannot index with a non-integer value\n");
      return lexeme(NIL);
    }
  }
  return val;
}

Lexeme *evalIf(Lexeme *tree, Lexeme *env) {
  //printf("Starting if\n");
  //displayTree(tree,"");
  Lexeme *condition = eval(car(tree),env);
  if(condition->ival) {
    //printf("Condition passed, eval first block\n");
    //displayTree(car(cdr(tree)),"");
    return eval(car(cdr(tree)),env);
  } else {
    //printf("Condition failed, looking for lovers\n");
    if(cdr(cdr(tree)) != NULL) {
      //printf("Else statement exists, or else if who knows\n");
      //displayTree(cdr(cdr(tree)),"");
      return eval(cdr(cdr(tree)),env);
    } else {
      //printf("Nothing exists\n");
      return lexeme(NIL);
    }
  }
}

Lexeme *evalElse(Lexeme *tree, Lexeme *env) {
  return eval(car(tree),env);
}

Lexeme *evalWhile(Lexeme *tree, Lexeme *env) {
  Lexeme *condition = eval(car(tree),env);
  Lexeme *result = lexeme(NIL);
  while(condition->ival) {
    result = eval(cdr(tree),env);
    condition = eval(car(tree),env);
  }
  return result;
}

Lexeme *evalIncrement(Lexeme *tree, Lexeme *env) {
  Lexeme *id = car(tree);
  Lexeme *incr = eval(cdr(tree),env);
  Lexeme *current = eval(id,env);
  Lexeme *new = lexeme(INTEGER);
  if(strcmp(current->type,INTEGER) != 0) {
    fprintf(stderr,"Incompatible type for increment\n");
    return lexeme(NIL);
  }

  new->ival = incr->ival + current->ival;
  return update(id,new,env);
}

Lexeme *evalTimes(Lexeme *tree, Lexeme *env) {
  Lexeme *left = eval(car(tree),env);
  Lexeme *right = eval(cdr(tree),env);
  Lexeme *result = NULL;
  if(areEqual(left,right,INTEGER)) {
    result = lexeme(INTEGER);
    result->ival = left->ival * right->ival;
  } /*else if(!strcmp(left->type,STRING) && !strcmp(right->type,INTEGER)) {
    result = lexeme(STRING);
    result->sval = malloc(sizeof(char) * right->ival * left->ival);
    int i;
    for(i = 0; i < right->ival * left->ival; ++i) {
      result->sval[i] = right->sval[i % right->ival];
    }*/
  //} else if(!strcmp(left->type,INTEGER && !
  return result;
}

Lexeme *evalMinus(Lexeme *tree, Lexeme *env) {
  Lexeme *left = eval(car(tree),env);
  Lexeme *right = eval(cdr(tree),env);
  Lexeme *result = NULL;
  if(!strcmp(left->type,NIL) && !strcmp(right->type,INTEGER)) {
    result = lexeme(INTEGER);
    result->ival = 0 - right->ival;
  } else if(areEqual(left,right,INTEGER)) {
    result = lexeme(INTEGER);
    result->ival = left->ival - right->ival;
  }
  return result;
}

Lexeme *evalDecrement(Lexeme *tree, Lexeme *env) {
  Lexeme *id = car(tree);
  Lexeme *decr = eval(cdr(tree),env);
  Lexeme *current = eval(id,env);
  Lexeme *new = lexeme(INTEGER);
  if(strcmp(current->type,INTEGER) != 0) {
    fprintf(stderr,"Incompatible type for increment\n");
    return lexeme(NIL);
  }

  new->ival = current->ival - decr->ival;
  return update(id,new,env);
}

Lexeme *evalDivide(Lexeme *tree, Lexeme *env) {
  Lexeme *left = eval(car(tree),env);
  Lexeme *right = eval(cdr(tree),env);
  Lexeme *result = NULL;
  if(areEqual(left,right,INTEGER)) {
    if(right->ival == 0) {
      fprintf(stderr,"MATHEXCEPTION: DIVIDE BY ZERO");
      exit(1);
    }
    result = lexeme(INTEGER);
    result->ival = left->ival / right->ival;
  }
  return result;
}

Lexeme *evalExp(Lexeme *tree, Lexeme *env) {
  Lexeme *left = eval(car(tree),env);
  Lexeme *right = eval(cdr(tree),env);
  Lexeme *result = NULL;
  if(areEqual(left,right,INTEGER)) {
    result = lexeme(INTEGER);
    result->ival = (int) pow((double) left->ival,(double) right->ival);
  }
  return result;
}

Lexeme *evalMod(Lexeme *tree, Lexeme *env) {
  Lexeme *left = eval(car(tree),env);
  Lexeme *right = eval(cdr(tree),env);
  Lexeme *result = NULL;
  if(areEqual(left,right,INTEGER)) {
    result = lexeme(INTEGER);
    result->ival = left->ival % right->ival;
  }
  return result;
}

Lexeme *evalLess_than(Lexeme *tree, Lexeme *env) {
  Lexeme *left = eval(car(tree),env);
  Lexeme *right = eval(cdr(tree),env);
  Lexeme *result = lexeme(INTEGER);
  result->ival = 0;
  if(areEqual(left,right,INTEGER)) {
    result->ival = (left->ival < right->ival);
  } else if(areEqual(left,right,STRING)) {
    result->ival = strcmp(left->sval,right->sval) < 0;
  }
  return result;
}

Lexeme *evalLT_EQ(Lexeme *tree, Lexeme *env) {
  Lexeme *left = eval(car(tree),env);
  Lexeme *right = eval(cdr(tree),env);
  Lexeme *result = lexeme(INTEGER);
  result->ival = 0;
  if(areEqual(left,right,INTEGER)) {
    result->ival = left->ival <= right->ival;
  } else if(areEqual(left,right,STRING)) {
    result->ival = strcmp(left->sval,right->sval) <= 0;
  }
  return result;
}

Lexeme *evalGreater_than(Lexeme *tree, Lexeme *env) {
  Lexeme *left = eval(car(tree),env);
  Lexeme *right = eval(cdr(tree),env);
  Lexeme *result = lexeme(INTEGER);
  result->ival = 0;
  if(areEqual(left,right,INTEGER)) {
    result->ival = left->ival > right->ival;
  } else if(areEqual(left,right,STRING)) {
    result->ival = strcmp(left->sval,right->sval) > 0;
  }
  return result;
}

Lexeme *evalGT_EQ(Lexeme *tree, Lexeme *env) {
  Lexeme *left = eval(car(tree),env);
  Lexeme *right = eval(cdr(tree),env);
  Lexeme *result = lexeme(INTEGER);
  result->ival = 0;
  if(areEqual(left,right,INTEGER)) {
    result->ival = left->ival >= right->ival;
  } else if(areEqual(left,right,STRING)) {
    result->ival = strcmp(left->sval,right->sval) >= 0;
  }
  return result;
}

Lexeme *evalEqual(Lexeme *tree, Lexeme *env) {
  //displayTree(tree,"");
  Lexeme *left = eval(car(tree),env);
  //printf("here\n");
  Lexeme *right = eval(cdr(tree),env);
  //printf("Here1\n");
  Lexeme *result = lexeme(INTEGER);
  result->ival = 0;
  if(areEqual(left,right,INTEGER)) {
    result->ival = left->ival == right->ival;
  } else if(areEqual(left,right,STRING)) {
    result->ival = strcmp(left->sval,right->sval) == 0;
  } else if(typeEqual(left,lexeme(NIL)) || typeEqual(right,lexeme(NIL))) {
    if(areEqual(left,right,NIL)) {
      result->ival = 1;
    } else {
      result->ival = 0;
    }
  } else {
    fprintf(stderr,"Incompatible types for comparison\n");
  }
  return result;
}

Lexeme *evalNot_equal(Lexeme *tree, Lexeme *env) {
  Lexeme *left = eval(car(tree),env);
  Lexeme *right = eval(cdr(tree),env);
  Lexeme *result = lexeme(INTEGER);
  result->ival = 0;
  if(areEqual(left,right,INTEGER)) {
    result->ival = left->ival != right->ival;
  } else if(areEqual(left,right,STRING)) {
    result->ival = strcmp(left->sval,right->sval) != 0;
  }
  return result;
}

// TODO free memory?
Lexeme *evalStrict_equal(Lexeme *tree, Lexeme *env) {
  Lexeme *left = eval(car(tree),env);
  Lexeme *right = eval(cdr(tree),env);
  if(!strcmp(left->type,right->type)) {
    Lexeme *op = lexeme(EQUAL);
    op->left = left;
    op->right = right;
    return evalEqual(op,env);
  } else {
    Lexeme *result = lexeme(INTEGER);
    result->ival = 0;
    return result;
  }
}

Lexeme *evalStrict_nequal(Lexeme *tree, Lexeme *env) {
  Lexeme *left = eval(car(tree),env);
  Lexeme *right = eval(cdr(tree),env);
  if(!strcmp(left->type,right->type)) {
    Lexeme *op = lexeme(NOT_EQUAL);
    op->left = left;
    op->right = right;
    return eval(op,env);
  } else {
    Lexeme *result = lexeme(INTEGER);
    result->ival = 1;
    return result;
  }
}

Lexeme *evalOr(Lexeme *tree, Lexeme *env) {
  Lexeme *left = eval(car(tree),env);
  //Lexeme *right = eval(cdr(tree),env);
  Lexeme *result = lexeme(INTEGER);
  result->ival = 0;
  if(!strcmp(left->type,INTEGER)) {
    if(!left->ival) {
      Lexeme *right = eval(cdr(tree),env);
      if(!strcmp(right->type,INTEGER)) {
        result->ival = right->ival;
      }
    } else {
      result->ival = left->ival;
    }
  }
  return result;
}

Lexeme *evalAnd(Lexeme *tree, Lexeme *env) {
  Lexeme *left = eval(car(tree),env);
  Lexeme *result = lexeme(INTEGER);
  result->ival = 0;
  if(!strcmp(left->type,INTEGER)) {
    if(left->ival) {
      Lexeme *right = eval(cdr(tree),env);
      if(!strcmp(right->type,INTEGER)) {
        result->ival = right->ival;
      }
    }
  }
  return result;
}

Lexeme *evalNot(Lexeme *tree, Lexeme *env) {
  Lexeme *right = eval(cdr(tree),env);
  Lexeme *result = lexeme(INTEGER);
  result->ival = 0;
  if(!strcmp(right->type,INTEGER)) {
    if(right->ival) {
      result->ival = 0;
    } else {
      result->ival = 1;
    }
  }
  return result;
}

Lexeme *evalXor(Lexeme *tree, Lexeme *env) {
  Lexeme *left = eval(car(tree),env);
  Lexeme *right = eval(car(tree),env);
  Lexeme *result = lexeme(INTEGER);
  result->ival = 0;
  if(areEqual(left,right,INTEGER)) {
    if(left->ival) {
      result->ival = !right->ival;
    } else {
      result->ival = right->ival;
    }
  }
  return result;
}

evalFunction getEvalFunction(char *type) {
  if(strcmp(type,PLUS) == 0) {
    return evalPlus;
  } else if(strcmp(type,ASSIGN) == 0) {
    return evalAssign;
  } else if(strcmp(type,LAMBDA) == 0) {
    return evalFuncDef;
  } else if(strcmp(type,FUNCCALL) == 0) {
    return evalFuncCall;
  } else if(strcmp(type,IF) == 0) {
    return evalIf;
  } else if(strcmp(type,ELSE) == 0) {
    return evalElse;
  } else if(strcmp(type,WHILE) == 0) {
    return evalWhile;
  } else if(strcmp(type,INCREMENT) == 0) {
    return evalIncrement;
  } else if(strcmp(type,TIMES) == 0) {
    return evalTimes;
  } else if(strcmp(type,MINUS) == 0) {
    return evalMinus;
  } else if(strcmp(type,DECREMENT) == 0) {
    return evalDecrement;
  } else if(strcmp(type,DIVIDE) == 0) {
    return evalDivide;
  } else if(strcmp(type,EXP) == 0) {
    return evalExp;
  } else if(strcmp(type,MOD) == 0) {
    return evalMod;
  } else if(strcmp(type,LESS_THAN) == 0) {
    return evalLess_than;
  } else if(strcmp(type,LT_EQ) == 0) {
    return evalLT_EQ;
  } else if(strcmp(type,GREATER_THAN) == 0) {
    return evalGreater_than;
  } else if(strcmp(type,GT_EQ) == 0) {
    return evalGT_EQ;
  } else if(strcmp(type,EQUAL) == 0) {
    return evalEqual;
  } else if(strcmp(type,NOT_EQUAL) == 0) {
    return evalNot_equal;
  } else if(strcmp(type,STRICT_EQ) == 0) {
    return evalStrict_equal;
  } else if(strcmp(type,STRICT_NEQ) == 0) {
    return evalStrict_nequal;
  } else if(strcmp(type,OR) == 0) {
    return evalOr;
  } else if(strcmp(type,AND) == 0) {
    return evalAnd;
  } else if(strcmp(type,NOT) == 0) {
    return evalNot;
  } else if(strcmp(type,ASSIGN) == 0) {
    return evalAssign;
  } else if(strcmp(type,XOR) == 0) {
    return evalXor;
  } else if(strcmp(type,ARRAY) == 0) {
    return evalArray;
  } else if(strcmp(type,ARRAYCALL) == 0) {
    return evalArrayCall;
  } else if(strcmp(type,VAR) == 0) {
    return evalVar;
  } else if(strcmp(type,ID) == 0) {
    return evalId;
  } else if(strcmp(type,INTEGER) == 0) {
    return evalInt;
  } else if(strcmp(type,STRING) == 0) {
    return evalString;
  } else if(strcmp(type,STATEMENTLIST) == 0) {
    return evalStatementList;
  } else if(strcmp(type,DOT) == 0) {
    return evalDot;
  } else if(strcmp(type,NIL) == 0) {
    return evalNil;
  } else if(!strcmp(type,THUNK)) {
    return evalThunk;
  } else if(!strcmp(type,JOIN)) {
    return evalJoin;
  } else if(!strcmp(type,NIL)) {
    return evalNil;
  } else {
    return NULL;
  }
}
