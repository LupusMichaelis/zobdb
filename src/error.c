
#include "error.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define FILE_LINE_FMT "%s:%d "
#define FMT_SIZE 1024U

#include <stdbool.h>

struct zob_error
{
	bool is_fatal;
};

////////////////////////////////////////////////////////////////////////////////
static bool _zob_error_make_format(char ** pp_full_fmt, char const * p_file, int const line, char const * p_fmt)
{
	size_t fmt_size = FMT_SIZE;
	int written = 0;

	// Build the base format string
	do
	{
		*pp_full_fmt = realloc(*pp_full_fmt, fmt_size);
		if(NULL == *pp_full_fmt)
		{
			fprintf(stderr, FILE_LINE_FMT "%m\n", p_file, line);
			return false;
		}

		written = snprintf(*pp_full_fmt, fmt_size, FILE_LINE_FMT " %s\n", p_file, line, p_fmt);

		if(written < 0)
		{
			fprintf(stderr, FILE_LINE_FMT "%m\n", p_file, line);
			return false;
		}

		if(written < fmt_size)
			break;

		fmt_size = written + 1;
	}
	while(1);

	return true;
}

static void _zob_error_printf(struct zob_error * p_error, char const * p_file, int const line, char const * p_fmt, va_list ap)
{
	char * p_full_fmt = NULL;
	if(!_zob_error_make_format(&p_full_fmt, p_file, line, p_fmt))
		exit(EXIT_FAILURE);

	// Lets atomically print the error message
	vfprintf(stderr, p_full_fmt, ap);

	if(p_full_fmt)
		free(p_full_fmt);

	if(!p_error || p_error->is_fatal)
		exit(EXIT_FAILURE);
}


////////////////////////////////////////////////////////////////////////////////
void zob_error_alloc(struct zob_error ** pp_error)
{
	struct zob_error * p = NULL;
	p = calloc(1, sizeof *p);
	if(NULL == p)
	{
		fprintf(stderr, FILE_LINE_FMT " %m\n", __FILE__, __LINE__);
		exit(EXIT_FAILURE);
	}
	*pp_error = p;
}

void zob_error_printf(struct zob_error * p_error, char const * p_file, int const line, char const * p_fmt, ...)
{
	va_list ap;
	va_start(ap, p_fmt);
	_zob_error_printf(p_error, p_file, line, p_fmt, ap);
	va_end(ap);
}

void zob_error_init(struct zob_error * p_error)
{
	p_error->is_fatal = true;
}

void zob_error_set_is_fatal(struct zob_error * p_error, bool is_fatal)
{
	p_error->is_fatal = is_fatal;
}

void zob_error_is_fatal(struct zob_error * p_error, bool * p_is_fatal)
{
	*p_is_fatal = p_error->is_fatal;
}
