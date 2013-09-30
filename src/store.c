#include "store.h"
#include "app.h"
#include "object.h"

#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

struct db_store
{
	struct db_app * p_app;
	int fd;
};


APP_ALLOC(store)
APP_CREATE(store)
APP_DISPOSE(store)
APP_CLONE(store)

void db_store_init(struct db_store * p_store, struct db_app * p_app)
{
	p_store->p_app = p_app;

	const char * filename = NULL;
	db_app_config_get(p_store->p_app, "store", (char **)&filename);
	p_store->fd = open(filename, 0600 | O_APPEND | O_WRONLY);

	CHECK_INT(p_store->p_app, p_store->fd);
}

void db_store_generate_ticket(struct db_store * p_store, char ** pp_ticket)
{
	// TODO a unique id ::= pid | ' ' | sequence-number
	(void) p_store;
	*pp_ticket = "1234";
}

static void do_write(struct db_store * p_store, const char * p_text)
{
	int writting_count = write(p_store->fd, p_text, strlen(p_text));
	CHECK_INT(p_store->p_app, writting_count);
	if(strlen(p_text) != writting_count)
		db_app_error(p_store->p_app, "IO mismatch", __FILE__, __LINE__);

}

void db_store_write(
		struct db_store * p_store,
		const char * p_key,
		const char * p_value,
		char ** pp_ticket,
		bool * is_ok)
{
	char * p_ticket = NULL;
	db_store_generate_ticket(p_store, &p_ticket);

	do_write(p_store, p_key);
	do_write(p_store, " ");
	do_write(p_store, p_ticket);
	do_write(p_store, "\n");
	do_write(p_store, p_value);
	do_write(p_store, "\n\n");

	*pp_ticket = p_ticket;
	*is_ok = true;
}

void db_store_copy(struct db_store * p_from, struct db_store * p_to)
{
	db_app_error(p_from->p_app, "Not implemented", __FILE__, __LINE__);
}

void db_store_clean(struct db_store * p_store, bool has_to_dispose)
{
	if(has_to_dispose)
		if(p_store->fd)
			close(p_store->fd);

	memset(p_store, 0, sizeof *p_store);
}

