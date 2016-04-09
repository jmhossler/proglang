/*
 * AUTHOR: John Hossler
 */

#ifndef TYPES_JH_CS403_2016
#define TYPES_JH_CS403_2016

// TODO define all of these within types.c

/* Glue keywords */
extern char *STATEMENTLIST;
extern char *WHILESTATEMENT;
extern char *IFSTATEMENT;
extern char *JOIN;
extern char *INIT;
extern char *ARRAY;
extern char *FUNCCALL;
extern char *FUNCDEF;
extern char *ARRAYCALL;
extern char *EXPRESSIONLIST;
extern char *PARAMLIST;
extern char *ENV;
extern char *VALUES;
extern char *BUILTIN;
extern char *CLOSURE;
extern char *THUNK;

/* Keywords for my designer language */
extern char *LAMBDA;
extern char *IF;
extern char *ELIF;
extern char *ELSE;
extern char *VAR;
extern char *BAD_LEX;
extern char *WHILE;

/* Internal types */
extern char *STRING;
extern char *INTEGER;
extern char *NIL;

/* Not a keyword? */
extern char *ID;

/* Puncuation for my designer language */
extern char *OPAREN;
extern char *CPAREN;
extern char *COMMA;
extern char *OB;
extern char *CB;
extern char *OBRACE;
extern char *CBRACE;
extern char *SEMI;
extern char *DOT;
extern char *END_OF_INPUT;

/* Operators */
extern char *PLUS;
extern char *INCREMENT;
extern char *TIMES;
extern char *MINUS;
extern char *DECREMENT;
extern char *DIVIDE;
extern char *EXP;
extern char *MOD;
extern char *LESS_THAN;
extern char *LT_EQ;
extern char *GREATER_THAN;
extern char *GT_EQ;
extern char *EQUAL;
extern char *NOT_EQUAL;
extern char *STRICT_EQ;
extern char *STRICT_NEQ;
extern char *OR;
extern char *AND;
extern char *NOT;
extern char *ASSIGN;
extern char *XOR;

#endif
