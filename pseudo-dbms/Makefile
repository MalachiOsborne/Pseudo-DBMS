CC=clang
CFLAGS=-ggdb3 -gdwarf-4 -O0 -std=c11 -Wall -Werror -Wextra -Qunused-arguments  -Wno-gnu-folding-constant -Wno-sign-compare -Wno-unused-parameter -Wno-unused-variable -Wshadow -Iinclude -lm

tracker: src/main.c src/llf.c
	$(CC) $(CFLAGS) src/main.c src/llf.c -o tracker

