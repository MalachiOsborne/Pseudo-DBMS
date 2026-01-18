ifeq ($(OS),Windows_NT)
    LIBS =
else
    LIBS = -lm
endif
CC=clang
<<<<<<< HEAD
CFLAGS=-ggdb3 -gdwarf-4 -O0 -std=c11 -Wall -Werror -Wextra -Qunused-arguments  -Wno-gnu-folding-constant -Wno-sign-compare -Wno-unused-parameter -Wno-unused-variable -Wshadow -Iinclude/ -lm
=======
CFLAGS=-ggdb3 -gdwarf-4 -O0 -std=c11 -Wall -Werror -Wextra -Qunused-arguments  -Wno-gnu-folding-constant -Wno-sign-compare -Wno-unused-parameter -Wno-unused-variable -Wshadow -Iinclude -D_CRT_SECURE_NO_WARNINGS
>>>>>>> ca9c112038a5b9eaa228c5363f3d93e520b62951

tracker: src/main.c src/llf.c
	$(CC) $(CFLAGS) src/main.c src/llf.c $(LIBS) -o tracker.exe

