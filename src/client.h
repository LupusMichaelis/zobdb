#ifndef H_DB_CLIENT
#	define H_DB_CLIENT

struct db_client;
struct db_app;

void db_client_create(struct db_client ** pp_client, struct db_app * p_app);
void db_client_new(struct db_client ** pp_db, struct db_app * p_app);
void db_client_init(struct db_client * p_db);

void db_client_connect(struct db_client * p_db, char const * sockname);
void db_client_send(struct db_client * p_db, int fd);
void db_client_recv(struct db_client * p_db);

#endif //H_DB_CLIENT

