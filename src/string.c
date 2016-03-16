
#include "string.h"
#include "buffer.h"
#include "app.h"
#include "object.h"
#include "object-vector.h"

#include <stdlib.h>
#include <stdlib.h>

struct zob_app;

struct zob_string
{
	struct zob_buffer * p_buffer;
};

APP_VECTOR_ALLOC(string)
APP_VECTOR_CREATE(string)
APP_VECTOR_CLONE(string)
APP_VECTOR_COPY(string)
APP_VECTOR_DISPOSE(string)
APP_VECTOR_CLEAN(string)

void zob_vector_string_copy(struct zob_string ** p_from, struct zob_string ** p_to)
{
	do zob_string_copy(*p_from, *p_to);
	while(++p_from && ++p_to);
}

APP_ALLOC(string)
APP_CREATE(string)
APP_CLONE(string)
APP_DISPOSE(string)

void zob_string_init(struct zob_string * p_string)
{
	zob_buffer_create(&p_string->p_buffer);
	zob_buffer_is_auto_set(p_string->p_buffer, true);
}

void zob_string_create_from_cstring(struct zob_string ** pp_string, char * p_raw)
{
	zob_string_create(pp_string);
	zob_string_set(*pp_string, p_raw);
}

void zob_string_clean(struct zob_string * p_string, bool has_to_dispose)
{
	if(has_to_dispose)
		if(NULL != p_string->p_buffer)
			zob_buffer_dispose(&p_string->p_buffer);

	memset(p_string, 0, sizeof *p_string);
}

void zob_string_copy(struct zob_string * p_from, struct zob_string * p_to)
{
	zob_buffer_copy(p_from->p_buffer, p_to->p_buffer);
}

void zob_string_compare(struct zob_string * p_lhs, struct zob_string * p_rhs, int * p_diff)
{
	zob_buffer_compare(p_lhs->p_buffer, p_rhs->p_buffer, p_diff);
}

void zob_string_size_set(struct zob_string * p_string, size_t input_size)
{
	zob_buffer_size_set(p_string->p_buffer, input_size);
}

void zob_string_size_get(struct zob_string * p_string, size_t * p_input_size)
{
	zob_buffer_size_get(p_string->p_buffer, p_input_size);
}

void zob_string_write(struct zob_string * p_string, size_t from, const char * p_text, size_t * p_written)
{
	zob_buffer_write(p_string->p_buffer, from, strlen(p_text), p_text, p_written);
}

void zob_string_set(struct zob_string * p_string, char * p_text)
{
	size_t length = strlen(p_text);
	zob_string_write(p_string, 0, p_text, &length);
	zob_buffer_size_set(p_string->p_buffer, length);
}

void zob_string_get(struct zob_string * p_string, char ** pp_text)
{
	zob_buffer_get(p_string->p_buffer, pp_text);
}

void zob_string_slice_get(struct zob_string * p_string, size_t first, size_t last, struct zob_string ** pp_slice)
{
	struct zob_buffer * p_slice = NULL;
	zob_buffer_slice_get(p_string->p_buffer, first, last, &p_slice);

	zob_string_create(pp_slice);
	(*pp_slice)->p_buffer = p_slice;
}

void zob_string_find_char(
		struct zob_string * p_string,
		char needle,
		size_t first,
		size_t last,
		size_t * p_position,
		bool * p_has_found
		)
{
	zob_buffer_find_char(p_string->p_buffer, needle, first, last, p_position, p_has_found);
}

void zob_string_find_string(
		struct zob_string * p_string,
		char const * p_needle,
		size_t first,
		size_t last,
		size_t * p_position,
		bool * p_has_found
		)
{
	zob_buffer_find_string(p_string->p_buffer, p_needle, first, last, p_position, p_has_found);
}


void zob_string_fill(struct zob_string * p_string, size_t size, char value)
{
	zob_buffer_fill(p_string->p_buffer, size, value);
}

