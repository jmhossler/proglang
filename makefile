compile: lexer.o parser.o types.o env.o eval.o compile.o
	gcc -Wall -o compile lexer.o parser.o types.o env.o eval.o compile.o -lm

compile.o: compile.c
	gcc -Wall -c compile.c

env.o: environment/env.c environment/env.h
	gcc -Wall -c environment/env.c

eval.o: evaluation/eval.c evaluation/eval.h
	gcc -Wall -c evaluation/eval.c

parser.o: parser/parser.c parser/parser.h
	gcc -Wall -c parser/parser.c

lexer.o: lexer/lexer.c lexer/lexer.h
	gcc -Wall -c lexer/lexer.c

types.o: types/types.c types/types.h
	gcc -Wall -c types/types.c

clean:
	rm *.o compile

run:
	bash test.sh
