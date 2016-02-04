#ifndef H_ZOB_STORE
#	define H_ZOB_STORE

struct zob_app;
struct zob_store;

#	include <stdbool.h>

void zob_store_alloc(struct zob_store ** pp_store);
void zob_store_create(struct zob_store ** pp_store);
void zob_store_init(struct zob_store * p_store);
void zob_store_clone(struct zob_store * p_store, struct zob_store ** pp_store);
void zob_store_copy(struct zob_store * p_from, struct zob_store * p_to);
void zob_store_clean(struct zob_store * p_store, bool has_to_dispose) ;
void zob_store_dispose(struct zob_store ** pp_store);

void zob_store_open(struct zob_store * p_store, const char * filename);
void zob_store_read(
		struct zob_store * p_store,
		const char * p_key,
		char ** pp_value,
		bool * is_ok);
void zob_store_write(
		struct zob_store * p_store,
		const char * p_key,
		const char * p_value,
		bool is_overwrite,
		bool * p_is_ok);
void zob_store_delete(
		struct zob_store * p_store,
		const char * p_key,
		bool * is_ok);

#endif // H_ZOB_STORE
