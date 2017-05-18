CC := gcc
CFLAGS := -Wall -Wextra -O3 -Wno-unused-parameter
LDFLAGS := -lm
TARGET := compile
OBJS := lexer.o parser.o types.o env.o eval.o compile.o
DIRS := environment evaluation parser lexer types

.PHONY: clean

vpath %.c $(DIRS)
vpath %.h $(DIRS)

$(TARGET): $(OBJS)

compile.o: compile.c

env.o: env.h

eval.o: eval.h

parser.o: parser.h

lexer.o: lexer.h

types.o: types.h

clean:
	rm -f $(OBJS) $(TARGET)
run:
	./test.sh
