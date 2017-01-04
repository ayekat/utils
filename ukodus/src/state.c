#include "state.h"
#include "util.h"
#include <string.h>

void
state_copy(struct state *dst, struct state *src)
{
	(void) memcpy(dst, src, sizeof(struct state));
}

void
state_delete(struct state *s)
{
	free(s);
}

void
state_init(struct state *s)
{
	int unsigned r, c;

	for (r = 0; r < 9; ++r)
		for (c = 0; c < 9; ++c)
			field_init(&s->fields[r][c]);
	s->nopen = 81;
}

struct state *
state_new(void)
{
	struct state *s;

	s = malloc(sizeof(struct state));
	s->prev = s->next = NULL;
	return s;
}
