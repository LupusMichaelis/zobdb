#include <unistd.h>
#include <stdlib.h>

#include <stdbool.h>

#include "src/app.h"
#include "src/server.h"

int main(int argc, char *argv[])
{
	daemon(1, 0);

	struct db_app * p_app;
	db_app_new(&p_app);
	db_app_init(p_app, argc, argv);
	db_app_open_log(p_app, LOG_NAME);

	struct db_server * p_db = NULL;
	db_server_new(&p_db, p_app);
	db_server_init(p_db);

	db_server_store_open(p_db, STORE_NAME);

	unlink(SOCK_NAME);
	db_server_listen(p_db, SOCK_NAME);
	do
	{
		db_server_session_begin(p_db);

		char * p_buffer = NULL;
		db_server_read(p_db, &p_buffer);
		db_server_store_write(p_db, p_buffer);
		free(p_buffer);

		db_server_answer(p_db, "Ok");
		db_server_session_end(p_db);
	}
	while(true);

	return EXIT_SUCCESS;
}
