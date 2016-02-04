#ifndef H_ZOB_LOG // H_ZOB_LOG
#	define H_ZOB_LOG // H_ZOB_LOG

#	include <stdbool.h>

struct zob_log;
struct zob_app;

void zob_log_create(struct zob_log ** pp_log);
void zob_log_init(struct zob_log * p_log);

void zob_log_clone(struct zob_log * p_log, struct zob_log ** pp_log);
void zob_log_copy(struct zob_log * p_from, struct zob_log * p_to);

void zob_log_clean(struct zob_log * p_log, bool has_to_dispose);
void zob_log_dispose(struct zob_log ** pp_log);

void zob_log_open(struct zob_log * p_log, char * filename);
void zob_log_error(struct zob_log * p_log, char * p_error, char * filename, int filenumber);
void zob_log_write(struct zob_log * p_log, char * text, char * filename, int filenumber);

#endif // H_ZOB_LOG

