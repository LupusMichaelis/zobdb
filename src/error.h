#ifndef H_ZOB_ERROR
#	define H_ZOB_ERROR

#	include <stdarg.h>
#	include <stdbool.h>

#	define DB_ERROR_PRINTF(p_error, p_message, ...) zob_error_printf(p_error, __FILE__, __LINE__, p_message, __VA_ARGS__)

struct zob_error;

void zob_error_alloc(struct zob_error ** pp_error);
void zob_error_init(struct zob_error * p_error);

void zob_error_set_is_fatal(struct zob_error * p_error, bool is_fatal);
void zob_error_is_fatal(struct zob_error * p_error, bool * p_is_fatal);
void zob_error_printf(struct zob_error * p_error, char const * p_file, int const line, char const * p_fmt, ...);

#endif //H_ZOB_ERROR
