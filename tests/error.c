#include "../src/error.h"

#include <assert.h>
#include <stdio.h>

//void * malloc(size_t);
//void free(void *);
int atexit(void (*function)(void));

void called_exit(void)
{
	printf("Huge success\n");
}

int main(int argc, char *argv[])
{
	atexit(called_exit);
	DB_ERROR_NOTIFY_PRINTF("%s\n", "This message should be a notice");
	DB_ERROR_WARN_PRINTF("%s\n", "This message should be displayed and app continued");
	DB_ERROR_FAIL_PRINTF("%s\n", "This message should be displayed and app exited");
	assert(0);


	return 0;
}



