#include "../src/buffer.h"


#include <cgreen/cgreen.h>

#include "mock.h"

void * malloc(size_t);
void free(void *);

// /MOCK ///////////////////////////////////////////////////////////////////////

Describe(buffer);

BeforeEach(buffer)
{
}

AfterEach(buffer)
{
}

Ensure(buffer, create_empty)
{
	struct zob_buffer * p_buffer = NULL;
	zob_buffer_create(&p_buffer);
	assert_that(p_buffer, is_not_equal_to(NULL));
	size_t size = -1;
	zob_buffer_size_get(p_buffer, &size);
	assert_that(size, is_equal_to(0));
	zob_buffer_dispose(&p_buffer);
	assert_that(p_buffer, is_equal_to(NULL));
}

Ensure(buffer, create_empty_ensure_size)
{
	struct zob_buffer * p_buffer = NULL;
	zob_buffer_create(&p_buffer);
	assert_that(p_buffer, is_not_equal_to(NULL));
	size_t size = -1;
	zob_buffer_size_get(p_buffer, &size);
	assert_that(size, is_equal_to(0));
	zob_buffer_ensure(p_buffer, 0, 10);
	zob_buffer_size_get(p_buffer, &size);
	assert_that(size, is_equal_to(0));
	zob_buffer_dispose(&p_buffer);
	assert_that(p_buffer, is_equal_to(NULL));
}

Ensure(buffer, create_empty_set_size)
{
	struct zob_buffer * p_buffer = NULL;
	zob_buffer_create(&p_buffer);
	assert_that(p_buffer, is_not_equal_to(NULL));
	size_t size = -1;
	zob_buffer_size_get(p_buffer, &size);
	assert_that(size, is_equal_to(0));
	zob_buffer_size_set(p_buffer, 10);
	zob_buffer_size_get(p_buffer, &size);
	assert_that(size, is_equal_to(10));
	zob_buffer_dispose(&p_buffer);
	assert_that(p_buffer, is_equal_to(NULL));
}

Ensure(buffer, create_empty_and_assign_data)
{
	struct zob_buffer * p_buffer = NULL;
	zob_buffer_create(&p_buffer);
	assert_that(p_buffer, is_not_equal_to(NULL));

	char const * p_message = "Alea jacta est";
	zob_buffer_is_auto_set(p_buffer, true);
	size_t size = -1;
	zob_buffer_write(p_buffer, 0, strlen(p_message), p_message, &size);
	assert_that(strlen(p_message), is_equal_to(size));

	zob_buffer_dispose(&p_buffer);
	assert_that(p_buffer, is_equal_to(NULL));
}

/*
Ensure(buffer, create_with_data)
{
	struct zob_buffer * p_buffer = NULL;
	char * p_message = "Direct content";
	zob_string_create_from_cstring(&p_buffer, p_message);

	size_t size = -1;
	zob_string_size_get(p_buffer, &size);
	assert_that(strlen(p_message), is_equal_to(size));
}
*/

Ensure(buffer, create_empty_and_assign_data_then_reset)
{
	struct zob_buffer * p_buffer = NULL;
	zob_buffer_create(&p_buffer);
	assert_that(p_buffer, is_not_equal_to(NULL));

	char const * p_message = "Alea jacta est";
	zob_buffer_is_auto_set(p_buffer, true);
	size_t size = -1;
	zob_buffer_write(p_buffer, 0, strlen(p_message), p_message, &size);
	assert_that(strlen(p_message), is_equal_to(size));

	zob_buffer_fill(p_buffer, 1024, '\0');

	zob_buffer_dispose(&p_buffer);
	assert_that(p_buffer, is_equal_to(NULL));
}

Ensure(buffer, create_empty_and_assign_data_and_search_for_char)
{
	struct zob_buffer * p_buffer = NULL;
	zob_buffer_create(&p_buffer);
	assert_that(p_buffer, is_not_equal_to(NULL));

	char const * p_message = "Alea jacta est";
	zob_buffer_is_auto_set(p_buffer, true);
	size_t size = -1;
	zob_buffer_write(p_buffer, 0, strlen(p_message), p_message, &size);
	assert_that(strlen(p_message), is_equal_to(size));

	size_t position = 0;
	bool has_found = false;
	zob_buffer_find_char(p_buffer, 'j', 0, size, &position, &has_found);

	assert_that(has_found, is_true);
	assert_that(position, is_equal_to(5));

	zob_buffer_dispose(&p_buffer);
	assert_that(p_buffer, is_equal_to(NULL));
}

Ensure(buffer, create_empty_and_assign_data_and_search_for_string)
{
	struct zob_buffer * p_buffer = NULL;
	zob_buffer_create(&p_buffer);
	assert_that(p_buffer, is_not_equal_to(NULL));

	char const * p_message = "Alea jacta est";
	zob_buffer_is_auto_set(p_buffer, true);
	size_t size = -1;
	zob_buffer_write(p_buffer, 0, strlen(p_message), p_message, &size);
	assert_that(strlen(p_message), is_equal_to(size));

	size_t position = 0;
	bool has_found = false;

	zob_buffer_find_char(p_buffer, 'j', 0, size, &position, &has_found);
	assert_that(has_found, is_true);
	assert_that(position, is_equal_to(5));

	zob_buffer_find_char(p_buffer, 'z', 0, size, &position, &has_found);
	assert_that(has_found, is_false);
	assert_that(position, is_equal_to(size));

	position = 0;
	zob_buffer_find_string(p_buffer, "jacta", 0, size, &position, &has_found);
	assert_that(has_found, is_true);
	assert_that(position, is_equal_to(5));

	position = 0;
	zob_buffer_find_string(p_buffer, "jactaest", 0, size, &position, &has_found);
	assert_that(has_found, is_false);
	assert_that(position, is_equal_to(size));

	position = 0;
	zob_buffer_find_string(p_buffer, "", 0, size, &position, &has_found);
	assert_that(has_found, is_false);
	assert_that(position, is_equal_to(0));

	{
		char p_huge_needle[2048];
		memset(p_huge_needle, 'x', 2048);
		p_huge_needle[2047] = 0;

		position = 0;
		zob_buffer_find_string(p_buffer, p_huge_needle, 0, size, &position, &has_found);
		assert_that(has_found, is_false);
		assert_that(position, is_equal_to(0));
	}

	zob_buffer_dispose(&p_buffer);
	assert_that(p_buffer, is_equal_to(NULL));
}

Ensure(buffer, create_empty_and_assign_data_and_amend)
{
	struct zob_buffer * p_buffer = NULL;
	zob_buffer_create(&p_buffer);
	assert_that(p_buffer, is_not_equal_to(NULL));

	char const * p_message = "My poney is blue";
	zob_buffer_is_auto_set(p_buffer, true);
	size_t size = -1;
	zob_buffer_write(p_buffer, 0, strlen(p_message), p_message, &size);
	assert_that(strlen(p_message), is_equal_to(size));

	char const * p_green = "green";
	zob_buffer_write(p_buffer, strlen("My poney is "), strlen(p_green), p_green, &size);
	assert_that(strlen(p_green), is_equal_to(size));

	struct zob_buffer * p_expected = NULL;
	zob_buffer_create(&p_expected);
	char const * p_expected_message = "My poney is green";
	zob_buffer_is_auto_set(p_expected, true);
	zob_buffer_write(p_expected, 0, strlen(p_expected_message), p_expected_message, &size);
	int diff = 42;
	zob_buffer_compare(p_buffer, p_expected, &diff);
	assert_that(diff, is_equal_to(0));

	zob_buffer_dispose(&p_buffer);
	assert_that(p_buffer, is_equal_to(NULL));
	zob_buffer_dispose(&p_expected);
	assert_that(p_expected, is_equal_to(NULL));
}

Ensure(buffer, create_and_assign_and_slice)
{
	struct zob_buffer * p_buffer = NULL;
	zob_buffer_create(&p_buffer);
	assert_that(p_buffer, is_not_equal_to(NULL));

	char const * p_message = "Alea jacta est";
	zob_buffer_is_auto_set(p_buffer, true);
	size_t size = -1;
	zob_buffer_write(p_buffer, 0, strlen(p_message), p_message, &size);
	assert_that(strlen(p_message), is_equal_to(size));

	struct zob_buffer * p_slice = NULL;
	zob_buffer_slice_get(p_buffer, 5, 11, &p_slice);
	assert_that(p_slice, is_not_equal_to(NULL));
	zob_buffer_size_get(p_slice, &size);
	assert_that(size, is_equal_to(11 - 5));
	zob_buffer_dispose(&p_buffer);
}

Ensure(buffer, compare_created_buffers)
{
	struct zob_buffer * p_first = NULL;
	zob_buffer_create(&p_first);
	assert_that(p_first, is_not_equal_to(NULL));

	char const * p_message = "Alea jacta est";
	zob_buffer_is_auto_set(p_first, true);
	zob_buffer_write(p_first, 0, strlen(p_message), p_message, NULL);

	struct zob_buffer * p_second = NULL;
	zob_buffer_create(&p_second);
	zob_buffer_is_auto_set(p_second, true);
	zob_buffer_write(p_second, 0, strlen(p_message), p_message, NULL);

	int diff = 42;
	zob_buffer_compare(p_first, p_second, &diff);
	assert_that(diff, is_equal_to(0));
	zob_buffer_dispose(&p_first);
	zob_buffer_dispose(&p_second);
}

Ensure(buffer, compare_cloned_buffers)
{
	struct zob_buffer * p_first = NULL;
	zob_buffer_create(&p_first);
	assert_that(p_first, is_not_equal_to(NULL));

	char const * p_message = "Alea jacta est";
	zob_buffer_is_auto_set(p_first, true);
	zob_buffer_write(p_first, 0, strlen(p_message), p_message, NULL);

	struct zob_buffer * p_second = NULL;
	zob_buffer_clone(p_first, &p_second);

	int diff = 42;
	zob_buffer_compare(p_first, p_second, &diff);
	assert_that(diff, is_equal_to(0));
	zob_buffer_dispose(&p_first);
}

Ensure(buffer, compare_different_buffers)
{
	struct zob_buffer * p_first = NULL;
	zob_buffer_create(&p_first);
	assert_that(p_first, is_not_equal_to(NULL));

	char const * p_first_message = "Alea jacta est";
	zob_buffer_is_auto_set(p_first, true);
	zob_buffer_write(p_first, 0, strlen(p_first_message), p_first_message, NULL);

	struct zob_buffer * p_second = NULL;
	zob_buffer_create(&p_second);

	char const * p_second_message = "Cogito ergo sum";
	zob_buffer_is_auto_set(p_second, true);
	zob_buffer_write(p_second, 0, strlen(p_second_message), p_second_message, NULL);

	int diff = 42;
	zob_buffer_compare(p_first, p_second, &diff);
	assert_that(diff, is_not_equal_to(0));
	assert_that(diff, is_not_equal_to(42));
	zob_buffer_dispose(&p_first);
	zob_buffer_dispose(&p_second);
}

Ensure(buffer, compare_buffers_with_slice)
{
	struct zob_buffer * p_original = NULL;
	zob_buffer_create(&p_original);
	assert_that(p_original, is_not_equal_to(NULL));

	char const * p_original_message = "Alea jacta est";
	zob_buffer_is_auto_set(p_original, true);
	zob_buffer_write(p_original, 0, strlen(p_original_message), p_original_message, NULL);

	struct zob_buffer * p_alea = NULL;
	zob_buffer_create(&p_alea);

	char const * p_alea_message = "Alea";
	zob_buffer_is_auto_set(p_alea, true);
	zob_buffer_write(p_alea, 0, strlen(p_alea_message), p_alea_message, NULL);

	int diff = 42;
	zob_buffer_compare(p_original, p_alea, &diff);
	assert_that(diff, is_not_equal_to(0));
	assert_that(diff, is_not_equal_to(42));

	struct zob_buffer * p_slice = NULL;
	zob_buffer_slice_get(p_original, 0, strlen(p_alea_message), &p_slice);

	zob_buffer_compare(p_slice, p_alea, &diff);
	assert_that(diff, is_equal_to(0));
	assert_that(diff, is_not_equal_to(42));
	zob_buffer_dispose(&p_alea);
	zob_buffer_dispose(&p_original);
}

Ensure(buffer, compare_buffers_with_slice_from_middle)
{
	struct zob_buffer * p_original = NULL;
	zob_buffer_create(&p_original);
	assert_that(p_original, is_not_equal_to(NULL));

	char const * p_original_message = "Alea jacta est";
	zob_buffer_is_auto_set(p_original, true);
	zob_buffer_write(p_original, 0, strlen(p_original_message), p_original_message, NULL);

	struct zob_buffer * p_jacta = NULL;
	zob_buffer_create(&p_jacta);

	char const * p_jacta_message = "jacta";
	zob_buffer_is_auto_set(p_jacta, true);
	zob_buffer_write(p_jacta, 0, strlen(p_jacta_message), p_jacta_message, NULL);

	int diff = 42;
	zob_buffer_compare(p_original, p_jacta, &diff);
	assert_that(diff, is_not_equal_to(0));
	assert_that(diff, is_not_equal_to(42));

	struct zob_buffer * p_slice = NULL;
	zob_buffer_slice_get(p_original, 5, 5 + strlen(p_jacta_message), &p_slice);

	zob_buffer_compare(p_slice, p_jacta, &diff);
	assert_that(diff, is_equal_to(0));
	assert_that(diff, is_not_equal_to(42));
	zob_buffer_dispose(&p_original);
	zob_buffer_dispose(&p_jacta);
}

#if 0
int main()
{
	TestSuite * suite = create_test_suite();
	/*
	add_test_with_context(suite, buffer, create_empty);
	add_test_with_context(suite, buffer, create_empty_ensure_size);
	add_test_with_context(suite, buffer, create_empty_and_assign_data);
	add_test_with_context(suite, buffer, create_empty_and_assign_data_then_reset);
	add_test_with_context(suite, buffer, create_empty_and_assign_data_and_search_for_string);
	add_test_with_context(suite, buffer, create_empty_and_assign_data);
	add_test_with_context(suite, buffer, create_and_assign_and_slice);
	add_test_with_context(suite, buffer, compare_cloned_buffers);
	add_test_with_context(suite, buffer, compare_buffers_with_slice_from_middle);
	add_test_with_context(suite, buffer, create_empty_and_assign_data_and_amend);
	*/

	run_test_suite(suite, create_text_reporter());
	setup_reporting(create_text_reporter());

/*	buffer__create_empty_ensure_size();*/
/*	buffer__compare_buffers_with_slice_from_middle();*/
	buffer__create_empty_and_assign_data_and_amend();

	return EXIT_SUCCESS;
}
#endif // 0
