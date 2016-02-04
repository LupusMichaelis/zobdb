#ifndef H_DB_OBJECT
#	define H_DB_OBJECT

#	include "app.h"
#	include "allocator.h"

#	include <stdlib.h>
#	include <stdbool.h>

struct db_allocator;

#	define APP_ALLOC(struct_name)																\
void db_##struct_name##_alloc(																	\
		struct db_##struct_name ** pp															\
)																								\
{																								\
	struct db_##struct_name * p = NULL;															\
	struct db_allocator * p_allocator = NULL;													\
	db_app_allocator_get(gp_app, &p_allocator);													\
	db_allocator_do_allocate(p_allocator, (void **) &p, sizeof *p);								\
	*pp = p;																					\
}																								\

#	define APP_CREATE(struct_name)																\
void db_##struct_name##_create(																	\
		struct db_##struct_name ** pp															\
)																								\
{																								\
	struct db_##struct_name * p = NULL;															\
	struct db_allocator * p_allocator = NULL;													\
	db_app_allocator_get(gp_app, &p_allocator);													\
	db_##struct_name##_alloc(&p);																\
	db_##struct_name##_clean(p, false);															\
	db_##struct_name##_init(p);																	\
	*pp = p;																					\
}																								\

#	define APP_DISPOSE(struct_name)																\
void db_##struct_name##_dispose(																\
		struct db_##struct_name ** pp															\
)																								\
{																								\
	db_##struct_name##_clean(*pp, true);														\
	struct db_allocator * p_allocator = NULL;													\
	db_app_allocator_get(gp_app, &p_allocator);													\
	db_allocator_do_release(p_allocator, (void **) pp);											\
	*pp = NULL;																					\
}																								\

#	define APP_CLONE(struct_name)																\
void db_##struct_name##_clone(																	\
		struct db_##struct_name * p,															\
		struct db_##struct_name ** pp															\
)																								\
{																								\
	db_##struct_name##_alloc(pp);																\
	db_##struct_name##_init(*pp);																\
	db_##struct_name##_copy(p, *pp);															\
}																								\

#endif// H_DB_OBJECT
