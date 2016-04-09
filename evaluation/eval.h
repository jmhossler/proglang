#ifndef EVAL_CS403_JH_2016
#define EVAL_CS403_JH_2016
#include "lexer.h"

Lexeme *eval(Lexeme *, Lexeme *);
Lexeme *evalPrint(Lexeme *, Lexeme *);
Lexeme *evalInclude(Lexeme *, Lexeme *);
// Put in BUILTIN list
Lexeme *evalCar(Lexeme *, Lexeme *);
Lexeme *evalCons(Lexeme *, Lexeme *);
Lexeme *evalCdr(Lexeme *, Lexeme *);
Lexeme *evalArr(Lexeme *, Lexeme *);
Lexeme *evalAppend(Lexeme *, Lexeme *);

#endif
