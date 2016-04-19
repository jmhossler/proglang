#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "../parser/parser.h"
#include "../types/types.h"
#include "lexer.h"

int isWhiteSpace(int);
void skipWhiteSpace(Parser *);
int getNextCharacter(Parser *);
char *resize(char *, int *);

Lexeme *lexNumber(Parser *, int);
Lexeme *lexString(Parser *, int);
Lexeme *lexID(Parser *, int);
Lexeme *badLexeme(int);

// TODO cleanup
Lexeme *lex(Parser *p) {
  int c;
  skipWhiteSpace(p);
  c = getNextCharacter(p);
  if(c == EOF) {
    return lexeme(END_OF_INPUT);
  }
  switch(c) {
    case '(':
      return lexeme(OPAREN);
    case ')':
      return lexeme(CPAREN);
    case '{':
      return lexeme(OB);
    case '}':
      return lexeme(CB);
    case '[':
      return lexeme(OBRACE);
    case ']':
      return lexeme(CBRACE);
    case '.':
      return lexeme(DOT);
    case ',':
      return lexeme(COMMA);
    case '+':
      c = getNextCharacter(p);
      if(c == '=') {
        return lexeme(INCREMENT);
      }
      ungetc(c,p->fp);
      return lexeme(PLUS);
    case '-':
      c = getNextCharacter(p);
      if(c == '=') {
        return lexeme(DECREMENT);
      }
      ungetc(c,p->fp);
      return lexeme(MINUS);
    case '*':
      c = getNextCharacter(p);
      if(c == '*') {
        return lexeme(EXP);
      }
      ungetc(c,p->fp);
      return lexeme(TIMES);
    case '/':
      return lexeme(DIVIDE);
    case '%':
      return lexeme(MOD);
    case '<':
      c = getNextCharacter(p);
      if(c == '=') {
        return lexeme(LT_EQ);
      }
      ungetc(c,p->fp);
      return lexeme(LESS_THAN);
    case '>':
      c = getNextCharacter(p);
      if(c == '=') {
        return lexeme(GT_EQ);
      }
      ungetc(c,p->fp);
      return lexeme(GREATER_THAN);
    case '=':
      c = getNextCharacter(p);
      if(c == '=') {
        c = getNextCharacter(p);
        if(c == '=') {
          return lexeme(STRICT_EQ);
        }
        ungetc(c,p->fp);
        return lexeme(EQUAL);
      }
      ungetc(c,p->fp);
      return lexeme(ASSIGN);
    case ';':
      return lexeme(SEMI);
    case '!':
      c = getNextCharacter(p);
      if(c == '=') {
        c = getNextCharacter(p);
        if(c == '=') {
          return lexeme(STRICT_NEQ);
        }
        ungetc(c,p->fp);
        return lexeme(NOT_EQUAL);
      }
      ungetc(c,p->fp);
      return lexeme(NOT);
    case '&':
      c = getNextCharacter(p);
      if(c == '&') {
        return lexeme(AND);
      }
      ungetc(c,p->fp);
      return badLexeme(c);
    case '|':
      c = getNextCharacter(p);
      if(c == '|') {
        return lexeme(OR);
      }
      ungetc(c,p->fp);
      return badLexeme(c);
    case '^':
      return lexeme(XOR);

    default:
      if(isdigit(c)) {
        return lexNumber(p,c);
      }
      else if(isalpha(c) || c == '@') {
        return lexID(p,c);
      }
      else if(c == '\"') {
        return lexString(p,c);
      }
      else {
        return badLexeme(c);
      }
  }
}

Lexeme *lexeme(char *s) {
  Lexeme *new = malloc(sizeof(Lexeme));
  new->type = s;
  new->sval = NULL;
  new->ival = 0;
  new->left = NULL;
  new->right = NULL;
  new->builtin = NULL;
  return new;
}

Lexeme *lexNumber(Parser *p, int c) {
  int filled = 1, size = 16;
  int n;
  char *num = malloc(sizeof(char) * size);
  Lexeme *new = lexeme(INTEGER);
  num[0] = c;
  n = getNextCharacter(p);

  while(isdigit(n) && !isWhiteSpace(n) && n != EOF) {
    if(filled >= size) {
      num = resize(num,&size);
    }
    num[filled++] = n;
    n = getNextCharacter(p);
  }
  ungetc(n,p->fp);
  num[filled++] = '\0';
  new->ival = atoi(num);
  new->sval = num;

  return new;
}

Lexeme *lexID(Parser *p, int c) {
  int filled = 1, size = 16;
  int n;
  Lexeme *new = lexeme(ID);
  char *name = malloc(sizeof(char) * size);
  name[0] = c;
  n = getNextCharacter(p);

  while((isalpha(n) || isdigit(n)) && !isWhiteSpace(n) && n != EOF) {
    if(filled >= size) {
      name = resize(name,&size);
    }
    name[filled++] = n;
    n = getNextCharacter(p);
  }
  ungetc(n,p->fp);
  name[filled++] = '\0';
  if(!strcmp(name,"function")) {
    new->type = LAMBDA;
  } else if(!strcmp(name,"if")) {
    new->type = IF;
  } else if(!strcmp(name,"else")) {
    new->type = ELSE;
  } else if(!strcmp(name,"var")) {
    new->type = VAR;
  } else if(!strcmp(name,"while")) {
    new->type = WHILE;
  } else if(!strcmp(name,"nil")) {
    new->type = NIL;
  } else {
    new->sval = name;
  }
  return new;
}

Lexeme *lexString(Parser *p, int c) {
  int filled = 0, size = 16;
  int n;
  Lexeme *new = lexeme(STRING);
  char *string = malloc(sizeof(char) * size);
  n = getNextCharacter(p);

  while(n != '\"') {
    if(n == EOF) {
      fprintf(stderr,"FATAL ERROR: STRING NOT CLOSED\n");
      exit(1);
    }
    if(filled >= size) {
      string = resize(string,&size);
    }
    if(n != '\\') {
      string[filled++] = n;
    } else {
      n = getNextCharacter(p);
      if(n == 'n') {
        string[filled++] = '\n';
      } else if(n == 't') {
        string[filled++] = '\t';
      } else if(n == 'r') {
        string[filled++] = '\r';
      } else {
        string[filled++] = n;
      }
    }
    n = getNextCharacter(p);
  }
  string[filled++] = '\0';
  new->sval = string;
  new->ival = filled;

  return new;
}

Lexeme *badLexeme(int c) {
  Lexeme *new = lexeme(BAD_LEX);
  char *bad_char = malloc(sizeof(char));
  bad_char[0] = c;
  new->sval = bad_char;
  return new;
}

Lexeme *cons(char *type, Lexeme *left, Lexeme *right) {
  Lexeme *new = lexeme(type);
  new->left = left;
  new->right = right;

  return new;
}

Lexeme *car(Lexeme *l) {
  return l->left;
}

Lexeme *cdr(Lexeme *l) {
  return l->right;
}

void setCar(Lexeme *l, Lexeme *left) {
  l->left = left;
}

void setCdr(Lexeme *l, Lexeme *right) {
  l->right = right;
}

char *resize(char *original, int *size) {
  *size *= 2;
  return realloc(original, sizeof(char) * (*size));
}

// TODO fix up
// maybe make it return a string which is the type and value
char *displayLexeme(Lexeme l) {
  char *string = malloc(sizeof(char) * 64);
  if(l.type == INTEGER) {
    sprintf(string,"%s %d",l.type,l.ival);
  }
  else if(l.type == STRING || l.type == ID || l.type == BAD_LEX) {
    sprintf(string,"%s %s",l.type,l.sval);
  }
  else {
    sprintf(string,"%s",l.type);
  }
  return string;
}

void skipWhiteSpace(Parser *p) {
  int c = getNextCharacter(p);

  while(!feof(p->fp) && isWhiteSpace(c)) {
    if(c == '\n') {
      p->line++;
    }
    c = getNextCharacter(p);
  }
  ungetc(c,p->fp);
}

int isWhiteSpace(int c) {
  return c == ' ' || c == '\n' || c == '\r' || c == '\t';
}

// TODO ignore comments
int getNextCharacter(Parser *p) {
  int c = fgetc(p->fp);

  // ignoring comments
  if(c == '/') {
    int s = fgetc(p->fp);
    if(s == '/') {
      while(!feof(p->fp) && c != '\n') {
        c = fgetc(p->fp);
      }
    } else {
      ungetc(s,p->fp);
    }
  }

  return c;
}
