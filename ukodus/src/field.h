#ifndef _SUDOKU_FIELD_H
#define _SUDOKU_FIELD_H

#include <stdbool.h>
#include <stdlib.h>

struct field {
	bool options[9];
	size_t noptions;
	int unsigned value;
};

void field_init(struct field *f);

#endif /* ndef _SUDOKU_FIELD_H */
