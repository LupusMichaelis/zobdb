
#include "app.h"
#include "client.h"
#include "server.h"
#include "string.h"
#include "config.h"

#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <string.h>
#include <stdbool.h>

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
	struct db_config ** pp_config;
};

// Don't use the APP_ALLOC macro, we don't have an app for error handling!
void db_app_alloc(struct db_app ** pp_app)
{
	struct db_app * p_app = NULL;
	p_app = calloc(1, sizeof *p_app);
	if(NULL == p_app)
		exit(1);

	*pp_app = p_app;
}

#include <assert.h>

static struct pair tbl_config[] = { { "store", STORE_NAME, }, };

void db_app_init(struct db_app * p_app, int argc, char ** argv)
{
	signal(SIGINT, db_app_signal);
	atexit(db_app_on_exit);

	// Determine app is server or client from name /////////////////////////
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

	p_app->is_client = 0 == strcmp(slash, "client");
	p_app->is_server = 0 == strcmp(slash, "writed");

	assert((p_app->is_client && !p_app->is_server) || (!p_app->is_client && p_app->is_server));

	// Configure app ///////////////////////////////////////////////////////
	int element_count = sizeof tbl_config / sizeof *tbl_config;
	struct db_config ** pp_config = NULL;

	db_config_vector_create(&pp_config, p_app, element_count);
	while(element_count--)
		db_config_vector_set(pp_config, element_count, &tbl_config[element_count]);

	p_app->pp_config = pp_config;

	// Instantiate engine //////////////////////////////////////////////////
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
}

void db_app_config_get(struct db_app * p_app, char * p_name, char ** pp_value)
{
	db_config_vector_get_by_name(p_app->pp_config, p_name, pp_value);
}

void db_app_error(struct db_app * p_app, char * p_error, char * filename, int filenumber)
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

void db_app_open_log(struct db_app * p_app, char * filename)
{
	p_app->log_fd = open(filename, O_CREAT | O_APPEND | O_WRONLY, 0600);
	CHECK_INT(p_app, p_app->log_fd);
}

void db_app_log(struct db_app * p_app, char * text, char * filename, int filenumber)
{
	FILE * log = fdopen(p_app->log_fd, "a");
	fprintf(log, "%s: %s[%d] %s\n", p_app->name, filename, filenumber, text);
}


