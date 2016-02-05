#ifndef H_ZOB_OBJECT
#	define H_ZOB_OBJECT

#	include "app.h"
#	include "allocator.h"

#	include <stdlib.h>
#	include <stdbool.h>

struct zob_allocator;

#	define APP_ALLOC(struct_name)																\
void zob_##struct_name##_alloc(																	\
		struct zob_##struct_name ** pp															\
)																								\
{																								\
	struct zob_##struct_name * p = NULL;														\
	struct zob_allocator * p_allocator = NULL;													\
	zob_app_allocator_get(gp_app, &p_allocator);												\
	zob_allocator_do_allocate(p_allocator, (void **) &p, sizeof *p);							\
	*pp = p;																					\
}																								\

#	define APP_CREATE(struct_name)																\
void zob_##struct_name##_create(																\
		struct zob_##struct_name ** pp															\
)																								\
{																								\
	struct zob_##struct_name * p = NULL;														\
	struct zob_allocator * p_allocator = NULL;													\
	zob_app_allocator_get(gp_app, &p_allocator);												\
	zob_##struct_name##_alloc(&p);																\
	zob_##struct_name##_clean(p, false);														\
	zob_##struct_name##_init(p);																\
	*pp = p;																					\
}																								\

#	define APP_DISPOSE(struct_name)																\
void zob_##struct_name##_dispose(																\
		struct zob_##struct_name ** pp															\
)																								\
{																								\
	zob_##struct_name##_clean(*pp, true);														\
	struct zob_allocator * p_allocator = NULL;													\
	zob_app_allocator_get(gp_app, &p_allocator);												\
	zob_allocator_do_release(p_allocator, (void *) *pp);										\
	*pp = NULL;																					\
}																								\

#	define APP_CLONE(struct_name)																\
void zob_##struct_name##_clone(																	\
		struct zob_##struct_name * p,															\
		struct zob_##struct_name ** pp															\
)																								\
{																								\
	zob_##struct_name##_alloc(pp);																\
	zob_##struct_name##_init(*pp);																\
	zob_##struct_name##_copy(p, *pp);															\
}																								\

#endif// H_ZOB_OBJECT
