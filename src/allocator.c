
#include "allocator.h"
#include "error.h"
#include "app.h"

#include <errno.h>
#include <string.h>
#include <stdio.h>

struct db_allocator
{
	db_allocator_do_allocate_callback do_allocate;
	db_allocator_do_reallocate_callback do_reallocate;
	db_allocator_do_release_callback do_release;
};

////////////////////////////////////////////////////////////////////////////////
void db_allocator_alloc(struct db_allocator ** pp)
{
	struct db_allocator * p = NULL;
	p = malloc(sizeof *p);
	CHECK_NULL(p);

	*pp = p;
}

void db_allocator_init(struct db_allocator * p_allocator,
		db_allocator_do_allocate_callback do_allocate_callback,
		db_allocator_do_reallocate_callback do_reallocate_callback,
		db_allocator_do_release_callback do_release_callback
		)
{
	memset(p_allocator, 0, sizeof *p_allocator);
	p_allocator->do_allocate = do_allocate_callback;
	p_allocator->do_reallocate = do_reallocate_callback;
	p_allocator->do_release = do_release_callback;
}

void db_allocator_clean(struct db_allocator * p_allocator)
{
}

void db_allocator_do_allocate(struct db_allocator * p_allocator, void ** pp_target, size_t size)
{
	(p_allocator->do_allocate)(p_allocator, pp_target, size);
}

void db_allocator_do_reallocate(struct db_allocator * p_allocator, void ** pp_target, size_t new_size)
{
	(p_allocator->do_reallocate)(p_allocator, pp_target, new_size);
}

void db_allocator_do_release(struct db_allocator * p_allocator, void ** pp_target)
{
	(p_allocator->do_release)(p_allocator, pp_target);
}

////////////////////////////////////////////////////////////////////////////////
static void _do_allocate_standard(struct db_allocator * p_allocator, void ** pp_target, size_t size);
static void _do_reallocate_standard(struct db_allocator * p_allocator, void ** pp_target, size_t new_size);
static void _do_release_standard(struct db_allocator * p_allocator, void ** pp_target);

void db_allocator_init_std(struct db_allocator * p_allocator)
{
	db_allocator_init(p_allocator, _do_allocate_standard, _do_reallocate_standard, _do_release_standard);
}

void _do_reallocate_standard(struct db_allocator * p_allocator, void ** pp_target, size_t new_size)
{
	void * p_target = realloc(*pp_target, new_size);
	CHECK_NULL(p_target);

	*pp_target = p_target;
}

void _do_allocate_standard(struct db_allocator * p_allocator, void ** pp_target, size_t size)
{
	void * p_target = malloc(size);
	CHECK_NULL(p_target);

	*pp_target = p_target;
}

void _do_release_standard(struct db_allocator * p_allocator, void ** pp_target)
{
	free(*pp_target);
	*pp_target = NULL;
}

