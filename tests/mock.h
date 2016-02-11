//
// MOCK ////////////////////////////////////////////////////////////////////////
struct zob_allocator;
struct zob_app {} gp_app;

inline void zob_allocator_do_allocate(struct zob_allocator * p_allocator, void ** pp_target, size_t size)
{
	*pp_target = malloc(size);
}

inline void zob_allocator_do_reallocate(struct zob_allocator * p_allocator, void ** pp_target, size_t new_size)
{
	*pp_target = realloc(*pp_target, new_size);
}

inline void zob_allocator_do_release(struct zob_allocator * p_allocator, void ** pp_target)
{
	free(*pp_target);
	*pp_target = NULL;
}

inline void zob_app_allocator_get(struct zob_app * p_app, struct zob_allocator ** pp_allocator)
{
	struct zob_allocator {};
	*pp_allocator = NULL;
}

inline void zob_app_error(struct zob_app * p_app, char * p_error, char * filename, int filenumber)
{
}

