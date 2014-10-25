
#include "error.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define FILE_LINE_FMT "%s:%d "
#define FMT_SIZE 1024U

#include <stdbool.h>

static bool _db_error_make_format(char ** pp_full_fmt, enum db_error_level level, char const * p_file, int const line, char const * p_fmt)
{
	size_t fmt_size = FMT_SIZE;
	size_t written = 0;

	char * p_level = NULL;
	switch(level)
	{
		case DB_ERROR_NOTICE:	p_level = "NOTICE"; break;
		case DB_ERROR_WARNING:	p_level = "WARNING"; break;
		case DB_ERROR_FAILURE:	p_level = "FAILURE"; break;
		default:				p_level = "(unknown)";
	}

	// Build the base format string
	do
	{
		*pp_full_fmt = realloc(*pp_full_fmt, fmt_size);
		if(NULL == *pp_full_fmt)
		{
			fprintf(stderr, FILE_LINE_FMT "%m", p_file, line);
			return false;
		}

		written = snprintf(*pp_full_fmt, fmt_size, "[%-10s] " FILE_LINE_FMT " %s", p_level, p_file, line, p_fmt);

		if(written < 0)
		{
			fprintf(stderr, FILE_LINE_FMT "%m", p_file, line);
			return false;
		}

		if(written < fmt_size)
			break;

		fmt_size = written + 1;
	}
	while(1);

	return true;
}

void db_error_printf(enum db_error_level level, char const * p_file, int const line, char const * p_fmt, ...)
{
	char * p_full_fmt = NULL;
	if(!_db_error_make_format(&p_full_fmt, level, p_file, line, p_fmt))
		exit(EXIT_FAILURE);

	// Lets atomically print the error message
	va_list ap;
	va_start(ap, p_fmt);
	vfprintf(stderr, p_full_fmt, ap);
	va_end(ap);

	if(p_full_fmt)
		free(p_full_fmt);

	if(DB_ERROR_FAILURE == level)
		exit(EXIT_FAILURE);
}

