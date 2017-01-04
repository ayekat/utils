#include "field.h"

void
field_init(struct field *f)
{
	int unsigned i;

	for (i = 0; i < 9; ++i)
		f->options[i] = true;
	f->noptions = 9;
	f->value = 0;
}
