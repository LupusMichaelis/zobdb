
#include "buffer.h"
#include "app.h"
#include "object.h"

#include <stdlib.h>
#include <assert.h>

struct zob_app;

struct zob_buffer
{
	// On demand, the buffer will be extended of chunck_size elements
	size_t chunk_size;

	// Growing strategy options
	bool is_auto: 1;	// Automatic grow

	// boundaries of allocated memory
	char * p_begin;
	char * p_end;

	// The window on actual carried data
	size_t size;
};

void _zob_buffer_error(struct zob_buffer * p_buffer, char * p_message, char * p_file, int line)
{
	zob_app_error(gp_app, p_message, p_file, line);
}

APP_ALLOC(buffer)
APP_CREATE(buffer)
APP_CLONE(buffer)
APP_DISPOSE(buffer)

void zob_buffer_init(struct zob_buffer * p_buffer)
{
	p_buffer->chunk_size = 1024;
	p_buffer->is_auto = false;
}

void zob_buffer_clean(struct zob_buffer * p_buffer, bool has_to_dispose)
{
	struct zob_allocator * p_allocator = NULL;
	zob_app_allocator_get(gp_app, &p_allocator);

	if(has_to_dispose)
		if(NULL != p_buffer->p_begin)
			zob_allocator_do_release(p_allocator, (void **) &p_buffer->p_begin);

	memset(p_buffer, 0, sizeof *p_buffer);
	p_buffer->size = 0;
}

void zob_buffer_copy(struct zob_buffer * p_from, struct zob_buffer * p_to)
{
	p_to->chunk_size = p_from->chunk_size;

	size_t buffer_size = 0;
	zob_buffer_size_get(p_from, &buffer_size);
	zob_buffer_is_auto_set(p_to, true);
	zob_buffer_size_set(p_to, p_from->size);
	memcpy(p_to->p_begin, p_from->p_begin, p_from->size);
	zob_buffer_is_auto_set(p_to, p_from->is_auto);
}

void zob_buffer_is_auto_set(struct zob_buffer * p_buffer, bool is_auto)
{
	p_buffer->is_auto = is_auto;
}

void zob_buffer_create_from_memory(struct zob_buffer ** pp_buffer, void * p_content, size_t size)
{
	zob_buffer_create(pp_buffer);
	zob_buffer_is_auto_set(*pp_buffer, 1);
	zob_buffer_ensure(*pp_buffer, 0, size);
	zob_buffer_is_auto_set(*pp_buffer, 0);
	zob_buffer_write(*pp_buffer, 0, size, p_content, NULL);
}

// Ensure we have enough room to write input_size elements from the from position and
// apply a linear grow algorithm, and fail if we can't grow.
void zob_buffer_ensure(struct zob_buffer * p_buffer, size_t position, size_t input_size)
{
	if(!p_buffer->is_auto && position > p_buffer->size)
	{
		_zob_buffer_error(p_buffer, "Attempt to resize a fixed size buffer", __FILE__, __LINE__);
		abort();
	}

	int available_size = 0;
	if(p_buffer->p_end > p_buffer->p_begin)
		available_size = p_buffer->size - position;

	if(available_size >= input_size)
		return;

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
		struct zob_allocator * p_allocator = NULL;
		zob_app_allocator_get(gp_app, &p_allocator);
		char * p_auto = p_buffer->p_begin;
		zob_allocator_do_allocate(p_allocator, (void **) &p_buffer->p_begin, new_size * sizeof *p_buffer->p_begin);
		if(p_auto)
		{
			memset(p_buffer->p_begin, 0, new_size * sizeof *p_buffer->p_begin);
			memcpy(p_buffer->p_begin, p_auto, p_buffer->p_end - p_auto);
			zob_allocator_do_release(p_allocator, (void **) &p_auto);
		}
		else
			p_buffer->size = 0;
	}

	CHECK_NULL(p_buffer->p_begin);

	p_buffer->p_end = p_buffer->p_begin;
	p_buffer->p_end += chunk_number * p_buffer->chunk_size;
}

void zob_buffer_write(struct zob_buffer * p_buffer, size_t from, size_t length, const char * p_text, size_t * p_written)
{
	if(!p_buffer->is_auto)
		length = MIN(length, p_buffer->p_end - p_buffer->p_begin);

	if(!length) zob_app_error(gp_app, "Empty write", __FILE__, __LINE__);

	if(p_buffer->is_auto)
		zob_buffer_ensure(p_buffer, from, length);
	size_t buffer_size = 0;
	zob_buffer_size_get(p_buffer, &buffer_size);

	memcpy(p_buffer->p_begin + from, p_text, length);
	p_buffer->size = from + length;
	if(p_written)
		*p_written = length;
}

void zob_buffer_write_at(struct zob_buffer * p_buffer, size_t at, char item)
{
	assert(at < p_buffer->size);
	*(p_buffer->p_begin + at) = item;
}

void zob_buffer_slice_get(struct zob_buffer * p_buffer, size_t from, size_t to, struct zob_buffer ** pp_slice)
{
	zob_buffer_create(pp_slice);
	zob_buffer_is_auto_set(*pp_slice, 1);
	zob_buffer_ensure(*pp_slice, 0, to - from);
	zob_buffer_is_auto_set(*pp_slice, 0);
	zob_buffer_write(*pp_slice, 0, to - from, p_buffer->p_begin + from, NULL);
	zob_buffer_is_auto_set(*pp_slice, 1);
}

void zob_buffer_compare(struct zob_buffer * p_lhs, struct zob_buffer * p_rhs, int * p_diff)
{
	if(p_lhs->size != p_rhs->size)
	{
		*p_diff = p_rhs->size - p_lhs->size;
		return;
	}

	if(0 == p_lhs->size)
	{
		*p_diff = 0;
		return;
	}

	*p_diff = 0;
	char * lhs = NULL;
	char * rhs = NULL;
	do
	{
		lhs = p_lhs->p_begin + *p_diff;
		rhs = p_rhs->p_begin + *p_diff;
		if(*lhs != *rhs)
			break;

		++*p_diff;
	}
	while(*p_diff < p_rhs->size);

	*p_diff = *rhs - *lhs;
}


void zob_buffer_get(struct zob_buffer * p_buffer, char ** pp_text)
{
	*pp_text = p_buffer->p_begin;
}

void zob_buffer_get_data(struct zob_buffer * p_buffer, size_t first, size_t last, char ** pp_string)
{
	assert(last > first);

	char * p_string = calloc(1 + last - first, sizeof *p_string);
	CHECK_NULL(p_string);

	strncpy(p_string, p_buffer->p_begin + first, last - first);

	*pp_string = p_string;
}

void zob_buffer_size_set(struct zob_buffer * p_buffer, size_t size)
{
	zob_buffer_ensure(p_buffer, 0, size);
	p_buffer->size = size;
}

void zob_buffer_size_get(struct zob_buffer * p_buffer, size_t * p_size)
{
	*p_size = p_buffer->size;
}

void zob_buffer_find_char(
		struct zob_buffer * p_buffer,
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

void zob_buffer_find_string(
		struct zob_buffer * p_buffer,
		char const * p_needle,
		size_t first,
		size_t last,
		size_t * p_position,
		bool * p_has_found
		)
{
	*p_position = first;

	size_t size = 0;
	zob_buffer_size_get(p_buffer, &size);

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
			while(*++p_needle_end)
			{
				++p_cursor_end;
				if(*p_cursor_end != *p_needle_end)
					break;
			}

			*p_has_found = '\0' == *p_needle_end ? true : false;

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

void zob_buffer_fill(struct zob_buffer * p_buffer, size_t size, char value)
{
	zob_buffer_ensure(p_buffer, 0, size);
	memset(p_buffer->p_begin, value, size);
}

