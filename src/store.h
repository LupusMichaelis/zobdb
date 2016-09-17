#ifndef H_ZOB_STORE
#	define H_ZOB_STORE

struct zob_app;
struct zob_store;
struct zob_string;

#	include <stdbool.h>

void zob_store_alloc(struct zob_store ** pp_store);
void zob_store_create(struct zob_store ** pp_store);
void zob_store_init(struct zob_store * p_store);
void zob_store_clone(struct zob_store * p_store, struct zob_store ** pp_store);
void zob_store_copy(struct zob_store * p_from, struct zob_store * p_to);
void zob_store_clean(struct zob_store * p_store, bool has_to_dispose) ;
void zob_store_dispose(struct zob_store ** pp_store);

void zob_store_connect(struct zob_store * p_store, struct zob_string * p_file_name);
void zob_store_open(struct zob_store * p_store, const char * filename);
void zob_store_read(
		struct zob_store * p_store,
		struct zob_string * p_key,
		struct zob_string * p_value,
		bool * p_is_ok);
void zob_store_write(
		struct zob_store * p_store,
		struct zob_string * p_key,
		struct zob_string * p_value,
		bool is_overwrite,
		bool * p_is_ok);
void zob_store_delete(
		struct zob_store * p_store,
		struct zob_string * p_key,
		bool * p_is_ok);

#endif // H_ZOB_STORE
