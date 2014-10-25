#ifndef H_DB_ERROR
#	define H_DB_ERROR

#	include <stdarg.h>

#	define DB_ERROR_FAIL_PRINTF(p_message, ...) db_error_printf(DB_ERROR_FAILURE, __FILE__, __LINE__, p_message, __VA_ARGS__)
#	define DB_ERROR_WARN_PRINTF(p_message, ...) db_error_printf(DB_ERROR_WARNING, __FILE__, __LINE__, p_message, __VA_ARGS__)
#	define DB_ERROR_NOTIFY_PRINTF(p_message, ...) db_error_printf(DB_ERROR_NOTICE, __FILE__, __LINE__, p_message, __VA_ARGS__)

enum db_error_level
{
	DB_ERROR_FAILURE,
	DB_ERROR_NOTICE,
	DB_ERROR_WARNING,
};

void db_error_printf(enum db_error_level level, char const * p_file, int const line, char const * p_fmt, ...);

#endif //H_DB_ERROR
