#include "../src/buffer.h"
#include "../src/string.h"

#include <assert.h>
#include <cgreen/cgreen.h>

#include "mock.h"

Describe(string);

BeforeEach(string)
{
}

AfterEach(string)
{
}

Ensure(string, create_empty)
{
	// Allocate an empty string
	struct zob_string * p_string = NULL;
	zob_string_create(&p_string);
	assert_that(p_string, is_not_equal_to(NULL));
	size_t size = 0;
	zob_string_size_get(p_string, &size);
	assert_that(size, is_equal_to(0));
	zob_string_dispose(&p_string);
	assert_that(p_string, is_equal_to(NULL));
}

Ensure(string, create_and_assign)
{
	// Allocate an empty string and assign data
	struct zob_string * p_string = NULL;
	zob_string_create(&p_string);
	assert_that(p_string, is_not_equal_to(NULL));

	char const * p_message = "Alea jacta est";
	size_t size = 0;
	zob_string_write(p_string, size, p_message, &size);
	assert_that(strlen(p_message), is_equal_to(size));

	zob_string_dispose(&p_string);
	assert_that(p_string, is_equal_to(NULL));
}

Ensure(string, create_and_assign_then_reset)
{
	// Allocate an empty string, assign data and reset
	struct zob_string * p_string = NULL;
	zob_string_create(&p_string);
	assert_that(p_string, is_not_equal_to(NULL));

	char const * p_message = "Alea jacta est";
	size_t size = 0;
	zob_string_write(p_string, size, p_message, &size);
	assert_that(strlen(p_message), is_equal_to(size));

	zob_string_fill(p_string, 1024, '\0');

	zob_string_dispose(&p_string);
	assert_that(p_string, is_equal_to(NULL));
}

Ensure(string, create_and_assign_then_look_for_char)
{
	// Allocate an empty string, assign data and search for a character
	struct zob_string * p_string = NULL;
	zob_string_create(&p_string);
	assert_that(p_string, is_not_equal_to(NULL));

	char const * p_message = "Alea jacta est";
	size_t size = 0;
	zob_string_write(p_string, size, p_message, &size);
	assert_that(strlen(p_message), is_equal_to(size));

	size_t position = 0;
	bool has_found = false;
	zob_string_find_char(p_string, 'j', 0, size, &position, &has_found);

	assert_that(has_found, is_true);
	assert_that(position, is_equal_to(5));

	zob_string_dispose(&p_string);
	assert_that(p_string, is_equal_to(NULL));
}

Ensure(string, create_and_assign_then_look_for_chars)
{
	// Allocate an empty string, assign data and search for a character
	struct zob_string * p_string = NULL;
	zob_string_create(&p_string);
	assert_that(p_string, is_not_equal_to(NULL));

	char const * p_message = "Alea jacta est";
	size_t size = 0;
	zob_string_write(p_string, size, p_message, &size);
	assert_that(strlen(p_message), is_equal_to(size));

	size_t position = 0;
	bool has_found = false;

	zob_string_find_char(p_string, 'j', 0, size, &position, &has_found);
	assert_that(has_found, is_true);
	assert_that(position, is_equal_to(5));

	zob_string_find_char(p_string, 'z', 0, size, &position, &has_found);
	assert_that(has_found, is_false);
	assert_that(position, is_equal_to(position));

	position = 0;
	zob_string_find_string(p_string, "jacta", 0, size, &position, &has_found);
	assert_that(has_found, is_true);
	assert_that(position, is_equal_to(5));

	position = 0;
	zob_string_find_string(p_string, "jactaest", 0, size, &position, &has_found);
	assert_that(has_found, is_false);
	assert_that(position, is_equal_to(position));

	position = 0;
	zob_string_find_string(p_string, "", 0, size, &position, &has_found);
	assert_that(has_found, is_false);
	assert_that(position, is_equal_to(0));

	{
		char * p_huge_needle = NULL;
		p_huge_needle = malloc(2048);
		memset(p_huge_needle, 'x', 2048);
		p_huge_needle[2047] = 0;

		position = 0;
		zob_string_find_string(p_string, "", 0, size, &position, &has_found);
		assert_that(has_found, is_false);
		assert_that(position, is_equal_to(0));

		free(p_huge_needle);
	}

	{
		char * p_huge_data = NULL;
		p_huge_data = malloc(2048);
		memset(p_huge_data, 'x', 2048);
		p_huge_data[2048 - 1] = 0;

		position = 0;
		zob_string_write(p_string, position, p_huge_data, &position);
		assert_that(position, is_equal_to(2048 - 1));

		position = 0;
		zob_string_find_string(p_string, p_huge_data, 0, size, &position, &has_found);
		assert_that(has_found, is_true);
		assert_that(position, is_equal_to(0));

		free(p_huge_data);
	}

	zob_string_dispose(&p_string);
	assert_that(p_string, is_equal_to(NULL));
}
