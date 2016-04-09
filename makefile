all: recognizer testEval compile

testEval: lexer.o parser.o types.o env.o eval.o testEval.o
	gcc -Wall -o testEval lexer.o parser.o types.o env.o eval.o testEval.o -lm

compile: lexer.o parser.o types.o env.o eval.o compile.o
	gcc -Wall -o compile lexer.o parser.o types.o env.o eval.o compile.o -lm

compile.o: compile.c
	gcc -Wall -c compile.c

recognizer: recognizer.o lexer.o parser.o types.o
	gcc -Wall -o recognizer recognizer.o lexer.o parser.o types.o

recognizer.o: recognizer.c
	gcc -Wall -c recognizer.c

testEval.o: testEval.c
	gcc -Wall -c testEval.c

env.o: env.c env.h
	gcc -Wall -c env.c

eval.o: eval.c eval.h
	gcc -Wall -c eval.c

parser.o: parser.c parser.h
	gcc -Wall -c parser.c

lexer.o: lexer.c lexer.h
	gcc -Wall -c lexer.c

types.o: types.c types.h
	gcc -Wall -c types.c

clean:
	rm *.o recognizer testEval compile

run:
	bash test.sh
