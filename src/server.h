#ifndef H_DB_SERVER
#	define H_DB_SERVER

struct db_app;
struct db_server;
struct db_message;

void db_server_create(struct db_server ** pp_server, struct db_app * p_app);
void db_server_alloc(struct db_server ** pp_db, struct db_app * p_app);
void db_server_init(struct db_server * p_db, struct db_app * p_app);

int db_server_run(struct db_server * p_server);
void db_server_listen(struct db_server * p_db, const char * socket_path);
void db_server_session_begin(struct db_server * p_db);
void db_server_session_end(struct db_server * p_db);
void db_server_read(struct db_server * p_db, char **pp_payload);
void db_server_answer(struct db_server * p_db, struct db_message * p_answer);
void db_server_process(struct db_server * p_db, struct db_message * p_request, struct db_message * p_answer);

#endif //H_DB_SERVER
