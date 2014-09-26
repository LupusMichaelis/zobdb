
#include "string.h"
#include "buffer.h"
#include "app.h"
#include "object.h"
#include "object-vector.h"

#include <stdlib.h>
#include <stdlib.h>

struct db_app;

struct db_string
{
	struct db_app * p_app;
	struct db_buffer * p_buffer;
};

APP_VECTOR_ALLOC(string)
APP_VECTOR_CREATE(string)
APP_VECTOR_CLONE(string)
APP_VECTOR_DISPOSE(string)

APP_ALLOC(string)
APP_CREATE(string)
APP_CLONE(string)
APP_DISPOSE(string)

void db_string_init(struct db_string * p_string, struct db_app * p_app)
{
	p_string->p_app = p_app;
	db_buffer_create(&p_string->p_buffer, p_app);
	db_buffer_set_is_auto(p_string->p_buffer, true);
}

void db_string_clean(struct db_string * p_string, bool has_to_dispose)
{
	if(has_to_dispose)
		if(NULL != p_string->p_buffer)
			db_buffer_dispose(&p_string->p_buffer);

	memset(p_string, 0, sizeof *p_string);
}

void db_string_copy(struct db_string * p_from, struct db_string * p_to)
{
	p_to->p_app = p_from->p_app;
	db_buffer_copy(p_from->p_buffer, p_to->p_buffer);
}

void db_string_ensure(struct db_string * p_string, size_t from, size_t input_size)
{
	db_buffer_ensure(p_string->p_buffer, from, input_size);
}

void db_string_write(struct db_string * p_string, size_t * p_from, const char * p_text)
{
	db_buffer_write(p_string->p_buffer, p_from, p_text);
}

void db_string_get(struct db_string * p_string, char ** pp_text)
{
	db_buffer_get(p_string->p_buffer, pp_text);
}

void db_string_get_data(struct db_string * p_string, size_t first, size_t last, char ** pp_data)
{
	// XXX memory leak!!!
	db_buffer_get_data(p_string->p_buffer, first, last, pp_data);
}

void db_string_find_char(
		struct db_string * p_string,
		char needle,
		size_t first,
		size_t last,
		size_t * p_position,
		bool * p_has_found
		)
{
	db_buffer_find_char(p_string->p_buffer, needle, first, last, p_position, p_has_found);
}

void db_string_find_string(
		struct db_string * p_string,
		char const * p_needle,
		size_t first,
		size_t last,
		size_t * p_position,
		bool * p_has_found
		)
{
	db_buffer_find_string(p_string->p_buffer, p_needle, first, last, p_position, p_has_found);
}


void db_string_fill(struct db_string * p_string, size_t size, char value)
{
	db_buffer_fill(p_string->p_buffer, size, value);
}

