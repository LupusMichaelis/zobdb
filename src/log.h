#ifndef H_ZOB_LOG // H_ZOB_LOG
#	define H_ZOB_LOG // H_ZOB_LOG

#	include <stdbool.h>

struct zob_app;
struct zob_string;
struct zob_log;

void zob_log_create(struct zob_log ** pp_log);
void zob_log_init(struct zob_log * p_log);

void zob_log_clone(struct zob_log * p_log, struct zob_log ** pp_log);
void zob_log_copy(struct zob_log * p_from, struct zob_log * p_to);

void zob_log_clean(struct zob_log * p_log, bool has_to_dispose);
void zob_log_dispose(struct zob_log ** pp_log);

void zob_log_open(struct zob_log * p_log, struct zob_string * p_filename);
void zob_log_error(struct zob_log * p_log, char * p_error, char * filename, int filenumber);
//void zob_log_write(struct zob_log * p_log, char * text, char * filename, int filenumber);

enum zob_log_level
{
	ZOB_LOG_LEVEL_INFO,
	ZOB_LOG_LEVEL_NOTICE,
	ZOB_LOG_LEVEL_DEBUG,
	ZOB_LOG_LEVEL_WARNING,
	ZOB_LOG_LEVEL_ERROR,
};

void zob_log_write
	( struct zob_log * p_log
	, char * filename
	, int filenumber
	, enum zob_log_level log_level
	, struct zob_string * p_text);

#endif // H_ZOB_LOG

