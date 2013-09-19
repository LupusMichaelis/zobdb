
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

#include "server.h"
#include "store.h"
#include "app.h"

struct db_app;
struct db_store;

struct db_server
{
	struct db_app * p_app;
	char buffer[DB_BUFFER_SIZE];

	int socket_fd;
	int session_fd;

	struct sockaddr_un self_addr;
	struct sockaddr_un remote_addr;
	socklen_t remote_addr_size;

	struct db_store * p_store;
};

void db_server_new(struct db_server ** pp_db, struct db_app * p_app)
{
	struct db_server * p_db = NULL;
	p_db = calloc(1, sizeof *p_db);
	CHECK_NULL(p_app, p_db);

	p_db->p_app = p_app;
	*pp_db = p_db;
}

void db_server_init(struct db_server * p_db)
{
	p_db->remote_addr_size = sizeof p_db->remote_addr;
	p_db->session_fd = -1;

	memset(&p_db->self_addr, 0, sizeof p_db->self_addr);
	p_db->self_addr.sun_family = AF_UNIX;
}

void db_server_create(struct db_server ** pp_server, struct db_app * p_app)
{
	struct db_server * p_server = NULL;
	db_server_new(&p_server, p_app);
	db_server_init(p_server);

	*pp_server = p_server;
}

int db_server_run(struct db_server * p_server)
{
	daemon(1, 0);

	db_app_open_log(p_server->p_app, LOG_NAME);
	db_app_store_create(&p_server->p_store, p_server->p_app);

	db_server_listen(p_server, SOCK_NAME);
	do
	{
		db_server_session_begin(p_server);

		char * p_buffer = NULL;
		db_server_read(p_server, &p_buffer);
		db_app_store_write(p_server->p_store, p_buffer);
		free(p_buffer);

		db_server_answer(p_server, "Ok");
		db_server_session_end(p_server);
	}
	while(true);
}

void db_server_listen(struct db_server * p_db, const char * socket_path)
{
	p_db->socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
	CHECK_INT(p_db->p_app, p_db->socket_fd);

	strcpy(p_db->self_addr.sun_path, socket_path);

	CHECK_INT(p_db->p_app, bind(p_db->socket_fd
				, (struct sockaddr *) &p_db->self_addr
				, sizeof p_db->self_addr));

	CHECK_INT(p_db->p_app, listen(p_db->socket_fd, 0));
}

void db_server_session_begin(struct db_server * p_db)
{
	CHECK_INT(p_db->p_app, p_db->session_fd = accept(p_db->socket_fd
				, (struct sockaddr *) &p_db->remote_addr
				, &p_db->remote_addr_size));
}

void db_server_session_end(struct db_server * p_db)
{
	close(p_db->session_fd);
}

#define min(l, r) (((l) > (r)) ? (r) : (l))

void db_server_read(struct db_server * p_db, char **pp_payload)
{
	char * p_payload = NULL;
	int payload_size = 0;

	p_payload = calloc(DB_BUFFER_SIZE + 1, sizeof *p_payload);
	*p_payload = '\0';

	int reading_count = 0;
	do
	{
		reading_count = read(p_db->session_fd, &p_db->buffer, DB_BUFFER_SIZE);
		CHECK_INT(p_db->p_app, reading_count);

		if(0 == reading_count)
			break;

		payload_size += min(DB_BUFFER_SIZE, reading_count);
		p_payload = realloc(p_payload, (payload_size + 1) * sizeof *p_payload);
		CHECK_NULL(p_db->p_app, p_payload);
		strncat(p_payload, p_db->buffer, min(DB_BUFFER_SIZE, reading_count));

	} while(reading_count > DB_BUFFER_SIZE);

	*pp_payload = p_payload;
}

void db_server_answer(struct db_server * p_db, const char * message)
{
	int writting_count = write(p_db->session_fd, message, strlen(message));
	CHECK_INT(p_db->p_app, writting_count);
}
