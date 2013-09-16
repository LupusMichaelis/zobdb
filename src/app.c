
#include "app.h"

#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <string.h>

static void db_app_signal(int s)
{
	exit(s);
}

static void db_app_on_exit()
{
	//unlink(SOCK_NAME);
}

struct db_app
{
	char * name;

	char *		p_error;
	void *		p_con;
};

void db_app_new(struct db_app ** pp_app)
{
	struct db_app * p_app = NULL;
	p_app = calloc(1, sizeof *p_app);
	if(NULL == p_app)
		exit(1);

	*pp_app = p_app;
}

void db_app_init(struct db_app * p_app, int argc, char ** argv)
{
	signal(SIGINT, db_app_signal);
	atexit(db_app_on_exit);

	p_app->name = *argv;
}

void db_app_error(struct db_app * p_app, const char * p_error, const char * filename, int filenumber)
{
	fprintf(stderr, "%s: %s[%d] %s\n", p_app->name, filename, filenumber, p_error);
	exit(EXIT_FAILURE);
}


