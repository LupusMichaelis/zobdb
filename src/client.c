#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>

#include "client.h"
#include "app.h"
#include "object.h"

struct db_client
{
	struct db_app * p_app;
	char buffer[DB_BUFFER_SIZE];

	int socket_fd;
	bool wait_moar;

	struct sockaddr_un remote_addr;
	socklen_t remote_addr_size;
};

APP_ALLOC(client)
APP_CREATE(client)

void db_client_init(struct db_client * p_client, struct db_app * p_app)
{
	p_client->p_app = p_app;
	p_client->remote_addr_size = sizeof p_client->remote_addr;
	p_client->remote_addr.sun_family = AF_UNIX;
}

void db_client_clean(struct db_client * p_client, bool has_to_dispose)
{
	if(has_to_dispose)
		if(p_client->socket_fd > 0)
			close(p_client->socket_fd);

	memset(p_client, 0, sizeof *p_client);
}

int db_client_run(struct db_client * p_client)
{
	char * p_sock_name = NULL;
	db_app_config_get(p_client->p_app, "socket", &p_sock_name);

	db_client_connect(p_client, p_sock_name);
	db_client_send(p_client, fileno(stdin));
	do db_client_recv(p_client); while(p_client->wait_moar);

	return EXIT_SUCCESS;
}

void db_client_connect(struct db_client * p_client, char const * sockname)
{
	strcpy(p_client->remote_addr.sun_path, sockname);

	p_client->socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
	CHECK_INT(p_client->p_app, p_client->socket_fd);

	CHECK_INT(p_client->p_app, connect(p_client->socket_fd
				, (struct sockaddr *) &p_client->remote_addr, sizeof p_client->remote_addr));
}

void db_client_send(struct db_client * p_client, int in_fd)
{
	int reading_count = read(in_fd, &p_client->buffer, DB_BUFFER_SIZE);
	CHECK_INT(p_client->p_app, reading_count);

	if(0 == reading_count)
		return;

	int writting_count = write(p_client->socket_fd, &p_client->buffer, reading_count);
	CHECK_INT(p_client->p_app, writting_count);

	if(reading_count != writting_count)
		db_app_error(p_client->p_app, "IO mismatch", __FILE__, __LINE__);
}

void db_client_recv(struct db_client * p_client)
{
	int reading_count = read(p_client->socket_fd, &p_client->buffer, DB_BUFFER_SIZE);
	CHECK_INT(p_client->p_app, reading_count);
	p_client->buffer[reading_count] = '\0';

	if(0 != strcmp("Ok", p_client->buffer))
		db_app_error(p_client->p_app, "No ack", __FILE__, __LINE__);

	p_client->wait_moar = false;
}
