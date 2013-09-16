
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
	int io_fd;
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
	p_db->io_fd = -1;

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

	CHECK(p_db->p_app, listen(p_db->socket_fd, 10));
}

void db_server_accept_begin(struct db_server * p_db)
{
	CHECK(p_db->p_app, p_db->io_fd = accept(p_db->socket_fd
				, (struct sockaddr *) &p_db->remote_addr
				, &p_db->remote_addr_size));
}

void db_server_accept_end(struct db_server * p_db)
{
	close(p_db->io_fd);
}

void db_server_read(struct db_server * p_db)
{
	int reading_count = recv(p_db->io_fd, &p_db->buffer, DB_BUFFER_SIZE, 0);
	CHECK(p_db->p_app, reading_count);

	if(0 == reading_count)
		return;

	int writting_count = write(p_db->store_fd, &p_db->buffer, reading_count);
	CHECK(p_db->p_app, writting_count);

	if(reading_count != writting_count)
		db_app_error(p_db->p_app, "IO mismatch", __FILE__, __LINE__);
		//fprintf(stderr, "%d of %d bytes written\n", writting_count, reading_count);
}
