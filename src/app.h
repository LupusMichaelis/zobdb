#ifndef H_DB_APP
#	define H_DB_APP

#	include <errno.h>
#	include <stdio.h>

#	define CHECK(p_app, int_val)\
		if((int_val) < 0) \
			db_app_error(p_app, strerror(errno), __FILE__, __LINE__);

#	define STORE_NAME "./datas"
#	define SOCK_NAME "./con"

#	define DB_BUFFER_SIZE 4096

struct db_app;

void db_app_new(struct db_app ** pp_app);
void db_app_init(struct db_app * p_app, int argc, char ** argv);
void db_app_error(struct db_app * p_app, const char * p_error, const char * filename, int filenumber);

#endif // H_DB_APP

