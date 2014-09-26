
#include "buffer.h"
#include "app.h"
#include "object.h"

#include <stdlib.h>
#include <stdlib.h>
#include <assert.h>

struct db_app;

struct db_buffer
{
	struct db_app * p_app;

	// On demand, the buffer will be extended of chunck_size elements
	size_t chunk_size;

	// Growing strategy options
	bool is_auto: 1;	// Automatic grow

	// boundaries of allocated memory
	char * p_begin;
	char * p_end;
};

void _db_buffer_error(struct db_buffer * p_buffer, char * p_message, char * p_file, int line)
{
	db_app_error(p_buffer->p_app, p_message, p_file, line);
}

APP_ALLOC(buffer)
APP_CREATE(buffer)
APP_CLONE(buffer)
APP_DISPOSE(buffer)

void db_buffer_init(struct db_buffer * p_buffer, struct db_app * p_app)
{
	p_buffer->chunk_size = 1024;
	p_buffer->is_auto = false;
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

void db_buffer_set_is_auto(struct db_buffer * p_buffer, bool is_auto)
{
	p_buffer->is_auto = is_auto;
}

// Ensure we have enough room to write input_size elements from the from position and
// apply a linear grow algorithm, and fail if we can't grow.
void db_buffer_ensure(struct db_buffer * p_buffer, size_t from, size_t input_size)
{
	int available_size = 0;
	if(p_buffer->p_end > p_buffer->p_begin)
		available_size = p_buffer->p_end - (p_buffer->p_begin + from);

	if(available_size >= input_size)
		return;

	if(!p_buffer->is_auto)
	{
		_db_buffer_error(p_buffer, "Attempt to resize a fixed size buffer", __FILE__, __LINE__);
		return;
	}

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
		if(p_auto)
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
	int length = strlen(p_text);

	if(!length) db_app_error(p_buffer->p_app, "Empty write", __FILE__, __LINE__);

	db_buffer_ensure(p_buffer, *p_from, length);
	strcpy(p_buffer->p_begin + *p_from, p_text);
	if(p_from)
		*p_from += length;
}

void db_buffer_get(struct db_buffer * p_buffer, char ** pp_text)
{
	*pp_text = p_buffer->p_begin;
}

void db_buffer_get_data(struct db_buffer * p_buffer, size_t first, size_t last, char ** pp_string)
{
	assert(last > first);

	char * p_string = calloc(1 + last - first, sizeof *p_string);
	CHECK_NULL(p_buffer->p_app, p_string);

	strncpy(p_string, p_buffer->p_begin + first, last - first);

	*pp_string = p_string;
}

void db_buffer_size(struct db_buffer * p_buffer, size_t * p_size)
{
	*p_size = p_buffer->p_end - p_buffer->p_begin;
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

void db_buffer_find_string(
		struct db_buffer * p_buffer,
		char const * p_needle,
		size_t first,
		size_t last,
		size_t * p_position,
		bool * p_has_found
		)
{
	*p_position = first;

	size_t size = 0;
	db_buffer_size(p_buffer, &size);

	size_t needle_size = strlen(p_needle);
	if(0 == needle_size || needle_size > size)
	{
		*p_position = 0;
		*p_has_found = false;
		return;
	}

	do
	{
		char * p_cursor = p_buffer->p_begin + *p_position;
		char * p_cursor_end = NULL;

		if(*p_cursor == *p_needle)
		{
			char const * p_needle_end = p_needle;

			p_cursor_end = p_cursor;
			while(*++p_needle_end);
			{
				++*p_cursor_end;
				if(*p_cursor_end == *p_needle_end)
					continue;

				if(!*p_needle_end)
					*p_has_found = true;
				else
					*p_has_found = false;

				break;
			}

			if(true == *p_has_found)
				break;
		}

		if(p_cursor == p_buffer->p_begin + last)
		{
			*p_has_found = false;
			break;
		}
	}
	while(++*p_position);
}

void db_buffer_fill(struct db_buffer * p_buffer, size_t size, char value)
{
	db_buffer_ensure(p_buffer, 0, size);
	memset(p_buffer->p_begin, value, size);
}

