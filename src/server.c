
#include "server.h"
#include "app.h"

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

struct db_app;

struct db_server
{
	struct db_app * p_app;
	char buffer[DB_BUFFER_SIZE];

	int socket_fd;
	int session_fd;
	int store_fd;

	struct sockaddr_un self_addr;
	struct sockaddr_un remote_addr;
	socklen_t remote_addr_size;
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

void db_server_store_open(struct db_server * p_db, const char * filename)
{
	p_db->store_fd = open(filename, 0600 | O_APPEND | O_WRONLY);
	CHECK_INT(p_db->p_app, p_db->store_fd);
}

void db_server_store_write(struct db_server * p_db, const char * text)
{
	int writting_count = write(p_db->store_fd, text, strlen(text));
	CHECK_INT(p_db->p_app, writting_count);

	if(strlen(text) != writting_count)
		db_app_error(p_db->p_app, "IO mismatch", __FILE__, __LINE__);
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

		payload_size += reading_count;
		p_payload = realloc(p_payload, (payload_size + 1) * sizeof *p_payload);
		CHECK_NULL(p_db->p_app, p_payload);
		strncpy(p_payload + payload_size - reading_count, p_db->buffer, min(DB_BUFFER_SIZE, reading_count));
		*(p_payload + payload_size) = '\0';


	} while(reading_count > DB_BUFFER_SIZE);

	*pp_payload = p_payload;
}


void db_server_answer(struct db_server * p_db, const char * message)
{
	int writting_count = write(p_db->session_fd, message, strlen(message));
	CHECK_INT(p_db->p_app, writting_count);
}
