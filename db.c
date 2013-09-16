#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#include <sys/socket.h>
#include <sys/un.h>

#include <stdbool.h>

#include "src/app.h"
#include "src/client.h"

int main(int argc, char *argv[])
{
	struct db_app * p_app;
	db_app_new(&p_app);
	db_app_init(p_app, argc, argv);

	struct db_client * p_db = NULL;
	db_client_new(&p_db, p_app);
	db_client_init(p_db);

	db_client_connect(p_db, SOCK_NAME);
	db_client_send(p_db, STDIN_FILENO);

	return EXIT_SUCCESS;
}

