#ifndef H_DB_LOG // H_DB_LOG
#	define H_DB_LOG // H_DB_LOG

#	include <stdbool.h>

struct db_log;
struct db_app;

void db_log_create(struct db_log ** pp_log);
void db_log_init(struct db_log * p_log);

void db_log_clone(struct db_log * p_log, struct db_log ** pp_log);
void db_log_copy(struct db_log * p_from, struct db_log * p_to);

void db_log_clean(struct db_log * p_log, bool has_to_dispose);
void db_log_dispose(struct db_log ** pp_log);

void db_log_open(struct db_log * p_log, char * filename);
void db_log_error(struct db_log * p_log, char * p_error, char * filename, int filenumber);
void db_log_write(struct db_log * p_log, char * text, char * filename, int filenumber);

#endif // H_DB_LOG

