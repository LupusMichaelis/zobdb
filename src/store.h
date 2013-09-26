#ifndef H_DB_STORE
#	define H_DB_STORE

struct db_app;
struct db_store;

#	include <stdbool.h>

void db_store_alloc(struct db_store ** pp_store, struct db_app * p_app);
void db_store_create(struct db_store ** pp_store, struct db_app * p_app);
void db_store_init(struct db_store * p_store, struct db_app * p_app);
void db_store_clone(struct db_store * p_store, struct db_store ** pp_store);
void db_store_copy(struct db_store * p_from, struct db_store * p_to);
void db_store_clean(struct db_store * p_store);
void db_store_dispose(struct db_store ** pp_store);

void db_store_generate_ticket(struct db_store * p_store, char ** pp_ticket);

void db_store_open(struct db_store * p_store, const char * filename);
//void db_store_read(struct db_store * p_store, const char * p_key, const char ** pp_value, bool * is_ok);
void db_store_write(
		struct db_store * p_store,
		const char * p_key,
		const char * p_value,
		char ** pp_ticket,
		bool * p_is_ok);

#endif // H_DB_STORE
