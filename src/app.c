
#include "app.h"

#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <string.h>
#include <stdbool.h>

// XXX Temporary forwarding declarations
struct db_client;
struct db_server;
void db_client_create(struct db_client ** pp_client, struct db_app * p_app);
void db_server_create(struct db_server ** pp_client, struct db_app * p_app);

int db_client_run(struct db_client * p_client);
int db_server_run(struct db_server * p_server);

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

	bool is_server: 1;
	bool is_client: 1;

	char * p_error;
	void * p_con;

	int log_fd;

	void * p_main_module;
};

void db_app_new(struct db_app ** pp_app)
{
	struct db_app * p_app = NULL;
	p_app = calloc(1, sizeof *p_app);
	if(NULL == p_app)
		exit(1);

	*pp_app = p_app;
}

#include <assert.h>

void db_app_init(struct db_app * p_app, int argc, char ** argv)
{
	signal(SIGINT, db_app_signal);
	atexit(db_app_on_exit);

	p_app->name = *argv;

	char * slash = p_app->name + strlen(p_app->name);
	do if('/' == *slash)
	{
		++slash;
		break;
	}
	else
		--slash;
	while(p_app->name != slash);
	
	p_app->is_client = 0 == strcmp(slash, "db");
	p_app->is_server = 0 == strcmp(slash, "dbd");

	assert((p_app->is_client && !p_app->is_server) || (!p_app->is_client && p_app->is_server));

	if(p_app->is_client)
		db_client_create((struct db_client **)&p_app->p_main_module, p_app);
	else if(p_app->is_server)
		db_server_create((struct db_server **)&p_app->p_main_module, p_app);
	else
		db_app_error(p_app, "Unknown module", __FILE__, __LINE__);
}

int db_app_run(struct db_app * p_app)
{
	if(p_app->is_client)
		return db_client_run(p_app->p_main_module);
	else if(p_app->is_server)
		return db_server_run(p_app->p_main_module);

	return EXIT_FAILURE;
	//return db_module_run(p_app->p_main_module);
}


void db_app_error(struct db_app * p_app, const char * p_error, const char * filename, int filenumber)
{
	FILE * log = fdopen(p_app->log_fd, "a");
	if(NULL == log)
	{
		log = stderr;
		fprintf(log, "%s: %s[%d] %s\n", p_app->name, __FILE__, __LINE__, strerror(errno));
	}

	fprintf(log, "%s: %s[%d] %s\n", p_app->name, filename, filenumber, p_error);

	exit(EXIT_FAILURE);
}

void db_app_open_log(struct db_app * p_app, const char * filename)
{
	p_app->log_fd = open(filename, O_CREAT | O_APPEND | O_WRONLY, 0600);
	CHECK_INT(p_app, p_app->log_fd);
}

void db_app_log(struct db_app * p_app, char const * text, const char * filename, int filenumber)
{
	FILE * log = fdopen(p_app->log_fd, "a");
	fprintf(log, "%s: %s[%d] %s\n", p_app->name, filename, filenumber, text);
}


