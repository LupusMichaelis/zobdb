#include "../src/app.h"
#include "../src/buffer.h"

#include <assert.h>

void * malloc(size_t);
void free(void *);

int main(int argc, char *argv[])
{
	struct db_app * p_app = NULL;
	db_app_alloc(&p_app);
	db_app_init(p_app, argc, argv);

	// Allocate an empty buffer
	{
		struct db_buffer * p_buffer = NULL;
		db_buffer_create(&p_buffer, p_app);
		assert(p_buffer);
		size_t size = 0;
		assert(0 == size);
		db_buffer_dispose(&p_buffer);
		assert(NULL == p_buffer);
	}

	// Allocate an empty buffer and assign data
	{
		struct db_buffer * p_buffer = NULL;
		db_buffer_create(&p_buffer, p_app);
		assert(p_buffer);

		char const * p_message = "Alea jacta est";
		db_buffer_set_is_auto(p_buffer, true);
		size_t size = 0;
		db_buffer_write(p_buffer, &size, p_message);
		assert(strlen(p_message) == size);

		db_buffer_dispose(&p_buffer);
		assert(NULL == p_buffer);
	}

	// Allocate an empty buffer, assign data and reset
	{
		struct db_buffer * p_buffer = NULL;
		db_buffer_create(&p_buffer, p_app);
		assert(p_buffer);

		char const * p_message = "Alea jacta est";
		db_buffer_set_is_auto(p_buffer, true);
		size_t size = 0;
		db_buffer_write(p_buffer, &size, p_message);
		assert(strlen(p_message) == size);

		db_buffer_fill(p_buffer, 1024, '\0');

		db_buffer_dispose(&p_buffer);
		assert(NULL == p_buffer);
	}

	// Allocate an empty buffer, assign data and search for a character
	{
		struct db_buffer * p_buffer = NULL;
		db_buffer_create(&p_buffer, p_app);
		assert(p_buffer);

		char const * p_message = "Alea jacta est";
		db_buffer_set_is_auto(p_buffer, true);
		size_t size = 0;
		db_buffer_write(p_buffer, &size, p_message);
		assert(strlen(p_message) == size);

		size_t position = 0;
		bool has_found = false;
		db_buffer_find_char(p_buffer, 'j', 0, size, &position, &has_found);

		assert(true == has_found);
		assert(5 == position);

		db_buffer_dispose(&p_buffer);
		assert(NULL == p_buffer);
	}

	// Allocate an empty buffer, assign data and search for a character
	{
		struct db_buffer * p_buffer = NULL;
		db_buffer_create(&p_buffer, p_app);
		assert(p_buffer);

		char const * p_message = "Alea jacta est";
		db_buffer_set_is_auto(p_buffer, true);
		size_t size = 0;
		db_buffer_write(p_buffer, &size, p_message);
		assert(strlen(p_message) == size);

		size_t position = 0;
		bool has_found = false;

		db_buffer_find_char(p_buffer, 'j', 0, size, &position, &has_found);
		assert(true == has_found);
		assert(5 == position);

		db_buffer_find_char(p_buffer, 'z', 0, size, &position, &has_found);
		assert(false == has_found);
		assert(size == position);

		position = 0;
		db_buffer_find_string(p_buffer, "jacta", 0, size, &position, &has_found);
		assert(true == has_found);
		assert(5 == position);

		position = 0;
		db_buffer_find_string(p_buffer, "jactaest", 0, size, &position, &has_found);
		assert(false == has_found);
		assert(size == position);

		position = 0;
		db_buffer_find_string(p_buffer, "", 0, size, &position, &has_found);
		assert(false == has_found);
		assert(0 == position);

		{
			char * p_huge_needle = NULL;
			p_huge_needle = malloc(2048);
			memset(p_huge_needle, 'x', 2048);
			p_huge_needle[2047] = 0;

			position = 0;
			db_buffer_find_string(p_buffer, "", 0, size, &position, &has_found);
			assert(false == has_found);
			assert(0 == position);

			free(p_huge_needle);
		}

		db_buffer_dispose(&p_buffer);
		assert(NULL == p_buffer);
	}

	printf("huge success\n");

	return 0;
}

