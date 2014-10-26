#ifndef H_DB_ERROR
#	define H_DB_ERROR

#	include <stdarg.h>
#	include <stdbool.h>

#	define DB_ERROR_PRINTF(p_error, p_message, ...) db_error_printf(p_error, __FILE__, __LINE__, p_message, __VA_ARGS__)

struct db_error;

void db_error_alloc(struct db_error ** pp_error);
void db_error_init(struct db_error * p_error);

void db_error_set_is_fatal(struct db_error * p_error, bool is_fatal);
void db_error_is_fatal(struct db_error * p_error, bool * p_is_fatal);
void db_error_printf(struct db_error * p_error, char const * p_file, int const line, char const * p_fmt, ...);

#endif //H_DB_ERROR
