#include <stdlib.h>

#include <stdbool.h>

#include "src/app.h"
#include "src/server.h"

int main(int argc, char *argv[])
{
	struct db_app * p_app;
	db_app_new(&p_app);
	db_app_init(p_app, argc, argv);

	struct db_server * p_db = NULL;
	db_server_new(&p_db, p_app);
	db_server_init(p_db);
	db_server_open_store(p_db, STORE_NAME);

	db_server_listen(p_db, SOCK_NAME);
	do
	{
		db_server_accept_begin(p_db);
		db_server_read(p_db);
		db_server_accept_end(p_db);
	}
	while(true);

	return EXIT_SUCCESS;
}
