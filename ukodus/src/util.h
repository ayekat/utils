#ifndef _SUDOKU_UTIL_H
#define _SUDOKU_UTIL_H

#include <stdio.h>

#define DIE(...) do \
{ \
	fprintf(stderr, "\n\033[31m" __VA_ARGS__); \
	fprintf(stderr, "\033[0m\n"); \
	exit(1); \
} while (0)

#endif /* ndef _SUDOKU_UTIL_H */
