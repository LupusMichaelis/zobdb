#ifndef H_DB_STORE
#	define H_DB_STORE

struct db_app;
struct db_store;

void db_app_store_create(struct db_store ** pp_store, struct db_app * p_app);
void db_app_store_new(struct db_store ** pp_store, struct db_app * p_app);
void db_app_store_init(struct db_store * p_app);

void db_app_store_open(struct db_store * p_store, const char * filename);
void db_app_store_write(struct db_store * p_store, const char * text);

#endif // H_DB_STORE
