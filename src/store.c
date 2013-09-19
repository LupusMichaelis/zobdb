#include "store.h"
#include "app.h"

#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

struct db_store
{
	struct db_app * p_app;

	int fd;
};


void db_app_store_new(struct db_store ** pp_store, struct db_app * p_app)
{
	struct db_store * p_store = NULL;
	p_store = calloc(1, sizeof *p_store);
	CHECK_NULL(p_app, p_store);

	p_store->p_app = p_app;
	*pp_store = p_store;
}

void db_app_store_init(struct db_store * p_store)
{
	const char * filename = NULL;
	db_app_config_get(p_store->p_app, "store", (void **)&filename);
	p_store->fd = open(filename, 0600 | O_APPEND | O_WRONLY);

	CHECK_INT(p_store->p_app, p_store->fd);
}

void db_app_store_create(struct db_store ** pp_store, struct db_app * p_app)
{
	struct db_store * p_store = NULL;
	db_app_store_new(&p_store, p_app);
	db_app_store_init(p_store);

	*pp_store = p_store;
}

void db_app_store_write(struct db_store * p_store, const char * text)
{
	int writting_count = write(p_store->fd, text, strlen(text));
	CHECK_INT(p_store->p_app, writting_count);

	if(strlen(text) != writting_count)
		db_app_error(p_store->p_app, "IO mismatch", __FILE__, __LINE__);
}

