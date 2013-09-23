#ifndef H_DB_OBJECT
#	define H_DB_OBJECT

#	define APP_ALLOC(struct_name)															\
void db_##struct_name##_alloc(struct db_##struct_name ** pp, struct db_app * p_app)			\
{																							\
	struct db_##struct_name * p = NULL;														\
	p = calloc(1, sizeof *p);																\
	CHECK_NULL(p_app, p);																	\
	*pp = p;																				\
}																							\

#	define APP_CREATE(struct_name)															\
void db_##struct_name##_create(struct db_##struct_name ** pp, struct db_app * p_app)		\
{																							\
	struct db_##struct_name * p = NULL;														\
	db_##struct_name##_alloc(&p, p_app);													\
	db_##struct_name##_init(p, p_app);														\
	*pp = p;																				\
}																							\

#	define APP_DISPOSE(struct_name)															\
void db_##struct_name##_dispose(struct db_##struct_name ** pp)								\
{																							\
	db_##struct_name##_clean(*pp);															\
	free(*pp);																				\
	*pp = NULL;																				\
}																							\

#	define APP_CLONE(struct_name)															\
void db_##struct_name##_clone(struct db_##struct_name * p, struct db_##struct_name ** pp)	\
{																							\
	db_##struct_name##_alloc(pp, p->p_app);													\
	db_##struct_name##_copy(p, *pp);														\
}																							\

#endif// H_DB_OBJECT
