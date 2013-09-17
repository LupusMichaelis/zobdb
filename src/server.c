
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
	if(NULL == p_db)
		db_app_error(p_app, strerror(errno), __FILE__, __LINE__);

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

void db_server_open_store(struct db_server * p_db, const char * filename)
{
	p_db->store_fd = open("./datas", 0600 | O_APPEND | O_WRONLY);
	CHECK(p_db->p_app, p_db->store_fd);
}

void db_server_listen(struct db_server * p_db, const char * socket_path)
{
	p_db->socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
	CHECK(p_db->p_app, p_db->socket_fd);

	strcpy(p_db->self_addr.sun_path, socket_path);

	CHECK(p_db->p_app, bind(p_db->socket_fd
				, (struct sockaddr *) &p_db->self_addr
				, sizeof p_db->self_addr));

	CHECK(p_db->p_app, listen(p_db->socket_fd, 0));
}

void db_server_accept_begin(struct db_server * p_db)
{
	CHECK(p_db->p_app, p_db->session_fd = accept(p_db->socket_fd
				, (struct sockaddr *) &p_db->remote_addr
				, &p_db->remote_addr_size));
}

void db_server_accept_end(struct db_server * p_db)
{
	close(p_db->session_fd);
}

static char buffer[4096];

void db_server_read(struct db_server * p_db)
{
	int reading_count = read(p_db->session_fd, &p_db->buffer, DB_BUFFER_SIZE);
	CHECK(p_db->p_app, reading_count);
	p_db->buffer[reading_count] = '\0';

	if(0 == reading_count)
		return;

	int string_size = snprintf(buffer, 4096, "% -5d:%s\n", getpid(), p_db->buffer);
	if(string_size >= 4096)
		db_app_error(p_db->p_app, "Data truncated", __FILE__, __LINE__);

	int writting_count = write(p_db->store_fd, &buffer, string_size);
	CHECK(p_db->p_app, writting_count);

	if(string_size != writting_count)
		db_app_error(p_db->p_app, "IO mismatch", __FILE__, __LINE__);
}

void db_server_answer(struct db_server * p_db, const char * message)
{
	int writting_count = write(p_db->session_fd, message, strlen(message));
	CHECK(p_db->p_app, writting_count);
}
