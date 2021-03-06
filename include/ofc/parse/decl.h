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

#ifndef __ofc_parse_decl_h__
#define __ofc_parse_decl_h__

typedef struct
{
	ofc_parse_lhs_t*   lhs;

	ofc_parse_expr_t*  init_expr;
	ofc_parse_clist_t* init_clist;
} ofc_parse_decl_t;

typedef struct
{
	unsigned           count;
	ofc_parse_decl_t** decl;
} ofc_parse_decl_list_t;


ofc_parse_decl_t* ofc_parse_decl(
	const ofc_sparse_t* src, const char* ptr,
	ofc_parse_debug_t* debug,
	unsigned* len);
void ofc_parse_decl_delete(
	ofc_parse_decl_t* decl);
bool ofc_parse_decl_print(
	ofc_colstr_t* cs, const ofc_parse_decl_t* decl);

ofc_parse_decl_list_t* ofc_parse_decl_list(
	const ofc_sparse_t* src, const char* ptr,
	ofc_parse_debug_t* debug,
	unsigned* len);
void ofc_parse_decl_list_delete(
	ofc_parse_decl_list_t* decl);
bool ofc_parse_decl_list_print(
	ofc_colstr_t* cs, const ofc_parse_decl_list_t* list);

#endif
