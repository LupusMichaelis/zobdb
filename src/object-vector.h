#ifndef H_ZOB_OBJECT_VECTOR
#	define H_ZOB_OBJECT_VECTOR

#	include "object.h"
#	include "pod-vector.h"

#	define APP_VECTOR_INIT(struct_name)		APP_POD_VECTOR_INIT(zob_##struct_name, struct zob_##struct_name)
#	define APP_VECTOR_ALLOC(struct_name)	APP_POD_VECTOR_ALLOC(zob_##struct_name, struct zob_##struct_name)
#	define APP_VECTOR_CREATE(struct_name)	APP_POD_VECTOR_CREATE(zob_##struct_name, struct zob_##struct_name)
#	define APP_VECTOR_DISPOSE(struct_name)	APP_POD_VECTOR_DISPOSE(zob_##struct_name, struct zob_##struct_name)
#	define APP_VECTOR_CLONE(struct_name)	APP_POD_VECTOR_CLONE(zob_##struct_name, struct zob_##struct_name)
#	define APP_VECTOR_COPY(struct_name)		APP_POD_VECTOR_COPY(zob_##struct_name, struct zob_##struct_name)
#	define APP_VECTOR_CLEAN(struct_name)	APP_POD_VECTOR_CLEAN(zob_##struct_name, struct zob_##struct_name)

#endif// H_ZOB_OBJECT_VECTOR
