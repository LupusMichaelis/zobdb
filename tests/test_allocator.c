#include "../src/allocator.h"
#include "../src/app.h"

#include <assert.h>
#include <stdio.h>
#include <stdbool.h>

void test_mock();
void test_std();

int main(int argc, char *argv[])
{
	//test_std();
	test_mock();

	printf("Huge success\n");

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
void _do_allocate_mock(struct zob_allocator * p_allocator, void ** pp_target, size_t size);
void _do_reallocate_mock(struct zob_allocator * p_allocator, void ** pp_target, size_t new_size);
void _do_release_mock(struct zob_allocator * p_allocator, void ** pp_target);

bool _allocation_done = false;
bool _reallocation_done = false;
bool _release_done = false;

void test_mock()
{
	struct zob_app * p_app = NULL;
	zob_app_alloc(&p_app);
	zob_app_init(p_app);

	struct zob_allocator * p_allocator = NULL;
	zob_allocator_alloc(&p_allocator);
	zob_allocator_init(p_allocator, &_do_allocate_mock, &_do_reallocate_mock, &_do_release_mock);
}

void _do_allocate_mock(struct zob_allocator * p_allocator, void ** pp_target, size_t size)
{
	*pp_target = (void *)0xdeadbeef;
	_allocation_done = true;
}

void _do_reallocate_mock(struct zob_allocator * p_allocator, void ** pp_target, size_t new_size)
{
	*pp_target = (void *)0xb00bb00b;
	_reallocation_done = true;
}

void _do_release_mock(struct zob_allocator * p_allocator, void ** pp_target)
{
	*pp_target = NULL;
	_release_done = true;
}

////////////////////////////////////////////////////////////////////////////////
void test_std()
{
	struct zob_app * p_app = NULL;
	zob_app_alloc(&p_app);
	zob_app_init(p_app);

	struct zob_allocator * p_allocator = NULL;
	zob_allocator_init_std(p_allocator);

	int * p_i = NULL;

	assert(!_allocation_done);
	assert(!_reallocation_done);
	assert(!_release_done);

	zob_allocator_do_allocate(p_allocator, (void **)&p_i, sizeof *p_i);

	assert(_allocation_done);
	assert(!_reallocation_done);
	assert((void *)0xdeadbeef == p_i);
	assert(!_release_done);

	zob_allocator_do_reallocate(p_allocator, (void **)&p_i, 10 * sizeof *p_i);

	assert(_allocation_done);
	assert(_reallocation_done);
	assert((void *)0xdeadbeef == p_i);
	assert(!_release_done);

	zob_allocator_do_release(p_allocator, (void **)&p_i);

	assert(_allocation_done);
	assert(_reallocation_done);
	assert(_release_done);
	assert(NULL == p_i);

}
