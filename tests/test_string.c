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
