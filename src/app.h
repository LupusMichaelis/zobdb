#ifndef H_ZOB_APP
#	define H_ZOB_APP

#	include <errno.h>
#	include <string.h>
#	include <stdio.h>
#	include <stdbool.h>

#	define MIN(A, B) \
	((A) < (B) ? (A) : (B))

#	define MAX(A, B) \
	((A) > (B) ? (A) : (B))

#	define CHECK_INT(int_val)\
		if((int_val) < 0) \
			zob_app_error(gp_app, strerror(errno), __FILE__, __LINE__);

#	define CHECK_NULL(ptr_val)\
		if((ptr_val) == NULL) \
			zob_app_error(gp_app, strerror(errno), __FILE__, __LINE__);

#	define DB_BUFFER_SIZE 4096

struct zob_app;
struct zob_allocator;
struct zob_string;

enum zob_log_level;

extern struct zob_app * gp_app;

void zob_app_singleton_set(struct zob_app * p_app);

void zob_app_alloc(struct zob_app ** pp_app);
void zob_app_dispose(struct zob_app ** pp_app);
void zob_app_init(struct zob_app * p_app);
void zob_app_setup(struct zob_app * p_app);
void zob_app_command(struct zob_app * p_app, int argc, char ** argv);
void zob_app_clean(struct zob_app * p_app, bool has_to_dispose);

int zob_app_run(struct zob_app * p_app);

void zob_app_allocator_get(struct zob_app * p_app, struct zob_allocator ** pp_allocator);

void zob_app_config_get(struct zob_app * p_app, char * p_name, char ** pp_value);
void zob_app_name_get_reference(struct zob_app * p_app, const char ** pp_name);

void zob_app_open_log(struct zob_app * p_app, char * filename);

void zob_app_error(struct zob_app * p_app, char * p_error, char * filename, int filenumber);
void zob_app_log(struct zob_app * p_app
	, char * filename, int filenumber
	, enum zob_log_level log_level, struct zob_string * p_text);

#endif // H_ZOB_APP

