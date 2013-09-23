
#include "buffer.h"
#include "app.h"

#include <stdlib.h>
#include <stdlib.h>

struct db_app;

struct db_buffer
{
	struct db_app * p_app;

	// On demand, the buffer will be extended of chunck_size elements
	size_t chunk_size;

	// boundaries of allocated memory
	char * p_begin;
	char * p_end;
};

void db_buffer_create(struct db_buffer ** pp_buffer, struct db_app * p_app)
{
	struct db_buffer * p_b = NULL;
	p_b = calloc(1, sizeof *p_b);
	CHECK_NULL(p_app, p_b);

	p_b->chunk_size = 1024;

	p_b->p_app = p_app;
	*pp_buffer = p_b;
}

void db_buffer_dispose(struct db_buffer ** pp_buffer)
{
	if(NULL != (*pp_buffer)->p_begin)
		free((*pp_buffer)->p_begin);

	free(*pp_buffer);
	*pp_buffer = NULL;
}

void db_buffer_ensure(struct db_buffer * p_buffer, size_t from, size_t input_size)
{
	int available_size = p_buffer->p_end - (p_buffer->p_begin + from);

	if(available_size < input_size)
	{
		int current_size = p_buffer->p_end - p_buffer->p_begin;
		// We compute the memory actually requested, and count how many chunks are needed
		// plus an extra chunk if there is remain.
		int chunk_number = (input_size - available_size) / p_buffer->chunk_size;
		chunk_number += (input_size - available_size) % p_buffer->chunk_size ? 1 : 0;
		int new_size = current_size + chunk_number * p_buffer->chunk_size;

		// We apply a linear grow algorithm
		p_buffer->p_begin = realloc(p_buffer->p_begin, new_size * sizeof *p_buffer->p_begin);
		CHECK_NULL(p_buffer->p_app, p_buffer->p_begin);

		if(p_buffer->p_end == NULL)
			p_buffer->p_end = p_buffer->p_begin;

		p_buffer->p_end += chunk_number * p_buffer->chunk_size;
	}
}

void db_buffer_write(struct db_buffer * p_buffer, size_t * p_from, const char * p_text)
{
	db_buffer_ensure(p_buffer, *p_from, strlen(p_text));

	do
		*(p_buffer->p_begin + (*p_from)++) = *p_text;
	while(*++p_text);
}

void db_buffer_get(struct db_buffer * p_buffer, char ** pp_text)
{
	*pp_text = p_buffer->p_begin;
}

void db_buffer_get_string(struct db_buffer * p_buffer, size_t first, size_t last, char ** pp_string)
{
	char * p_string = calloc(1 + last - first, sizeof *p_string);
	CHECK_NULL(p_buffer->p_app, p_string);

	strncpy(p_string, p_buffer->p_begin + first, last - first);

	*pp_string = p_string;
}

void db_buffer_find_char(
		struct db_buffer * p_db,
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
		char * p_cursor = p_db->p_begin + *p_position;

		if(*p_cursor ==  needle)
			*p_has_found = true;
		else if(p_cursor == p_db->p_begin + last)
			*p_has_found = false;
		else
			continue;

		break;
	}
	while(++*p_position);
}


