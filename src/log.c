
#include "log.h"
#include "object.h"

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

struct db_app;

struct db_log
{
	int log_fd;
	FILE * p_log;
};

APP_ALLOC(log)
APP_CREATE(log)
APP_CLONE(log)
APP_DISPOSE(log)

void db_log_init(struct db_log * p_log)
{
	char * p_filename = NULL;
	db_app_config_get(gp_app, "log.file", &p_filename);
	CHECK_NULL(p_filename);
	db_log_open(p_log, p_filename);
}

void db_log_clean(struct db_log * p_log, bool has_to_dispose)
{
	if(p_log->p_log)
		fclose(p_log->p_log);

	if(p_log->log_fd)
		close(p_log->log_fd);

	memset(p_log, 0, sizeof *p_log);
}

void db_log_copy(struct db_log * p_from, struct db_log * p_to)
{
	p_to->log_fd = dup(p_from->log_fd);
	CHECK_INT(p_to->log_fd);
	p_to->p_log = fdopen(p_to->log_fd, "a");
	CHECK_NULL(p_to->p_log);
}

void db_log_open(struct db_log * p_log, char * filename)
{
	p_log->log_fd = open(filename, O_CREAT | O_APPEND | O_WRONLY, 0600);
	CHECK_INT(p_log->log_fd);
	p_log->p_log = fdopen(p_log->log_fd, "a");
	CHECK_NULL(p_log->p_log);
}

void db_log_write(struct db_log * p_log, char * text, char * filename, int filenumber)
{
	const char * p_name = NULL;
	db_app_name_get_reference(gp_app, &p_name);
	fprintf(p_log->p_log, "%s: %s[%d] %s\n", p_name, filename, filenumber, text);
	fflush(p_log->p_log);
}

void db_log_error(struct db_log * p_log, char * p_error, char * filename, int filenumber)
{
	FILE * log = NULL;

	if(p_log)
		log = p_log->p_log;

	if(NULL == log)
		log = stderr;

	const char * p_name = NULL;
	db_app_name_get_reference(gp_app, &p_name);
	fprintf(log, "%s: %s[%d] %s\n", p_name, filename, filenumber, p_error);

	exit(EXIT_FAILURE);
}

