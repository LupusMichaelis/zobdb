#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>

#include "client.h"
#include "app.h"

struct db_client
{
	struct db_app * p_app;
	char buffer[DB_BUFFER_SIZE];

	int socket_fd;

	struct sockaddr_un remote_addr;
	socklen_t remote_addr_size;
};

void db_client_new(struct db_client ** pp_db, struct db_app * p_app)
{
	struct db_client * p_db = NULL;
	p_db = calloc(1, sizeof *p_db);
	if(NULL == p_db)
		db_app_error(p_app, strerror(errno), __FILE__, __LINE__);

	p_db->remote_addr_size = sizeof p_db->remote_addr;
	p_db->p_app = p_app;
	*pp_db = p_db;
}

void db_client_init(struct db_client * p_db)
{
	p_db->remote_addr.sun_family = AF_UNIX;
}

void db_client_connect(struct db_client * p_db, char const * sockname)
{
	strcpy(p_db->remote_addr.sun_path, sockname);

	p_db->socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
	CHECK(p_db->p_app, p_db->socket_fd);

	CHECK(p_db->p_app, connect(p_db->socket_fd
				, (struct sockaddr *) &p_db->remote_addr, sizeof p_db->remote_addr));
}

void db_client_send(struct db_client * p_db, int in_fd)
{
	do
	{
		int reading_count = read(in_fd, &p_db->buffer, DB_BUFFER_SIZE);
		CHECK(p_db->p_app, reading_count);

		if(0 == reading_count)
			break;

		int writting_count = write(p_db->socket_fd, &p_db->buffer, reading_count);
		CHECK(p_db->p_app, writting_count);

		if(reading_count != writting_count)
			db_app_error(p_db->p_app, "IO mismatch", __FILE__, __LINE__);
	}
	while(true);
}

void db_client_recv(struct db_client * p_db)
{

	int reading_count = read(p_db->socket_fd, &p_db->buffer, DB_BUFFER_SIZE);
	CHECK(p_db->p_app, reading_count);
	p_db->buffer[reading_count] = '\0';

	if(0 == strcmp("OK", p_db->buffer))
		db_app_error(p_db->p_app, "No ack", __FILE__, __LINE__);
}
