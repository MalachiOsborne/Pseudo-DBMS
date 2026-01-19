EXEEXT := $(if $(filter Windows_NT,$(OS)),.exe,)
EXE=pseudo-dbms$(EXEEXT)
pseudo-dbms: $(EXE)
ifeq ($(OS),Windows_NT)
    LIBS=
else
    LIBS = -lm
endif
CC=clang
CFLAGS=-ggdb3 -gdwarf-4 -O0 -std=c11 -Wall -Werror -Wextra -Qunused-arguments  -Wno-gnu-folding-constant -Wno-sign-compare -Wno-unused-parameter -Wno-unused-variable -Wshadow -Iinclude -D_CRT_SECURE_NO_WARNINGS
$(EXE): src/main.c src/llf.c src/dbmsf.c
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@
