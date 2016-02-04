#ifndef H_DB_POD_VECTOR
#	define H_DB_POD_VECTOR

#	include "allocator.h"

void *memset(void *s, int c, size_t n);

#	define APP_POD_VECTOR_INIT(pod_name, pod_type)											\
void pod_name##_vector_init(pod_type * p, size_t count)										\
{																							\
	memset(p, 0, sizeof *p);																\
}																							\

#	define APP_POD_VECTOR_CLEAN(pod_name, pod_type)											\
void pod_name##_vector_clean(pod_type ** pp, bool has_to_dispose)							\
{																							\
	if(has_to_dispose)																		\
	{																						\
		pod_type ** it = pp;																\
		do pod_name##_clean(*it, has_to_dispose);											\
		while(*++it);																		\
	}																						\
	memset(pp, 0, sizeof *pp);																\
}																							\

#	define APP_POD_VECTOR_ALLOC(pod_name, pod_type)											\
void pod_name##_vector_alloc(pod_type *** ppp, size_t count)								\
{																							\
	/* Allocates a vector of struct pointers */												\
	pod_type ** pp = NULL;																	\
	pp = calloc(count + 1, sizeof *pp);														\
	CHECK_NULL(pp);																	\
																							\
	/* Allocates elements  */																\
	pod_type * p = NULL;																	\
	p = calloc(count, sizeof *p);															\
	CHECK_NULL(p);																	\
																							\
	/* Hold elements in the vector */														\
	pod_type ** it = pp;																	\
	do	*it++ = p++;																		\
	while(--count);																			\
																							\
	/* Returns the vector */																\
	*ppp = pp;																				\
}																							\

#	define APP_POD_VECTOR_CREATE(pod_name, pod_type)										\
void pod_name##_vector_create(pod_type *** ppp, size_t count)								\
{																							\
	pod_type ** pp = NULL;																	\
	pod_name##_vector_alloc(&pp, count);													\
	CHECK_NULL(pp)																	\
	pod_type ** it = pp;																	\
	do																						\
	{																						\
		--count;																			\
		pod_name##_clean(*it, false);														\
		pod_name##_init(*it);																\
		++it;																				\
	} while(count);																			\
	*ppp = pp;																				\
}																							\

#	define APP_POD_VECTOR_DISPOSE(pod_name, pod_type)										\
void pod_name##_vector_dispose(pod_type *** pp)												\
{																							\
	do pod_name##_dispose(*pp);																\
	while(*++pp);																			\
	free(*pp);																				\
	*pp = NULL;																				\
}																							\

#	define APP_POD_VECTOR_CLONE(pod_name, pod_type)											\
void pod_name##_vector_clone(pod_type ** pp, pod_type *** ppp)								\
{																							\
	size_t length = 0;																		\
	while(*pp++) ++length;																	\
	pp -= length;																			\
	pod_name##_vector_alloc(ppp, length);													\
	pod_name##_vector_copy(pp, *ppp);														\
}																							\

#	define APP_POD_VECTOR_COPY(pod_name, pod_type)											\
void pod_name##_vector_copy(pod_type ** pp_from, pod_type ** pp_to)							\
{																							\
	do pod_name##_copy(*pp_from, *pp_to);													\
	while(*++pp_from && *++pp_to);															\
}																							\

#endif// H_DB_POD_VECTOR
