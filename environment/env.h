#ifndef ENV_CS403_JH_2016
#define ENV_CS403_JH_2016
#include "../lexer/lexer.h"

Lexeme *create();
Lexeme *lookup(Lexeme *, Lexeme *);
Lexeme *update(Lexeme *, Lexeme *, Lexeme *);
Lexeme *insert(Lexeme *, Lexeme *, Lexeme *);
Lexeme *extend(Lexeme *, Lexeme *, Lexeme *);

void displayEnv(Lexeme *);

#endif
