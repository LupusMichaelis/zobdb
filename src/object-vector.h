#ifndef H_DB_OBJECT_VECTOR
#	define H_DB_OBJECT_VECTOR

#	include "object.h"
#	include "pod-vector.h"

#	define APP_VECTOR_INIT(struct_name)		APP_POD_VECTOR_INIT(db_##struct_name, struct db_##struct_name)
#	define APP_VECTOR_ALLOC(struct_name)	APP_POD_VECTOR_ALLOC(db_##struct_name, struct db_##struct_name)
#	define APP_VECTOR_CREATE(struct_name)	APP_POD_VECTOR_CREATE(db_##struct_name, struct db_##struct_name)
#	define APP_VECTOR_DISPOSE(struct_name)	APP_POD_VECTOR_DISPOSE(db_##struct_name, struct db_##struct_name)
#	define APP_VECTOR_CLONE(struct_name)	APP_POD_VECTOR_CLONE(db_##struct_name, struct db_##struct_name)
#	define APP_VECTOR_COPY(struct_name)		APP_POD_VECTOR_COPY(db_##struct_name, struct db_##struct_name)
#	define APP_VECTOR_CLEAN(struct_name)	APP_POD_VECTOR_CLEAN(db_##struct_name, struct db_##struct_name)

#endif// H_DB_OBJECT_VECTOR
