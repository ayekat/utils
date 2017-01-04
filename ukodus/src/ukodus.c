#include "state.h"
#include "util.h"
#include "display.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>
#include <string.h>

struct {
	struct state *state;
	struct state *solutions;
	size_t nsolutions;
} sudoku;

static int cancel_value(int unsigned row, int unsigned col, int unsigned value);
static bool check_unique(int unsigned row, int unsigned col, int unsigned value);
static int fix_value(int unsigned row, int unsigned col, int unsigned value);
static void solve(void);

static int
cancel_value(int unsigned row, int unsigned col, int unsigned value)
{
	int unsigned i;
	struct field *f = &sudoku.state->fields[row][col];

	if (f->value != (size_t) 0)
		return value == f->value ? -1 : 0;

	f->noptions -= (size_t) f->options[value - 1];
	f->options[value - 1] = false;

	switch (f->noptions) {
	case 0:
		/* should not happen */
		DIE("row=%u|col=%u: noptions=%zu (value=%u), cancel=%u (option?%d)",
		    row, col, f->noptions, f->value, value, f->options[value - 1]);
	case 1:
		for (i = 0; i < 9 && !f->options[i]; ++i);
		if (i == 9)
			DIE("noptions == 1 but not found");
		if (fix_value(row, col, i + 1) < 0)
			return -1;
		break;
	default:
		for (i = 0; i < 9; ++i)
			if (f->options[i] && check_unique(row, col, i + 1))
				if (fix_value(row, col, i + 1) < 0)
					return -1;
		break;
	}
	return 0;
}

static bool
check_unique(int unsigned row, int unsigned col, int unsigned value)
{
	int unsigned r, c;

	/* same row */
	for (r = row, c = 0; c < 9; ++c) {
		if (c == col)
			continue;
		if (sudoku.state->fields[r][c].options[value - 1])
			return false;
	}

	/* same column */
	for (c = col, r = 0; r < 9; ++r) {
		if (r == row)
			continue;
		if (sudoku.state->fields[r][c].options[value - 1])
			return false;
	}

	/* same area */
	for (r = row / 3 * 3; r < row / 3 * 3 + 3; ++r) {
		for (c = col / 3 * 3; c < col / 3 * 3 + 3; ++c) {
			if (r == row && c == col)
				continue;
			if (sudoku.state->fields[r][c].options[value - 1])
				return false;
		}
	}

	/* unique! */
	return true;
}

static int
fix_value(int unsigned row, int unsigned col, int unsigned value)
{
	int unsigned i, r, c;
	struct field *f = &sudoku.state->fields[row][col];

	/* value already fixed */
	if (f->value == value)
		return 0;

	/* value conflict */
	if (f->value % value != 0)
		return -1;

	/* set value */
	for (i = 0; i < 9; ++i)
		f->options[i] = i + 1 == value;
	f->noptions = 1;
	f->value = value;

	/* update state */
	--sudoku.state->nopen;
	display_message("[%u|%u] -> %u (%zu left)",
	                row, col, f->value, sudoku.state->nopen);

	/* cancel on same row */
	for (c = 0; c < 9; ++c) {
		if (c == col)
			continue;
		if (cancel_value(row, c, value) < 0)
			return -1;
	}

	/* cancel on same column */
	for (r = 0; r < 9; ++r) {
		if (r == row)
			continue;
		if (cancel_value(r, col, value) < 0)
			return -1;
	}

	/* cancel in same area */
	for (c = col / 3 * 3; c < col / 3 * 3 + 3; ++c) {
		for (r = row / 3 * 3; r < row / 3 * 3 + 3; ++r) {
			if (r == row && c == col)
				continue;
			if (cancel_value(r, c, value) < 0)
				return -1;
		}
	}

	/* update display */
	display_update(sudoku.state, row, col);

	return 0;
}

static void
solve(void)
{
	int unsigned i, r, c;
	struct field *f;
	struct state *hypothesis, *oldstate, *solution;

	oldstate = sudoku.state;

	/* solved, add state to list of solutions */
	if (sudoku.state->nopen == 0) {
		solution = state_new();
		state_copy(solution, oldstate);
		if (sudoku.solutions == NULL) {
			sudoku.solutions = solution;
		} else {
			sudoku.solutions->prev = solution;
			solution->next = sudoku.solutions;
		}
		++sudoku.nsolutions;
		return;
	}

	/* find next open field (naive searching) */
	for (r = 0; r < 9; ++r) {
		for (c = 0; c < 9; ++c) {
			f = &oldstate->fields[r][c];
			if (f->value == 0)
				goto solve_hypothesis;
		}
	}

	/* try fixing field to all possible values */
 solve_hypothesis:
	hypothesis = state_new();
	sudoku.state = hypothesis;
	for (i = 0; i < 9; ++i) {
		if (!f->options[i])
			continue;
		state_copy(hypothesis, oldstate);
		if (fix_value(r, c, i + 1) == 0)
			solve();
	}
	sudoku.state = oldstate;
	state_delete(hypothesis);
}

int
main(int argc, char **argv)
{
	int unsigned row, col;
	struct state *s;
	(void) argc;
	(void) argv;

	//int unsigned board[9][9] = {
	//	{ 8, 0, 0,  0, 0, 0,  0, 0, 0 },
	//	{ 0, 0, 3,  6, 0, 0,  0, 0, 0 },
	//	{ 0, 7, 0,  0, 9, 0,  2, 0, 0 },
	//	{ 0, 5, 0,  0, 0, 7,  0, 0, 0 },
	//	{ 0, 0, 0,  0, 4, 5,  7, 0, 0 },
	//	{ 0, 0, 0,  1, 0, 0,  0, 3, 0 },
	//	{ 0, 0, 1,  0, 0, 0,  0, 6, 8 },
	//	{ 0, 0, 8,  5, 0, 0,  0, 1, 0 },
	//	{ 0, 9, 0,  0, 0, 0,  4, 0, 0 },
	//};
	//int unsigned board[9][9] = {
	//	{ 1, 0, 0,  0, 0, 7,  0, 9, 0 },
	//	{ 0, 3, 0,  0, 2, 0,  0, 0, 8 },
	//	{ 0, 0, 9,  6, 0, 0,  5, 0, 0 },
	//	{ 0, 0, 5,  3, 0, 0,  9, 0, 0 },
	//	{ 0, 1, 0,  0, 8, 0,  0, 0, 2 },
	//	{ 6, 0, 0,  0, 0, 4,  0, 0, 0 },
	//	{ 3, 0, 0,  0, 0, 0,  0, 1, 0 },
	//	{ 0, 4, 0,  0, 0, 0,  0, 0, 7 },
	//	{ 0, 0, 7,  0, 0, 0,  3, 0, 0 },
	//};
	int unsigned board[9][9] = {
		{ 5, 0, 6,  9, 0, 3,  2, 0, 0 },
		{ 0, 0, 0,  0, 6, 0,  0, 7, 0 },
		{ 8, 0, 0,  0, 0, 0,  0, 0, 4 },
		{ 1, 0, 0,  0, 0, 7,  0, 0, 3 },
		{ 0, 4, 0,  0, 0, 0,  0, 9, 0 },
		{ 6, 0, 0,  2, 0, 0,  0, 0, 7 },
		{ 4, 0, 0,  0, 0, 0,  0, 0, 6 },
		{ 0, 1, 0,  0, 9, 0,  0, 0, 0 },
		{ 0, 0, 2,  5, 0, 8,  3, 0, 1 },
	};
	//int unsigned board[9][9] = {
	//	{ 8, 0, 0,  4, 0, 6,  0, 0, 7 },
	//	{ 0, 0, 0,  0, 0, 0,  4, 0, 0 },
	//	{ 0, 1, 0,  0, 0, 0,  6, 5, 0 },
	//	{ 5, 0, 9,  0, 3, 0,  7, 8, 0 },
	//	{ 0, 0, 0,  0, 7, 0,  0, 0, 0 },
	//	{ 0, 4, 8,  0, 2, 0,  1, 0, 3 },
	//	{ 0, 5, 2,  0, 0, 0,  0, 9, 0 },
	//	{ 0, 0, 1,  0, 0, 0,  0, 0, 0 },
	//	{ 3, 0, 0,  9, 0, 2,  0, 0, 5 },
	//};
	//int unsigned board[9][9] = {
	//	{ 5, 3, 0,  0, 7, 0,  0, 0, 0 },
	//	{ 6, 0, 0,  1, 9, 5,  0, 0, 0 },
	//	{ 0, 9, 8,  0, 0, 0,  0, 6, 0 },
	//	{ 8, 0, 0,  0, 6, 0,  0, 0, 3 },
	//	{ 4, 0, 0,  8, 0, 3,  0, 0, 1 },
	//	{ 7, 0, 0,  0, 2, 0,  0, 0, 6 },
	//	{ 0, 6, 0,  0, 0, 0,  2, 8, 0 },
	//	{ 0, 0, 0,  4, 1, 9,  0, 0, 5 },
	//	{ 0, 0, 0,  0, 8, 0,  0, 7, 9 },
	//};

	/* initialise states and display */
	sudoku.solutions = NULL;
	sudoku.state = state_new();
	state_init(sudoku.state);
	display_init();

	/* insert values and solve */
	for (row = 0; row < 9; ++row)
		for (col = 0; col < 9; ++col)
			if (board[row][col] != 0)
				fix_value(row, col, board[row][col]);
	solve();

	/* display solutions and clean up */
	while (sudoku.nsolutions > 0) {
		/* detach first solution */
		s = sudoku.solutions;
		sudoku.solutions = sudoku.solutions->next;
		--sudoku.nsolutions;

		/* display */
		printf("-----<>-----<>-----                                \n");
		display_draw(s, 9, 9);
		state_delete(s);
	}
	state_delete(sudoku.state);
	printf("\n");
	return 0;
}
