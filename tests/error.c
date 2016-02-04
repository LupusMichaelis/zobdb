#include "../src/error.h"

#include <assert.h>
#include <stdio.h>
#include <stdbool.h>

//void * malloc(size_t);
//void free(void *);
int atexit(void (*function)(void));

static bool should_ve_exited = false;
void called_exit(void)
{
	if(should_ve_exited)
		printf("Huge success\n");
	else
		assert(0);
}

int main(int argc, char *argv[])
{
	atexit(called_exit);

	struct db_error * p_error = NULL;
	db_error_alloc(&p_error);
	db_error_init(p_error);
	db_error_set_is_fatal(p_error, false);

	DB_ERROR_PRINTF(p_error, "%s", "This message should be a notice");
	DB_ERROR_PRINTF(p_error, "%s", "This message should be displayed and app continued");

	db_error_set_is_fatal(p_error, true);
	should_ve_exited = true;
	DB_ERROR_PRINTF(p_error, "%s", "This message should be displayed and app exited");
	assert(0);

	return 0;
}



