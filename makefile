CC := gcc
CFLAGS := -Wall -Wextra -O3 -Wno-unused-parameter
LDFLAGS := -lm
TARGET := sux
OBJS := lexer.o parser.o types.o env.o eval.o sux.o
DIRS := environment evaluation parser lexer types

.PHONY: clean

vpath %.c $(DIRS)
vpath %.h $(DIRS)

$(TARGET): $(OBJS)
	$(CC) $^ $(LDFLAGS) -o $@

sux.o: sux.c

env.o: env.h

eval.o: eval.h

parser.o: parser.h

lexer.o: lexer.h

types.o: types.h

clean:
	rm -f $(OBJS) $(TARGET)
run:
	./test.sh
