#include "../src/app.h"
#include "../src/string.h"

#include <assert.h>

void * malloc(size_t);
void free(void *);

int main(int argc, char *argv[])
{
	struct zob_app * p_app = NULL;
	zob_app_alloc(&p_app);
	zob_app_init(p_app, argc, argv);

	// Allocate an empty string
	{
		struct zob_string * p_string = NULL;
		zob_string_create(&p_string, p_app);
		assert(p_string);
		size_t size = 0;
		assert(0 == size);
		zob_string_dispose(&p_string);
		assert(NULL == p_string);
	}

	// Allocate an empty string and assign data
	{
		struct zob_string * p_string = NULL;
		zob_string_create(&p_string, p_app);
		assert(p_string);

		char const * p_message = "Alea jacta est";
		size_t size = 0;
		zob_string_write(p_string, &size, p_message);
		assert(strlen(p_message) == size);

		zob_string_dispose(&p_string);
		assert(NULL == p_string);
	}

	// Allocate an empty string, assign data and reset
	{
		struct zob_string * p_string = NULL;
		zob_string_create(&p_string, p_app);
		assert(p_string);

		char const * p_message = "Alea jacta est";
		size_t size = 0;
		zob_string_write(p_string, &size, p_message);
		assert(strlen(p_message) == size);

		zob_string_fill(p_string, 1024, '\0');

		zob_string_dispose(&p_string);
		assert(NULL == p_string);
	}

	// Allocate an empty string, assign data and search for a character
	{
		struct zob_string * p_string = NULL;
		zob_string_create(&p_string, p_app);
		assert(p_string);

		char const * p_message = "Alea jacta est";
		size_t size = 0;
		zob_string_write(p_string, &size, p_message);
		assert(strlen(p_message) == size);

		size_t position = 0;
		bool has_found = false;
		zob_string_find_char(p_string, 'j', 0, size, &position, &has_found);

		assert(true == has_found);
		assert(5 == position);

		zob_string_dispose(&p_string);
		assert(NULL == p_string);
	}

	// Allocate an empty string, assign data and search for a character
	{
		struct zob_string * p_string = NULL;
		zob_string_create(&p_string, p_app);
		assert(p_string);

		char const * p_message = "Alea jacta est";
		size_t size = 0;
		zob_string_write(p_string, &size, p_message);
		assert(strlen(p_message) == size);

		size_t position = 0;
		bool has_found = false;

		zob_string_find_char(p_string, 'j', 0, size, &position, &has_found);
		assert(true == has_found);
		assert(5 == position);

		zob_string_find_char(p_string, 'z', 0, size, &position, &has_found);
		assert(false == has_found);
		assert(size == position);

		position = 0;
		zob_string_find_string(p_string, "jacta", 0, size, &position, &has_found);
		assert(true == has_found);
		assert(5 == position);

		position = 0;
		zob_string_find_string(p_string, "jactaest", 0, size, &position, &has_found);
		assert(false == has_found);
		assert(size == position);

		position = 0;
		zob_string_find_string(p_string, "", 0, size, &position, &has_found);
		assert(false == has_found);
		assert(0 == position);

		{
			char * p_huge_needle = NULL;
			p_huge_needle = malloc(2048);
			memset(p_huge_needle, 'x', 2048);
			p_huge_needle[2047] = 0;

			position = 0;
			zob_string_find_string(p_string, "", 0, size, &position, &has_found);
			assert(false == has_found);
			assert(0 == position);

			free(p_huge_needle);
		}

		{
			char * p_huge_data = NULL;
			p_huge_data = malloc(2048);
			memset(p_huge_data, 'x', 2048);
			p_huge_data[2047] = 0;

			position = 0;
			zob_string_write(p_string, &position, p_huge_data);
			assert(false == has_found);
			assert(0 == position);

			free(p_huge_data);
		}

		zob_string_dispose(&p_string);
		assert(NULL == p_string);
	}

	printf("huge success\n");

	return 0;
}


