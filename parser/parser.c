/*
 * Author: John Hossler
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include "../lexer/lexer.h"
#include "../parser/parser.h"
#include "../types/types.h"

/*--------FUNCTION DECLARATIONS-------*/

int check(Parser *, char *);
void advance(Parser *);
Lexeme *match(Parser *, char *);
void matchNoAdvance(Parser *, char *);

// Helper functions
char *type(Lexeme *);

// Pending functions
int statementListPending(Parser *); // optional
int expressionPending(Parser *);
int initPending(Parser *); // optional
int primaryPending(Parser *);
int parenExprPending(Parser *);
int idExprPending(Parser *);
int primitivePending(Parser *);
int operatorPending(Parser *);
int expressionListPending(Parser *); // optional
int blockPending(Parser *);
int statementPending(Parser *);
int ifStatementPending(Parser *);
int elsePending(Parser *); // optional
int whileLoopPending(Parser *);
int anonFuncPending(Parser *);
int parameterListPending(Parser *); // optional
int uOpPending(Parser *); // TODO

// Consumption functions
// Program
Lexeme *statementList(Parser *);

// StatementList
Lexeme *statement(Parser *);

// Statement
Lexeme *ifStatement(Parser *);
Lexeme *whileLoop(Parser *);
Lexeme *expression(Parser *);

// if
Lexeme *block(Parser *);
Lexeme *elseStatement(Parser *);

// Expression
Lexeme *primary(Parser *);
Lexeme *operator(Parser *);
Lexeme *init(Parser *);

// Primary
Lexeme *primitive(Parser *);
Lexeme *idExpr(Parser *);
Lexeme *parenExpr(Parser *);
Lexeme *expressionList(Parser *);
Lexeme *anonFunc(Parser *);
Lexeme *uOp(Parser *);

// AnonFunc
Lexeme *parameterList(Parser *);

/*-------------FUNCTION DEFINTITIONS----------------*/

// Main driver, will consume the whole file
Lexeme *parse(FILE *fp) {
  // allocate space for parser struct
  Parser *p = malloc(sizeof(Parser));
  // Initialize values
  p->fp = fp;
  p->pending = lex(p->fp);
  p->line = 1;
  p->out = stdout;
  Lexeme *tree = NULL;

  // Consume Lexemes
  if(statementListPending(p)) {
    tree = statementList(p);
  }

  // return parse tree for evaluation
  return tree;
}

// Check, Advance, Match, and MatchNoAdvance functions
int check(Parser *p, char *t) {
  return type(p->pending) == t;
}

void advance(Parser *p) {
  p->previous = p->pending;
  p->pending = lex(p->fp);
}

Lexeme *match(Parser *p, char *t) {
  Lexeme *current = p->pending;
  matchNoAdvance(p,t);
  advance(p);
  return current;
}

void matchNoAdvance(Parser *p, char *t) {
  if(!check(p,t)) {
    printf("illegal\n");
    fprintf(stderr,"Syntax error\n");
    fprintf(stderr,"Expected type %s, found %s\n",t,displayLexeme(*(p->pending)));
    fprintf(stderr,"Previous type %s\n",displayLexeme(*(p->previous)));
    exit(1);
  }
}

// Helper Functions
char *type(Lexeme *l) {
  return l->type;
}

int typeEqual(Lexeme *a, Lexeme *b) {
  return strcmp(a->type,b->type) == 0;
}

// PENDING DEFINITIONS

int uOpPending(Parser *p) {
  return check(p,MINUS) || check(p,NOT);
}

int statementListPending(Parser *p) {
  return statementPending(p);
}

int statementPending(Parser *p) {
  return ifStatementPending(p) || whileLoopPending(p) || expressionPending(p);
}

int ifStatementPending(Parser *p) {
  return check(p,IF);
}

int whileLoopPending(Parser *p) {
  return check(p,WHILE);
}

int expressionPending(Parser *p) {
  return primaryPending(p) || check(p,VAR);
}

int elsePending(Parser *p) {
  return check(p,ELSE);
}

int blockPending(Parser *p) {
  return check(p,OB);
}

int primaryPending(Parser *p) {
  return primitivePending(p) || idExprPending(p) || parenExprPending(p) || check(p,OB) || anonFuncPending(p) || uOpPending(p);
}

int operatorPending(Parser *p) {
  return check(p,PLUS) || check(p,TIMES) || check(p,MINUS) || check(p,DIVIDE) || check(p,EXP) || check(p,MOD) || check(p,LESS_THAN) || check(p,LT_EQ) || check(p,GREATER_THAN) || check(p,GT_EQ) || check(p,EQUAL) || check(p,NOT_EQUAL) || check(p,STRICT_EQ) || check(p,STRICT_NEQ) || check(p,OR) || check(p,AND) || check(p,XOR) || check(p,ASSIGN) || check(p,INCREMENT) || check(p,DECREMENT);
}

int initPending(Parser *p) {
  return check(p,ASSIGN);
}

int primitivePending(Parser *p) {
  return check(p,STRING) || check(p,INTEGER) || check(p,NIL);
}

int idExprPending(Parser *p) {
  return check(p,ID);
}

int parenExprPending(Parser *p) {
  return check(p,OPAREN);
}

int anonFuncPending(Parser *p) {
  return check(p,LAMBDA);
}

int parameterListPending(Parser *p) {
  return check(p,ID) || check(p,OPAREN);
}

int expressionListPending(Parser *p) {
  return expressionPending(p);
}

// CONSUMPTION DEFINITIONS

// statementList: statement
//              | statement statementList
Lexeme *statementList(Parser *p) {
  Lexeme *a, *b = NULL;
  a = statement(p);
  if(statementListPending(p)) {
    b = statementList(p);
  }
  return cons(STATEMENTLIST,a,b);
}

// statement: ifStatement
//          | whileLoop
//          | expression SEMI
Lexeme *statement(Parser *p) {
  if(ifStatementPending(p)) {
    return ifStatement(p);
  } else if(whileLoopPending(p)) {
    return whileLoop(p);
  } else {
    Lexeme *a = expression(p);
    match(p,SEMI);
    return a;
  }
}

// ifStatement: IF OPAREN expression CPAREN block optElse
// TODO add   | IF OPAREN expression CPAREN block optElseif
Lexeme *ifStatement(Parser *p) {
  Lexeme *a,*b,*c;
  a = match(p,IF);
  match(p,OPAREN);
  a->left = expression(p);
  match(p,CPAREN);
  b = block(p);
  c = NULL;
  if(elsePending(p)) {
    c = elseStatement(p);
  }
  a->right = cons(JOIN,b,c);
  return a;
}

// whileLoop: WHILE OPAREN expression CPAREN block
Lexeme *whileLoop(Parser *p) {
  Lexeme *a;
  a = match(p,WHILE);
  match(p,OPAREN);
  a->left = expression(p);
  match(p,CPAREN);
  a->right = block(p);
  return a;
}

// expression: primary
//           | primary operator expression
//           | operator primary
//           | VAR ID optInit
Lexeme *expression(Parser *p) {
  Lexeme *a, *b, *c;
  if(primaryPending(p)) {
    a = primary(p);
    if(operatorPending(p)) {
      b = operator(p);
      c = expression(p);
      b->left = a;
      b->right = c;
      return b;
    }
    return a;
  } else {
    a = match(p,VAR);
    a->left = match(p,ID);
    a->right = NULL;
    if(initPending(p)) {
      a->right = init(p);
    } else {
      a->right = lexeme(NIL);
    }
    return a;
  }
}

Lexeme *uOp(Parser *p) {
  if(check(p,MINUS)) {
    return match(p,MINUS);
  } else {
    return match(p,NOT);
  }
}

// block: OB optStatementList CB
Lexeme *block(Parser *p) {
  Lexeme *a = NULL;
  match(p,OB);
  if(statementListPending(p)) {
    a = statementList(p);
  }
  match(p,CB);
  return a;
}

// optElse: ELSE block
//        | ELSE IF OPAREN expression CPAREN block optElse
//        | *empty*
Lexeme *elseStatement(Parser *p) {
  Lexeme *a = match(p,ELSE);
  if(check(p,IF)) {
    a = ifStatement(p);
  } else {
    a->left = block(p);
  }
  return a;;
}

// primary: primitive
//        | idExpr
//        | parenExpr
//        | OB optExpressionList CB
//        | anonFunc
Lexeme *primary(Parser *p) {
  if(primitivePending(p)) {
    return primitive(p);
  } else if(idExprPending(p)) {
    return idExpr(p);
  } else if(parenExprPending(p)) {
    return parenExpr(p);
  } else if(uOpPending(p)) {
    Lexeme *a = uOp(p);
    a->right = primary(p);
    return a;
  } else if(check(p,OB)) {
    Lexeme *a = lexeme(ARRAY);
    match(p,OB);
    if(expressionListPending(p)) {
      a->right = expressionList(p);
    }
    match(p,CB);
    return a;
  } else {
    return anonFunc(p);
  }
}

// TODO fix?
Lexeme *operator(Parser *p) {
  return match(p,p->pending->type);
}

Lexeme *init(Parser *p) {
  match(p,ASSIGN);
  return expression(p);
}

Lexeme *primitive(Parser *p) {
  if(check(p,STRING)) {
    return match(p,STRING);
  } else if(check(p,NIL)) {
      return match(p,NIL);
  } else {
    return match(p,INTEGER);
  }
}

Lexeme *idExpr(Parser *p) {
  Lexeme *a, *b = NULL;
  a = match(p,ID);
  if(check(p,OPAREN)) {
    match(p,OPAREN);
    if(expressionListPending(p)) {
      b = expressionList(p);
    }
    match(p,CPAREN);
    return cons(FUNCCALL,a,b);
  } else if(check(p,OB)) {
    match(p,OBRACE);
    b = expression(p);
    match(p,CBRACE);
    return cons(ARRAYCALL,a,b);
  } else if(check(p,DOT)) {
    b = match(p,DOT);
    b->right = idExpr(p);
    b->left = a;
    return b;
  }
  return a;
}

Lexeme *parenExpr(Parser *p) {
  Lexeme *a, *b = NULL;
  match(p,OPAREN);
  a = expression(p);
  match(p,CPAREN);
  if(check(p,OPAREN)) {
    match(p,OPAREN);
    if(expressionListPending(p)) {
      b = expressionList(p);
    }
    match(p,CPAREN);
    return cons(FUNCCALL,a,b);
  } else if(check(p,OB)) {
    match(p,OBRACE);
    b = expression(p);
    match(p,CBRACE);
    return cons(ARRAYCALL,a,b);
  } else if(check(p,DOT)) {
    b = match(p,DOT);
    b->right = idExpr(p);
    b->left = a;
    return b;
  }
  return a;
}

Lexeme *expressionList(Parser *p) {
  Lexeme *a, *b = NULL;
  a = expression(p);
  if(check(p,COMMA)) {
    match(p,COMMA);
    b = expressionList(p);
  }
  return cons(JOIN,a,b);
}

Lexeme *anonFunc(Parser *p) {
  Lexeme *a;
  a = match(p,LAMBDA);
  match(p,OPAREN);
  if(parameterListPending(p)) {
    a->left = parameterList(p);
  }
  match(p,CPAREN);
  a->right = block(p);
  return a;
}

Lexeme *parameterList(Parser *p) {
  Lexeme *a, *b = NULL;
  if(check(p,OPAREN)) {
    a = match(p,OPAREN);
    a->left = match(p,ID);
    match(p,CPAREN);
  } else {
    a = match(p,ID);
  }
  if(check(p,COMMA)) {
    match(p,COMMA);
    b = parameterList(p);
  }
  return cons(JOIN,a,b);
}

void displayTree(Lexeme *tree, char *head) {
  if(tree != NULL) {
    char *newHead = malloc(sizeof(char) * 1024);
    strcat(newHead,head);
    strcat(newHead,"\t");
    displayTree(tree->left,newHead);
    printf("%s%s\n",head,displayLexeme(*tree));
    displayTree(tree->right,newHead);
  }
}
