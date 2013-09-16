#ifndef H_DB_SERVER
#	define H_DB_SERVER

struct db_server;
struct db_app;

void db_server_new(struct db_server ** pp_db, struct db_app * p_app);
void db_server_init(struct db_server * p_db);
void db_server_open_store(struct db_server * p_db, const char * filename);
void db_server_listen(struct db_server * p_db, const char * socket_path);
void db_server_accept_begin(struct db_server * p_db);
void db_server_accept_end(struct db_server * p_db);
void db_server_read(struct db_server * p_db);

#endif //H_DB_SERVER
