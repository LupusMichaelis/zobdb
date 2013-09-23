
#include "app.h"
#include "client.h"
#include "server.h"

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

struct config
{
	char * p_name;
	char * p_value;
};


struct db_app
{
	char * name;

	bool is_server: 1;
	bool is_client: 1;

	char * p_error;
	void * p_con;

	int log_fd;

	void * p_main_module;
	struct config ** pp_config;
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

static struct config tbl_config[] = { { "store", STORE_NAME, }, };

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
	
	p_app->is_client = 0 == strcmp(slash, "db");
	p_app->is_server = 0 == strcmp(slash, "dbd");

	assert((p_app->is_client && !p_app->is_server) || (!p_app->is_client && p_app->is_server));

	// Configure app ///////////////////////////////////////////////////////
	int element_count = sizeof tbl_config / sizeof *tbl_config;
	struct config ** pp_config = calloc(1 + element_count, sizeof *pp_config);
	struct config * p_config = calloc(element_count, sizeof **pp_config);

	while(element_count--)
	{
		memcpy(p_config + element_count, &tbl_config[element_count], sizeof *p_config);
		*(pp_config + element_count) = p_config + element_count;
	}

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

void db_app_config_get(struct db_app * p_app, const char * p_name, void ** pp_value)
{
	struct config ** pp_config = p_app->pp_config;

	while(*pp_config)
		if(0 == strcmp(p_name, (*pp_config)->p_name))
			break;
		else
			++pp_config;

	*pp_value = NULL == *pp_config ? NULL : (*pp_config)->p_value;
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


