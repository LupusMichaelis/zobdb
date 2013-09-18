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

int main(int argc, char *argv[])
{
	struct db_app * p_app;
	db_app_new(&p_app);
	db_app_init(p_app, argc, argv);
	return db_app_run(p_app);
}

