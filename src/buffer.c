
#include "buffer.h"
#include "app.h"
#include "object.h"

#include <stdlib.h>
#include <stdlib.h>

struct db_app;

struct db_buffer
{
	struct db_app * p_app;

	// On demand, the buffer will be extended of chunck_size elements
	size_t chunk_size;

	// Growing strategy options
	bool is_auto: 1;
	bool is_fixed: 1;

	// boundaries of allocated memory
	char * p_begin;
	char * p_end;
};

APP_ALLOC(buffer)
APP_CREATE(buffer)
APP_CLONE(buffer)
APP_DISPOSE(buffer)

void db_buffer_init(struct db_buffer * p_buffer, struct db_app * p_app)
{
	p_buffer->chunk_size = 1024;
	p_buffer->is_auto = false;
	p_buffer->is_fixed = true;
	p_buffer->p_app = p_app;
}

void db_buffer_clean(struct db_buffer * p_buffer, bool has_to_dispose)
{
	if(has_to_dispose)
		if(NULL != p_buffer->p_begin)
			free(p_buffer->p_begin);

	memset(p_buffer, 0, sizeof *p_buffer);
}

void db_buffer_copy(struct db_buffer * p_from, struct db_buffer * p_to)
{
	p_to->p_app = p_from->p_app;
	p_to->chunk_size = p_from->chunk_size;

	p_to->p_begin = calloc(p_from->p_end - p_from->p_begin, sizeof *p_to->p_begin);
	strncpy(p_to->p_begin, p_from->p_begin, p_from->p_end - p_from->p_begin);
}

void db_buffer_set_is_fixed_size(struct db_buffer * p_buffer, bool is_fixed)
{
	p_buffer->is_fixed = is_fixed;
}

void db_buffer_set_is_auto(struct db_buffer * p_buffer, bool is_auto)
{
	p_buffer->is_auto = is_auto;
}

// Ensure we have enough room to write input_size elements from the from position and
// apply a linear grow algorithm, and fail if we can't grow.
void db_buffer_ensure(struct db_buffer * p_buffer, size_t from, size_t input_size)
{
	int available_size = p_buffer->p_end - (p_buffer->p_begin + from);

	if(available_size >= input_size)
		return;

	if(p_buffer->is_fixed)
		db_app_error(p_buffer->p_app, "Attempt to resize a fixed size buffer", __FILE__, __LINE__);

	int current_size = p_buffer->p_end - p_buffer->p_begin;
	// We compute the memory actually requested, and count how many chunks are needed
	// plus an extra chunk if there is remain.
	int chunk_number = (input_size - available_size) / p_buffer->chunk_size;
	chunk_number += (input_size - available_size) % p_buffer->chunk_size ? 1 : 0;
	int new_size = current_size + chunk_number * p_buffer->chunk_size;

	// If p_begin point an automatic variable, we have to allocate a brand new memory
	// chunk
	if(p_buffer->is_auto)
	{
		char * p_auto = p_buffer->p_begin;
		p_buffer->p_begin = malloc(new_size * sizeof *p_buffer->p_begin);
		strcpy(p_buffer->p_begin, p_auto);
		p_buffer->is_auto = false;
	}
	else
	{
		p_buffer->p_begin = realloc(p_buffer->p_begin, new_size * sizeof *p_buffer->p_begin);
	}
	CHECK_NULL(p_buffer->p_app, p_buffer->p_begin);

	if(p_buffer->p_end == NULL)
		p_buffer->p_end = p_buffer->p_begin;

	p_buffer->p_end += chunk_number * p_buffer->chunk_size;
}

void db_buffer_write(struct db_buffer * p_buffer, size_t * p_from, const char * p_text)
{
	db_buffer_ensure(p_buffer, *p_from, strlen(p_text));
	strcpy(p_buffer->p_begin + *p_from, p_text);
	*p_from += strlen(p_text);
}

void db_buffer_get(struct db_buffer * p_buffer, char ** pp_text)
{
	*pp_text = p_buffer->p_begin;
}

void db_buffer_get_data(struct db_buffer * p_buffer, size_t first, size_t last, char ** pp_string)
{
	char * p_string = calloc(1 + last - first, sizeof *p_string);
	CHECK_NULL(p_buffer->p_app, p_string);

	strncpy(p_string, p_buffer->p_begin + first, last - first);

	*pp_string = p_string;
}

void db_buffer_find_char(
		struct db_buffer * p_buffer,
		char needle,
		size_t first,
		size_t last,
		size_t * p_position,
		bool * p_has_found
		)
{
	*p_position = first;

	do
	{
		char * p_cursor = p_buffer->p_begin + *p_position;

		if(*p_cursor ==  needle)
			*p_has_found = true;
		else if(p_cursor == p_buffer->p_begin + last)
			*p_has_found = false;
		else
			continue;

		break;
	}
	while(++*p_position);
}


