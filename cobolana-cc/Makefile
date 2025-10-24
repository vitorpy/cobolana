# Makefile for cobolana-cc: COBOL to sBPF compiler
# Uses GnuCOBOL's parser with our sBPF backend

CC = gcc
CFLAGS = -Wall -g -O2 -DHAVE_CONFIG_H -I. -I/home/vitorpy/code/gnucobol-code
LDFLAGS = -lgmp -lm

# Core sources (no libcob runtime needed!)
PARSER_SRCS = parser.c scanner.c ppparse.c pplex.c
TREE_SRCS = tree.c field.c typeck.c
CONFIG_SRCS = config.c reserved.c error.c replace.c
CODEGEN_SRCS = codegen.c
MAIN_SRCS = cobc.c help.c stubs.c

OBJS = $(PARSER_SRCS:.c=.o) $(TREE_SRCS:.c=.o) $(CONFIG_SRCS:.c=.o) $(CODEGEN_SRCS:.c=.o) $(MAIN_SRCS:.c=.o)

all: cobolana-cc

# Generate parser from bison
parser.c parser.h: parser.y
	bison -d -o parser.c parser.y

# Generate preprocessor parser
ppparse.c ppparse.h: ppparse.y
	bison -d -o ppparse.c ppparse.y

# Generate scanner from flex
scanner.c: scanner.l
	flex -o scanner.c scanner.l

# Generate preprocessor scanner
pplex.c: pplex.l
	flex -o pplex.c pplex.l

cobolana-cc: parser.c scanner.c ppparse.c pplex.c $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

clean:
	rm -f *.o parser.c parser.h scanner.c ppparse.c ppparse.h pplex.c cobolana-cc

.PHONY: all clean
