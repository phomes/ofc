/* Copyright 2015 Codethink Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __ofc_sema_decl_h__
#define __ofc_sema_decl_h__

#include <ofc/hashmap.h>

struct ofc_sema_decl_s
{
	const ofc_sema_type_t* type;
	ofc_str_ref_t          name;
	ofc_sema_scope_t*      func;

	union
	__attribute__((__packed__))
	{
		ofc_sema_typeval_t*  init;
		ofc_sema_typeval_t** init_array;
	};

    bool is_static;
	bool is_automatic;
	bool is_volatile;
	bool is_intrinsic;
	bool is_external;

	bool is_target;

	ofc_sema_equiv_t* equiv;

	bool used;
};

struct ofc_sema_decl_list_s
{
	bool case_sensitive;
	bool is_ref;

	unsigned count;

	union
	__attribute__((__packed__))
	{
		ofc_sema_decl_t**       decl;
		const ofc_sema_decl_t** decl_ref;
	};

	ofc_hashmap_t* map;
};

ofc_sema_decl_t* ofc_sema_decl_create(
	const ofc_sema_type_t* type,
	ofc_str_ref_t name);

ofc_sema_decl_t* ofc_sema_decl_spec(
	ofc_sema_scope_t*       scope,
	ofc_str_ref_t           name,
	const ofc_sema_spec_t*  spec,
	const ofc_sema_array_t* array);
ofc_sema_decl_t* ofc_sema_decl_implicit(
	ofc_sema_scope_t*       scope,
	ofc_str_ref_t           name,
	const ofc_sema_array_t* array);
ofc_sema_decl_t* ofc_sema_decl_implicit_lhs(
	ofc_sema_scope_t*       scope,
	const ofc_parse_lhs_t*  lhs);

ofc_sema_decl_t* ofc_sema_decl_function(
	ofc_sema_scope_t*       scope,
	ofc_str_ref_t           name,
	const ofc_sema_spec_t*  spec);
ofc_sema_decl_t* ofc_sema_decl_subroutine(
	ofc_sema_scope_t* scope,
	ofc_str_ref_t     name);

bool ofc_sema_decl(
	ofc_sema_scope_t* scope,
	const ofc_parse_stmt_t* stmt);
void ofc_sema_decl_delete(
	ofc_sema_decl_t* decl);

bool ofc_sema_decl_init(
	const ofc_sema_scope_t* scope,
	ofc_sema_decl_t* decl,
	const ofc_sema_expr_t* init);
bool ofc_sema_decl_init_offset(
	const ofc_sema_scope_t* scope,
	ofc_sema_decl_t* decl,
	unsigned offset,
	const ofc_sema_expr_t* init);
bool ofc_sema_decl_init_array(
	const ofc_sema_scope_t* scope,
	ofc_sema_decl_t* decl,
	const ofc_sema_array_t* array,
	unsigned count,
	const ofc_sema_expr_t** init);
bool ofc_sema_decl_init_func(
	ofc_sema_decl_t* decl,
	ofc_sema_scope_t* func);

bool ofc_sema_decl_size(
	const ofc_sema_decl_t* decl,
	unsigned* size);
bool ofc_sema_decl_elem_count(
	const ofc_sema_decl_t* decl,
	unsigned* count);

bool ofc_sema_decl_is_array(
	const ofc_sema_decl_t* decl);
bool ofc_sema_decl_is_composite(
	const ofc_sema_decl_t* decl);

bool ofc_sema_decl_is_subroutine(
	const ofc_sema_decl_t* decl);
bool ofc_sema_decl_is_function(
	const ofc_sema_decl_t* decl);
bool ofc_sema_decl_is_procedure(
	const ofc_sema_decl_t* decl);

bool ofc_sema_decl_has_initializer(
	const ofc_sema_decl_t* decl);

const ofc_sema_type_t* ofc_sema_decl_type(
	const ofc_sema_decl_t* decl);
const ofc_sema_type_t* ofc_sema_decl_base_type(
	const ofc_sema_decl_t* decl);

bool ofc_sema_decl_equiv(
	ofc_sema_decl_t* a,
	ofc_sema_decl_t* b);


ofc_sema_decl_list_t* ofc_sema_decl_list_create(bool case_sensitive);
ofc_sema_decl_list_t* ofc_sema_decl_list_create_ref(bool case_sensitive);
void ofc_sema_decl_list_delete(ofc_sema_decl_list_t* list);

bool ofc_sema_decl_list_add(
	ofc_sema_decl_list_t* list,
	ofc_sema_decl_t* decl);
bool ofc_sema_decl_list_add_ref(
	ofc_sema_decl_list_t* list,
	const ofc_sema_decl_t* decl);

const ofc_sema_decl_t* ofc_sema_decl_list_find(
	const ofc_sema_decl_list_t* list,
	ofc_str_ref_t name);
ofc_sema_decl_t* ofc_sema_decl_list_find_modify(
	ofc_sema_decl_list_t* list,
	ofc_str_ref_t name);

const ofc_hashmap_t* ofc_sema_decl_list_map(
	const ofc_sema_decl_list_t* list);

bool ofc_sema_decl_print(ofc_colstr_t* cs, bool print_type,
	const ofc_sema_decl_t* decl);

bool ofc_sema_decl_list_print(
	ofc_colstr_t* cs, unsigned indent,
	const ofc_sema_decl_list_t* decl_list);

#endif
