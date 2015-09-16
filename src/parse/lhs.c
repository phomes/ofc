#include "parse.h"

unsigned parse_lhs(
	const sparse_t* src, const char* ptr,
	parse_lhs_t* lhs)
{
	unsigned len = parse_name(
		src, ptr, &lhs->variable);
	if (len == 0) return 0;

	/* TODO - Implement more LHS types. */

	lhs->type = PARSE_LHS_VARIABLE;
	return len;
}

void parse_lhs_cleanup(
	parse_lhs_t lhs)
{
	switch (lhs.type)
	{
		case PARSE_LHS_VARIABLE:
			break;
		default:
			if (lhs.parent)
				parse_lhs_cleanup(*lhs.parent);
			free(lhs.parent);
			break;
	}
}