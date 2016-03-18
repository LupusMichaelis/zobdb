
#include "app.h"
#include "allocator.h"
#include "client.h"
#include "server.h"
#include "string.h"
#include "config.h"
#include "log.h"
#include "object.h"

#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

struct zob_app * gp_app;

void zob_app_singleton_set(struct zob_app * p_app)
{
	assert(!gp_app);

	gp_app = p_app;
}

struct zob_app
{
	char * name;

	bool is_server: 1;
	bool is_client: 1;

	void * p_main_module;
	struct zob_config ** pp_config;

	struct zob_log * p_log;
	struct zob_allocator * p_allocator;
};

// Don't use the APP_ALLOC macro, we don't have an app for error handling!
void zob_app_alloc(struct zob_app ** pp_app)
{
	struct zob_app * p_app = NULL;
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

void zob_app_init(struct zob_app * p_app)
{
}

void zob_app_dispose(struct zob_app ** pp_app)
{
	zob_app_clean(*pp_app, true);
	free(*pp_app);
	*pp_app = NULL;
}

void zob_app_command(struct zob_app * p_app, int argc, char ** argv)
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

	p_app->is_client = 0 == strcmp(slash, "zob");
	p_app->is_server = 0 == strcmp(slash, "zobd");
}

void zob_app_setup(struct zob_app * p_app)
{
	// Default allocator
	zob_allocator_alloc(&p_app->p_allocator);
	zob_allocator_init_std(p_app->p_allocator);

	// Configure app ///////////////////////////////////////////////////////
	int element_count = sizeof tbl_config / sizeof *tbl_config;
	struct zob_config ** pp_config = NULL;

	zob_config_vector_create(&pp_config, element_count);
	while(element_count--)
		zob_config_vector_set(pp_config, element_count, &tbl_config[element_count]);

	p_app->pp_config = pp_config;

	zob_log_create(&p_app->p_log);

	// Instantiate engine //////////////////////////////////////////////////
	if(p_app->is_client)
		zob_client_create((struct zob_client **)&p_app->p_main_module);
	else if(p_app->is_server)
		zob_server_create((struct zob_server **)&p_app->p_main_module);
	else
		zob_app_error(p_app, "Unknown module", __FILE__, __LINE__);
}

void zob_app_clean(struct zob_app * p_app, bool has_to_dispose)
{
	if(has_to_dispose)
	{
		if(p_app->is_client)
			zob_client_dispose((struct zob_client **)&p_app->p_main_module);
		else if(p_app->is_server)
			zob_server_dispose((struct zob_server **)&p_app->p_main_module);

		zob_log_dispose(&p_app->p_log);
		zob_config_vector_dispose(&p_app->pp_config);
		zob_allocator_dispose(&p_app->p_allocator);
	}

	memset(p_app, 0, sizeof *p_app);
}

int zob_app_run(struct zob_app * p_app)
{
	int exit_status = EXIT_FAILURE;
	if(p_app->is_client)
		exit_status = zob_client_run(p_app->p_main_module);
	else if(p_app->is_server)
		exit_status = zob_server_run(p_app->p_main_module);

	zob_app_dispose(&p_app);

	return exit_status;
}

void zob_app_allocator_get(struct zob_app * p_app, struct zob_allocator ** pp_allocator)
{
	*pp_allocator = p_app->p_allocator;
}

void zob_app_config_get(struct zob_app * p_app, char * p_name, char ** pp_value)
{
	char * p_value;
	zob_config_vector_get_by_name(p_app->pp_config, p_name, (void *) &p_value);
	*pp_value = p_value;
}

void zob_app_name_get_reference(struct zob_app * p_app, const char ** pp_name)
{
	*pp_name = p_app->name;
}

void zob_app_error(struct zob_app * p_app, char * p_error, char * filename, int filenumber)
{
	zob_log_error(p_app->p_log, p_error, filename, filenumber);
}

void zob_app_log(struct zob_app * p_app, char * filename, int filenumber, enum zob_log_level log_level, struct zob_string * p_text)
{
	zob_log_write(p_app->p_log, filename, filenumber, log_level, p_text);
}

