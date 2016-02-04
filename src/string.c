
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
	do
	{
		zob_string_copy(*p_from, *p_to);
	} while(++p_from && ++p_to);
}

APP_ALLOC(string)
APP_CREATE(string)
APP_CLONE(string)
APP_DISPOSE(string)

void zob_string_init(struct zob_string * p_string)
{
	zob_buffer_create(&p_string->p_buffer);
	zob_buffer_set_is_auto(p_string->p_buffer, true);
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

void zob_string_ensure(struct zob_string * p_string, size_t from, size_t input_size)
{
	zob_buffer_ensure(p_string->p_buffer, from, input_size);
}

void zob_string_write(struct zob_string * p_string, size_t * p_from, const char * p_text)
{
	zob_buffer_write(p_string->p_buffer, p_from, p_text);
}

void zob_string_get(struct zob_string * p_string, char ** pp_text)
{
	zob_buffer_get(p_string->p_buffer, pp_text);
}

void zob_string_get_data(struct zob_string * p_string, size_t first, size_t last, char ** pp_data)
{
	// XXX memory leak!!!
	zob_buffer_get_data(p_string->p_buffer, first, last, pp_data);
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

