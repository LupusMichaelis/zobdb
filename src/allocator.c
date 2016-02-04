
#include "allocator.h"
#include "error.h"
#include "app.h"

#include <errno.h>
#include <string.h>
#include <stdio.h>

struct zob_allocator
{
	zob_allocator_do_allocate_callback do_allocate;
	zob_allocator_do_reallocate_callback do_reallocate;
	zob_allocator_do_release_callback do_release;
};

////////////////////////////////////////////////////////////////////////////////
void zob_allocator_alloc(struct zob_allocator ** pp)
{
	struct zob_allocator * p = NULL;
	p = malloc(sizeof *p);
	CHECK_NULL(p);

	*pp = p;
}

void zob_allocator_init(struct zob_allocator * p_allocator,
		zob_allocator_do_allocate_callback do_allocate_callback,
		zob_allocator_do_reallocate_callback do_reallocate_callback,
		zob_allocator_do_release_callback do_release_callback
		)
{
	memset(p_allocator, 0, sizeof *p_allocator);
	p_allocator->do_allocate = do_allocate_callback;
	p_allocator->do_reallocate = do_reallocate_callback;
	p_allocator->do_release = do_release_callback;
}

void zob_allocator_clean(struct zob_allocator * p_allocator)
{
}

void zob_allocator_do_allocate(struct zob_allocator * p_allocator, void ** pp_target, size_t size)
{
	(p_allocator->do_allocate)(p_allocator, pp_target, size);
}

void zob_allocator_do_reallocate(struct zob_allocator * p_allocator, void ** pp_target, size_t new_size)
{
	(p_allocator->do_reallocate)(p_allocator, pp_target, new_size);
}

void zob_allocator_do_release(struct zob_allocator * p_allocator, void ** pp_target)
{
	(p_allocator->do_release)(p_allocator, pp_target);
}

////////////////////////////////////////////////////////////////////////////////
static void _do_allocate_standard(struct zob_allocator * p_allocator, void ** pp_target, size_t size);
static void _do_reallocate_standard(struct zob_allocator * p_allocator, void ** pp_target, size_t new_size);
static void _do_release_standard(struct zob_allocator * p_allocator, void ** pp_target);

void zob_allocator_init_std(struct zob_allocator * p_allocator)
{
	zob_allocator_init(p_allocator, _do_allocate_standard, _do_reallocate_standard, _do_release_standard);
}

void _do_reallocate_standard(struct zob_allocator * p_allocator, void ** pp_target, size_t new_size)
{
	void * p_target = realloc(*pp_target, new_size);
	CHECK_NULL(p_target);

	*pp_target = p_target;
}

void _do_allocate_standard(struct zob_allocator * p_allocator, void ** pp_target, size_t size)
{
	void * p_target = malloc(size);
	CHECK_NULL(p_target);

	*pp_target = p_target;
}

void _do_release_standard(struct zob_allocator * p_allocator, void ** pp_target)
{
	free(*pp_target);
	*pp_target = NULL;
}

