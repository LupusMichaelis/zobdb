#ifndef H_DB_APP
#	define H_DB_APP

#	include <errno.h>
#	include <string.h>
#	include <stdio.h>

#	define CHECK_INT(p_app, int_val)\
		if((int_val) < 0) \
			db_app_error(p_app, strerror(errno), __FILE__, __LINE__);

#	define CHECK_NULL(p_app, ptr_val)\
		if((ptr_val) == NULL) \
			db_app_error(p_app, strerror(errno), __FILE__, __LINE__);

#	define DB_BUFFER_SIZE 4096

struct db_app;

void db_app_alloc(struct db_app ** pp_app);
void db_app_init(struct db_app * p_app, int argc, char ** argv);

int db_app_run(struct db_app * p_app);

void db_app_config_get(struct db_app * p_app, char * p_name, char ** pp_value);

void db_app_open_log(struct db_app * p_app, char * filename);

void db_app_error(struct db_app * p_app, char * p_error, char * filename, int filenumber);
void db_app_log(struct db_app * p_app, char * text, char * filename, int filenumber);

#endif // H_DB_APP

