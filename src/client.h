#ifndef H_DB_CLIENT
#	define H_DB_CLIENT

struct db_client;
struct db_app;

#	include <stdbool.h>

void db_client_create(struct db_client ** pp_client);
void db_client_alloc(struct db_client ** pp_client);
void db_client_init(struct db_client * p_client);
void db_client_clean(struct db_client * p_client, bool has_to_dispose);

int db_client_run(struct db_client * p_client);

void db_client_connect(struct db_client * p_client, char const * sockname);
void db_client_send(struct db_client * p_client, int fd);
void db_client_recv(struct db_client * p_client);

#endif //H_DB_CLIENT
