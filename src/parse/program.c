#include "parse.h"
#include <string.h>
#include <ctype.h>



static bool parse_program_add_stmt(
	parse_program_t* program, parse_stmt_t stmt)
{
	if (!program)
		return false;

	parse_stmt_t* nstmt
		= (parse_stmt_t*)realloc(program->stmt,
			(sizeof(parse_stmt_t) * (program->stmt_count + 1)));
	if (!nstmt) return false;

	program->stmt = nstmt;
	program->stmt[program->stmt_count++] = stmt;
	return true;
}

unsigned parse_program(
	const sparse_t* src, const char* ptr,
	const label_table_t* labels,
	parse_program_t* program)
{
	program->name = STR_REF_EMPTY;
	unsigned i = parse_keyword_name(
		src, ptr, PARSE_KEYWORD_PROGRAM, &program->name);
	if (i == 0) return 0;

	if (str_ref_empty(program->name))
	{
		sparse_error(src, ptr, "Expected name in PROGRAM statement");
		return 0;
	}

	if ((ptr[i] != '\n')
		&& (ptr[i] != '\r'))
	{
		sparse_error(src, &ptr[i],
			"Expected newline after PROGRAM statement");
		return 0;
	}
	i += 1;

	program->implicit = PARSE_IMPLICIT_DEFAULT;

	lang_opts_t opts = sparse_lang_opts(src);
	program->decl = hashmap_create(
		(void*)(opts.case_sensitive
			? parse_decl_hash
			: parse_decl_hash_ci),
		(void*)(opts.case_sensitive
			? parse_decl_key_compare
			: parse_decl_key_compare_ci),
		(void*)parse_decl_key,
		(void*)parse_decl_delete);
	if (!program->decl)
		return 0;

	program->stmt_count = 0;
	program->stmt       = NULL;

	unsigned len;
	while (true)
	{
		const char* fptr = sparse_file_pointer(src, &ptr[i]);
		if (!fptr)
		{
			/* This should never happen, should always have a
			   corresponding file pointer. */
			parse_program_cleanup(*program);
			return 0;
		}

		unsigned label = 0;
		bool has_label = label_table_find(
			labels, fptr, &label);

		{
			len = parse_implicit(
				src, &ptr[i], &program->implicit);
			if (len > 0)
			{
				if (has_label)
				{
					sparse_warning(src, &ptr[i],
						"Ignoring label on implicit statement");
				}

				i += len;
				continue;
			}
		}

		{
			len = parse_dimension(
				src, &ptr[i], &program->implicit, program->decl);
			if (len > 0)
			{
				if (has_label)
				{
					sparse_warning(src, &ptr[i],
						"Ignoring label on DIMENSION statement");
				}

				i += len;
				continue;
			}
		}

		{
			len = parse_decl(
				src, &ptr[i],
				program->decl);

			if (len > 0)
			{
				if (has_label)
				{
					sparse_warning(src, &ptr[i],
						"Ignoring label on declaration");
				}

				i += len;
				continue;
			}
		}

		{
			parse_stmt_t stmt;
			len = parse_stmt(
				src, &ptr[i], (has_label ? &label : NULL),
				&program->implicit, program->decl, &stmt);

			if (len > 0)
			{
				if (stmt.type == PARSE_STMT_EMPTY)
				{
					sparse_warning(src, &ptr[i],
						"Empty statement");
				}
				else if (!parse_program_add_stmt(program, stmt))
				{
					/* This should never happen, likely out of memory. */
					parse_program_cleanup(*program);
					return 0;
				}
				i += len;
				continue;
			}
		}

		break;
	}

	str_ref_t end_name = STR_REF_EMPTY;
	len = parse_keyword_name(
		src, &ptr[i], PARSE_KEYWORD_END_PROGRAM, &end_name);
	if (len == 0)
	{
		sparse_error(src, &ptr[i],
			"Expected END PROGRAM");
		parse_program_cleanup(*program);
		return 0;
	}

	if (!str_ref_empty(end_name)
		&& (opts.case_sensitive
			? !str_ref_equal(program->name, end_name)
			: !str_ref_equal_ci(program->name, end_name)))
	{
		sparse_warning(src, &ptr[i],
			"END PROGRAM name '%.*s' doesn't match PROGRAM name '%.*s'",
			end_name.size, end_name.base,
			program->name.size, program->name.base);
	}
	i += len;

	if (ptr[i] != '\0')
	{
		sparse_error(src, &ptr[i],
			"Expected end of input after main program");
		parse_program_cleanup(*program);
		return 0;
	}

	return i;
}

void parse_program_cleanup(
	parse_program_t program)
{
	unsigned i;
	for (i = 0; i < program.stmt_count; i++)
		parse_stmt_cleanup(program.stmt[i]);
	free(program.stmt);

	hashmap_delete(program.decl);
}
