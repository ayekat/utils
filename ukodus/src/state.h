#ifndef _SUDOKU_STATE_H
#define _SUDOKU_STATE_H

#include "field.h"
#include <stdlib.h>

struct state {
	struct state *prev, *next;
	struct field fields[9][9];
	size_t nopen;
};

void state_copy(struct state *dst, struct state *src);
void state_delete(struct state *s);
void state_init(struct state *s);
struct state *state_new(void);

#endif /* ndef _SUDOKU_STATE_H */
