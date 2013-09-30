#ifndef H_DB_OBJECT_VECTOR
#	define H_DB_OBJECT_VECTOR

#	include "object.h"

#	define APP_VECTOR_ALLOC(struct_name)															\
void db_##struct_name##_vector_alloc(struct db_##struct_name *** ppp, struct db_app * p_app, size_t count)	\
{																							\
	/* Allocates a vector of struct pointers */												\
	struct db_##struct_name ** pp = NULL;													\
	pp = calloc(count + 1, sizeof *pp);														\
	CHECK_NULL(p_app, pp);																	\
																							\
	/* Allocates elements  */																\
	struct db_##struct_name * p = NULL;														\
	p = calloc(count, sizeof *p);															\
	CHECK_NULL(p_app, p);																	\
																							\
	/* Hold elements in the vector */														\
	struct db_##struct_name ** it = pp;														\
	do	*it++ = p++;																		\
	while(--count);																			\
																							\
	/* Returns the vector */																\
	*ppp = pp;																				\
}																							\

#	define APP_VECTOR_CREATE(struct_name)													\
void db_##struct_name##_vector_create(struct db_##struct_name *** ppp, struct db_app * p_app, size_t count)	\
{																							\
	struct db_##struct_name ** pp = NULL;													\
	db_##struct_name##_vector_alloc(&pp, p_app, count);										\
	CHECK_NULL(p_app, pp)																	\
	struct db_##struct_name ** it = pp;															\
	do																						\
	{																						\
		--count;																			\
		db_##struct_name##_clean(*it, false);												\
		db_##struct_name##_init(*it, p_app);												\
		++it;																				\
	} while(count);																			\
	*ppp = pp;																				\
}																							\

#	define APP_VECTOR_DISPOSE(struct_name)													\
void db_##struct_name##_vector_dispose(struct db_##struct_name *** pp)						\
{																							\
	do db_##struct_name##_dispose(*pp);														\
	while(*++pp);																			\
	free(*pp);																				\
	*pp = NULL;																				\
}																							\

#	define APP_VECTOR_CLONE(struct_name)													\
void db_##struct_name##_vector_clone(struct db_##struct_name ** pp, struct db_##struct_name *** ppp)			\
{																							\
	size_t length = 0;																		\
	while(*pp++) ++length ;																	\
	pp -= length;																			\
	db_##struct_name##_vector_alloc(ppp, (*pp)->p_app, length);									\
	do db_##struct_name##_copy(*pp, **ppp);														\
	while(++ppp, *++pp);																		\
}																							\

#endif// H_DB_OBJECT_VECTOR
