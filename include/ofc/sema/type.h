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

#ifndef __ofc_sema_type_h__
#define __ofc_sema_type_h__

typedef enum
{
	OFC_SEMA_TYPE_LOGICAL = 0,
	OFC_SEMA_TYPE_INTEGER,
	OFC_SEMA_TYPE_REAL,
	OFC_SEMA_TYPE_COMPLEX,
	OFC_SEMA_TYPE_BYTE,
	OFC_SEMA_TYPE_CHARACTER,
	OFC_SEMA_TYPE_STRUCTURE,
	OFC_SEMA_TYPE_POINTER,
	OFC_SEMA_TYPE_FUNCTION,
	OFC_SEMA_TYPE_SUBROUTINE,

	OFC_SEMA_TYPE_COUNT
} ofc_sema_type_e;

struct ofc_sema_type_s
{
	ofc_sema_type_e type;

	ofc_sema_array_t* array;

	union
	{
		const ofc_sema_type_t* subtype;

		struct
		{
			unsigned kind;
			unsigned len;
		};

		const ofc_sema_structure_t* structure;
	};
};

const ofc_sema_type_t* ofc_sema_type_spec(
	const ofc_sema_spec_t* spec);

const char* ofc_sema_type_str_rep(
	const ofc_sema_type_t* type);
const char* ofc_sema_type_str_cast_rep(
	const ofc_sema_type_t* type);

const ofc_sema_type_t* ofc_sema_type_create_primitive(
	ofc_sema_type_e type, unsigned kind);
const ofc_sema_type_t* ofc_sema_type_create_character(
	unsigned kind, unsigned len);
const ofc_sema_type_t* ofc_sema_type_create_structure(
	const ofc_sema_structure_t* structure);
const ofc_sema_type_t* ofc_sema_type_create_pointer(
	ofc_sema_type_t* target);
const ofc_sema_type_t* ofc_sema_type_create_array(
	const ofc_sema_type_t* type, const ofc_sema_array_t* array);
const ofc_sema_type_t* ofc_sema_type_create_function(
	const ofc_sema_type_t* type);

const ofc_sema_type_t* ofc_sema_type_star_len(
	const ofc_sema_type_t* type, unsigned star_len);

typedef const ofc_sema_type_t* (*ofc_sema_type_f)(void);

const ofc_sema_type_t* ofc_sema_type_logical_default(void);
const ofc_sema_type_t* ofc_sema_type_integer_default(void);
const ofc_sema_type_t* ofc_sema_type_real_default(void);
const ofc_sema_type_t* ofc_sema_type_double_default(void);
const ofc_sema_type_t* ofc_sema_type_complex_default(void);
const ofc_sema_type_t* ofc_sema_type_byte_default(void);
const ofc_sema_type_t* ofc_sema_type_subroutine(void);

uint8_t ofc_sema_type_hash(
	const ofc_sema_type_t* type);

bool ofc_sema_type_compare(
	const ofc_sema_type_t* a,
	const ofc_sema_type_t* b);

bool ofc_sema_type_size(
	const ofc_sema_type_t* type,
	unsigned* size);
bool ofc_sema_type_elem_count(
	const ofc_sema_type_t* type,
	unsigned* count);

bool ofc_sema_type_is_integer(const ofc_sema_type_t* type);
bool ofc_sema_type_is_scalar(const ofc_sema_type_t* type);
bool ofc_sema_type_is_logical(const ofc_sema_type_t* type);
bool ofc_sema_type_is_character(const ofc_sema_type_t* type);

bool ofc_sema_type_is_array(const ofc_sema_type_t* type);
bool ofc_sema_type_is_structure(const ofc_sema_type_t* type);
bool ofc_sema_type_is_composite(const ofc_sema_type_t* type);

bool ofc_sema_type_is_subroutine(const ofc_sema_type_t* type);
bool ofc_sema_type_is_function(const ofc_sema_type_t* type);
bool ofc_sema_type_is_procedure(const ofc_sema_type_t* type);

const ofc_sema_type_t* ofc_sema_type_base(const ofc_sema_type_t* type);

const ofc_sema_type_t* ofc_sema_type_promote(
	const ofc_sema_type_t* a,
	const ofc_sema_type_t* b);

bool ofc_sema_type_cast_is_lossless(
	const ofc_sema_type_t* base,
	const ofc_sema_type_t* target);

#endif
