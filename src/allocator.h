#ifndef H_ZOB_ALLOCATOR
#	define H_ZOB_ALLOCATOR

#	include <stdlib.h>

////////////////////////////////////////////////////////////////////////////////
struct zob_app;
struct zob_allocator;
struct zob_error;

typedef void (*zob_allocator_do_allocate_callback)	(struct zob_allocator * p_allocator, void ** pp_target, size_t size);
typedef void (*zob_allocator_do_reallocate_callback)	(struct zob_allocator * p_allocator, void ** pp_target, size_t new_size);
typedef void (*zob_allocator_do_release_callback)	(struct zob_allocator * p_allocator, void ** pp_target);

////////////////////////////////////////////////////////////////////////////////
void zob_allocator_alloc(struct zob_allocator ** pp);
void zob_allocator_init(struct zob_allocator * p_allocator,
		zob_allocator_do_allocate_callback do_allocate_callback,
		zob_allocator_do_reallocate_callback do_reallocate_callback,
		zob_allocator_do_release_callback do_release_callback);
void zob_allocator_clean(struct zob_allocator * p_allocator);

void zob_allocator_do_allocate(struct zob_allocator * p_allocator, void ** pp_target, size_t size);
void zob_allocator_do_reallocate(struct zob_allocator * p_allocator, void ** pp_target, size_t new_size);
void zob_allocator_do_release(struct zob_allocator * p_allocator, void ** pp_target);

////////////////////////////////////////////////////////////////////////////////
void zob_allocator_init_std(struct zob_allocator * p_allocator);

#endif // H_ZOB_ALLOCATOR
