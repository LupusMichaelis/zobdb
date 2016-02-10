#include "../src/buffer.h"

#include <cgreen/cgreen.h>

void * malloc(size_t);
void free(void *);

// MOCK ////////////////////////////////////////////////////////////////////////
struct zob_allocator;
struct zob_app {} gp_app;

void zob_allocator_do_allocate(struct zob_allocator * p_allocator, void ** pp_target, size_t size)
{
	*pp_target = malloc(size);
}

void zob_allocator_do_reallocate(struct zob_allocator * p_allocator, void ** pp_target, size_t new_size)
{
	*pp_target = realloc(*pp_target, new_size);
}

void zob_allocator_do_release(struct zob_allocator * p_allocator, void ** pp_target)
{
	free(*pp_target);
	*pp_target = NULL;
}

void zob_app_allocator_get(struct zob_app * p_app, struct zob_allocator ** pp_allocator)
{
	struct zob_allocator {};
	*pp_allocator = NULL;
}

void zob_app_error(struct zob_app * p_app, char * p_error, char * filename, int filenumber)
{
}
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
	size_t size = 0;
	assert_that(size, is_equal_to(0));
	zob_buffer_dispose(&p_buffer);
	assert_that(p_buffer, is_equal_to(NULL));
}

Ensure(buffer, create_empty_and_assign_data)
{
	struct zob_buffer * p_buffer = NULL;
	zob_buffer_create(&p_buffer);
	assert_that(p_buffer, is_not_equal_to(NULL));

	char const * p_message = "Alea jacta est";
	zob_buffer_set_is_auto(p_buffer, true);
	size_t size = 0;
	zob_buffer_write(p_buffer, &size, p_message);
	assert_that(strlen(p_message), is_equal_to(size));

	zob_buffer_dispose(&p_buffer);
	assert_that(p_buffer, is_equal_to(NULL));
}

Ensure(buffer, create_empty_and_assign_data_then_reset)
{
	struct zob_buffer * p_buffer = NULL;
	zob_buffer_create(&p_buffer);
	assert_that(p_buffer, is_not_equal_to(NULL));

	char const * p_message = "Alea jacta est";
	zob_buffer_set_is_auto(p_buffer, true);
	size_t size = 0;
	zob_buffer_write(p_buffer, &size, p_message);
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
	zob_buffer_set_is_auto(p_buffer, true);
	size_t size = 0;
	zob_buffer_write(p_buffer, &size, p_message);
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
	zob_buffer_set_is_auto(p_buffer, true);
	size_t size = 0;
	zob_buffer_write(p_buffer, &size, p_message);
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

/*
int main()
{
	TestSuite * suite = create_test_suite();
	add_test_with_context(suite, buffer, create_empty);
	add_test_with_context(suite, buffer, create_empty_and_assign_data);

	run_single_test(suite, "Buffer", create_text_reporter());
	run_test_suite(suite, create_text_reporter());

	return EXIT_SUCCESS;
}
*/
