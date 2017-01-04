#ifndef _SUDOKU_DISPLAY_H
#define _SUDOKU_DISPLAY_H

#include "state.h"

void display_draw(struct state *s, int unsigned row, int unsigned col);
void display_init(void);
void display_message(char const *format, ...)
                     __attribute__((format(printf,1,2)));
void display_update(struct state *s, int unsigned row, int unsigned col);

#endif /* ndef _SUDOKU_DISPLAY_H */
