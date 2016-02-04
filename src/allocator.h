#ifndef H_DB_ALLOCATOR
#	define H_DB_ALLOCATOR

#	include <stdlib.h>

////////////////////////////////////////////////////////////////////////////////
struct db_app;
struct db_allocator;
struct db_error;

typedef void (*db_allocator_do_allocate_callback)	(struct db_allocator * p_allocator, void ** pp_target, size_t size);
typedef void (*db_allocator_do_reallocate_callback)	(struct db_allocator * p_allocator, void ** pp_target, size_t new_size);
typedef void (*db_allocator_do_release_callback)	(struct db_allocator * p_allocator, void ** pp_target);

////////////////////////////////////////////////////////////////////////////////
void db_allocator_alloc(struct db_allocator ** pp);
void db_allocator_init(struct db_allocator * p_allocator,
		db_allocator_do_allocate_callback do_allocate_callback,
		db_allocator_do_reallocate_callback do_reallocate_callback,
		db_allocator_do_release_callback do_release_callback);
void db_allocator_clean(struct db_allocator * p_allocator);

void db_allocator_do_allocate(struct db_allocator * p_allocator, void ** pp_target, size_t size);
void db_allocator_do_reallocate(struct db_allocator * p_allocator, void ** pp_target, size_t new_size);
void db_allocator_do_release(struct db_allocator * p_allocator, void ** pp_target);

////////////////////////////////////////////////////////////////////////////////
void db_allocator_init_std(struct db_allocator * p_allocator);

#endif // H_DB_ALLOCATOR
