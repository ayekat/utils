#define _XOPEN_SOURCE 500

#include "field.h"
#include "display.h"
#include "util.h"
#include <unistd.h>
#include <stdarg.h>

void
display_draw(struct state *s, int unsigned row, int unsigned col)
{
	int unsigned r, c;
	struct field *f;

	for (r = 0; r < 9; ++r) {
		if (r > 0 && r % 3 == 0)
			printf("\n");
		for (c = 0; c < 9; ++c) {
			f = &s->fields[r][c];
			if (c > 0 && c % 3 == 0)
				printf(" ");
			if (f->value == 0)
				printf("\033[3%dm%zu\033[0m ",
				       f->noptions == 1 ? 2 : 4, f->noptions);
			else
				printf("%s%u%s ",
				       r == row && c == col ? "\033[1m" : "",
				       f->value,
				       r == row && c == col ? "\033[0m" : "");
		}
		printf("\n");
	}
}

void
display_init(void)
{
	int unsigned i;

	for (i = 0; i < 11; ++i)
		printf("\n");
}

void
display_message(char const *format, ...)
{
	va_list args;

	va_start(args, format);
	(void) vprintf(format, args);
	va_end(args);
	printf("\r");
}

void
display_update(struct state *s, int unsigned row, int unsigned col)
{
	printf("\033[11F");
	display_draw(s, row, col);
	usleep(100000);
}
