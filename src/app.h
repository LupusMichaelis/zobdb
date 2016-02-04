#ifndef H_DB_APP
#	define H_DB_APP

#	include <errno.h>
#	include <string.h>
#	include <stdio.h>
#	include <stdbool.h>

#	define CHECK_INT(int_val)\
		if((int_val) < 0) \
			db_app_error(gp_app, strerror(errno), __FILE__, __LINE__);

#	define CHECK_NULL(ptr_val)\
		if((ptr_val) == NULL) \
			db_app_error(gp_app, strerror(errno), __FILE__, __LINE__);

#	define DB_BUFFER_SIZE 4096

struct db_app;
struct db_allocator;

extern struct db_app * gp_app;

void db_app_singleton_set(struct db_app * p_app);

void db_app_alloc(struct db_app ** pp_app);
void db_app_init(struct db_app * p_app);
void db_app_setup(struct db_app * p_app);
void db_app_command(struct db_app * p_app, int argc, char ** argv);
void db_app_clean(struct db_app * p_app, bool has_to_dispose);

int db_app_run(struct db_app * p_app);

void db_app_allocator_get(struct db_app * p_app, struct db_allocator ** pp_allocator);

void db_app_config_get(struct db_app * p_app, char * p_name, char ** pp_value);
void db_app_name_get_reference(struct db_app * p_app, const char ** pp_name);

void db_app_open_log(struct db_app * p_app, char * filename);

void db_app_error(struct db_app * p_app, char * p_error, char * filename, int filenumber);
void db_app_log(struct db_app * p_app, char * text, char * filename, int filenumber);

#endif // H_DB_APP

