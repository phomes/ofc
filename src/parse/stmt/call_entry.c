#include "../parse.h"

unsigned parse_stmt_call(
	const sparse_t* src, const char* ptr,
	parse_stmt_t* stmt)
{
	unsigned i = parse_keyword(
		src, ptr, PARSE_KEYWORD_CALL);
	if (i == 0) return 0;

	unsigned len = parse_name(
		src, &ptr[i], &stmt->call_entry.name);
	if (len == 0) return 0;
	i += len;

	stmt->call_entry.args = NULL;
	if (ptr[i] == '(')
	{
		i += 1;

		stmt->call_entry.args = parse_call_arg_list(
			src, &ptr[i], &len);
		if (stmt->call_entry.args) i += len;

		if (ptr[i++] != ')')
		{
			parse_call_arg_list_delete(
				stmt->call_entry.args);
			return 0;
		}
	}

	stmt->type = PARSE_STMT_CALL;
	return i;
}

unsigned parse_stmt_entry(
	const sparse_t* src, const char* ptr,
	parse_stmt_t* stmt)
{
	unsigned i = parse_keyword(
		src, ptr, PARSE_KEYWORD_ENTRY);
	if (i == 0) return 0;

	unsigned len = parse_name(
		src, &ptr[i], &stmt->call_entry.name);
	if (len == 0) return 0;
	i += len;

	stmt->call_entry.args = NULL;
	if (ptr[i] == '(')
	{
		i += 1;

		stmt->call_entry.args = parse_call_arg_list(
			src, &ptr[i], &len);
		if (stmt->call_entry.args) i += len;

		if (ptr[i++] != ')')
		{
			parse_call_arg_list_delete(
				stmt->call_entry.args);
			return 0;
		}
	}

	stmt->type = PARSE_STMT_ENTRY;
	return i;
}

bool parse_stmt_call_entry_print(
	int fd, const parse_stmt_t* stmt)
{
	if (!stmt)
		return false;

	const char* kwstr;
	switch (stmt->type)
	{
		case PARSE_STMT_CALL:
			kwstr = "CALL";
			break;
		case PARSE_STMT_ENTRY:
			kwstr = "ENTRY";
			break;
		default:
			return false;
	}

	if (!dprintf_bool(fd, "%s ", kwstr)
		|| !str_ref_print(fd, stmt->call_entry.name)
		|| !dprintf_bool(fd, "("))
		return false;

	if (stmt->call_entry.args
		&& !parse_call_arg_list_print(
			fd, stmt->call_entry.args))
		return false;

	return dprintf_bool(fd, ")");
}
