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

.IGNORE: run-error1 run-error2 run-error3

cat-error1:
	cat testScripts/error1.sux

run-error1:
	./compile testScripts/error1.sux || true

cat-error2:
	cat testScripts/error2.sux

run-error2:
	./compile testScripts/error2.sux || true

cat-error3:
	cat testScripts/error3.sux

run-error3:
	./compile testScripts/error3.sux || true

cat-arrays:
	cat testScripts/testArr.sux

run-arrays:
	./compile testScripts/testArr.sux

cat-conditionals:
	cat testScripts/ifelseifelse.sux

run-conditionals:
	./compile testScripts/ifelseifelse.sux

cat-recursion:
	cat testScripts/printFib.sux

run-recursion:
	./compile testScripts/printFib.sux

cat-iteration:
	cat testScripts/while.sux

run-iteration:
	./compile testScripts/while.sux

cat-functions:
	cat testScripts/testFirstclass.sux

run-functions:
	./compile testScripts/testFirstclass.sux

cat-dictionary:
	cat testScripts/testDictionary.sux

run-dictionary:
	./compile testScripts/testDictionary.sux
