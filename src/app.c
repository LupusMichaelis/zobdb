
#include "app.h"
#include "allocator.h"
#include "client.h"
#include "server.h"
#include "string.h"
#include "config.h"
#include "log.h"

#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

struct db_app * gp_app;

void db_app_singleton_set(struct db_app * p_app)
{
	assert(!gp_app);

	gp_app = p_app;
}

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

	void * p_main_module;
	struct db_config ** pp_config;

	struct db_log * p_log;
	struct db_allocator * p_allocator;
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

static struct pair tbl_config[] =
	{
		{ "store", "./datas", },
		{ "log.file", "./log", },
		{ "socket.name", "./con", },
	};

void db_app_init(struct db_app * p_app)
{
	signal(SIGINT, db_app_signal);
	atexit(db_app_on_exit);
}

void db_app_command(struct db_app * p_app, int argc, char ** argv)
{
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
}

void db_app_setup(struct db_app * p_app)
{
	// Default allocator
	db_allocator_alloc(&p_app->p_allocator);
	db_allocator_init_std(p_app->p_allocator);

	// Configure app ///////////////////////////////////////////////////////
	int element_count = sizeof tbl_config / sizeof *tbl_config;
	struct db_config ** pp_config = NULL;

	db_config_vector_create(&pp_config, element_count);
	while(element_count--)
		db_config_vector_set(pp_config, element_count, &tbl_config[element_count]);

	p_app->pp_config = pp_config;

	db_log_create(&p_app->p_log);

	// Instantiate engine //////////////////////////////////////////////////
	if(p_app->is_client)
		db_client_create((struct db_client **)&p_app->p_main_module);
	else if(p_app->is_server)
		db_server_create((struct db_server **)&p_app->p_main_module);
/* XXX
	else
		db_app_error(p_app, "Unknown module", __FILE__, __LINE__);
*/
}

/*
void db_app_clean(struct db_app * p_app, bool has_to_dispose)
{
}
*/

int db_app_run(struct db_app * p_app)
{
	if(p_app->is_client)
		return db_client_run(p_app->p_main_module);
	else if(p_app->is_server)
		return db_server_run(p_app->p_main_module);

	return EXIT_FAILURE;
}

void db_app_allocator_get(struct db_app * p_app, struct db_allocator ** pp_allocator)
{
	*pp_allocator = p_app->p_allocator;
}

void db_app_config_get(struct db_app * p_app, char * p_name, char ** pp_value)
{
	char * p_value;
	db_config_vector_get_by_name(p_app->pp_config, p_name, (void *) &p_value);
	*pp_value = p_value;
}

void db_app_name_get_reference(struct db_app * p_app, const char ** pp_name)
{
	*pp_name = p_app->name;
}

void db_app_error(struct db_app * p_app, char * p_error, char * filename, int filenumber)
{
	db_log_error(p_app->p_log, p_error, filename, filenumber);
}

void db_app_log(struct db_app * p_app, char * text, char * filename, int filenumber)
{
	db_log_write(p_app->p_log, text, filename, filenumber);
}

