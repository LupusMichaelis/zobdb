
#include "log.h"
#include "object.h"
#include "string.h"

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

struct zob_app;

struct zob_log
{
	int log_fd;
	FILE * p_log;
};

APP_ALLOC(log)
APP_CREATE(log)
APP_CLONE(log)
APP_DISPOSE(log)

void zob_log_init(struct zob_log * p_log)
{
	struct zob_string * p_filename = NULL;
	zob_app_config_get_helper(gp_app, "server.log.file", &p_filename);
	zob_log_open(p_log, p_filename);
	zob_string_dispose(&p_filename);
}

void zob_log_clean(struct zob_log * p_log, bool has_to_dispose)
{
	if(has_to_dispose && p_log->p_log)
		fclose(p_log->p_log);

	if(has_to_dispose && p_log->log_fd)
		close(p_log->log_fd);

	memset(p_log, 0, sizeof *p_log);
}

void zob_log_copy(struct zob_log * p_from, struct zob_log * p_to)
{
	p_to->log_fd = dup(p_from->log_fd);
	CHECK_INT(p_to->log_fd);
	p_to->p_log = fdopen(p_to->log_fd, "a");
	CHECK_NULL(p_to->p_log);
}

void zob_log_open(struct zob_log * p_log, struct zob_string * p_filename)
{
	char * filename = NULL;
	zob_string_get(p_filename, &filename);

	p_log->log_fd = open(filename, O_CREAT | O_APPEND | O_WRONLY, 0600);
	CHECK_INT(p_log->log_fd);
	p_log->p_log = fdopen(p_log->log_fd, "a");
	CHECK_NULL(p_log->p_log);
}

void zob_log_write
	( struct zob_log * p_log
	, char * filename
	, int filenumber
	, enum zob_log_level log_level
	, struct zob_string * p_text)
{
	char * p_raw_text = NULL;
	zob_string_get(p_text, &p_raw_text);
	struct zob_string * p_name = NULL;
	zob_app_name_get_reference(gp_app, &p_name);
	char * p_raw_name = NULL;
	zob_string_get(p_name, &p_raw_name);
	fprintf(p_log->p_log, "%s: %s:%d %s\n", p_raw_name, filename, filenumber, p_raw_text);
	fflush(p_log->p_log);
}

void zob_log_error(struct zob_log * p_log, char * p_error, char * filename, int filenumber)
{
	FILE * log = NULL;

	if(p_log)
		log = p_log->p_log;

	if(NULL == log)
		log = stderr;

	struct zob_string * p_name = NULL;
	zob_app_name_get_reference(gp_app, &p_name);
	char * p_raw_name = NULL;
	zob_string_get(p_name, &p_raw_name);
	fprintf(log, "%s: %s[%d] %s\n", p_raw_name, filename, filenumber, p_error);

	exit(EXIT_FAILURE);
}

