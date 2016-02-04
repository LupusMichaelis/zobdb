#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>

#include "client.h"
#include "app.h"
#include "object.h"

struct zob_client
{
	char buffer[DB_BUFFER_SIZE];

	int socket_fd;
	bool wait_moar;

	struct sockaddr_un remote_addr;
	socklen_t remote_addr_size;
};

APP_ALLOC(client)
APP_CREATE(client)

void zob_client_init(struct zob_client * p_client)
{
	p_client->remote_addr_size = sizeof p_client->remote_addr;
	p_client->remote_addr.sun_family = AF_UNIX;
}

void zob_client_clean(struct zob_client * p_client, bool has_to_dispose)
{
	if(has_to_dispose)
		if(p_client->socket_fd > 0)
			close(p_client->socket_fd);

	memset(p_client, 0, sizeof *p_client);
}

int zob_client_run(struct zob_client * p_client)
{
	char * p_sock_name = NULL;
	zob_app_config_get(gp_app, "socket.name", &p_sock_name);

	zob_client_connect(p_client, p_sock_name);
	zob_client_send(p_client, fileno(stdin));
	do zob_client_recv(p_client); while(p_client->wait_moar);

	return EXIT_SUCCESS;
}

void zob_client_connect(struct zob_client * p_client, char const * sockname)
{
	strcpy(p_client->remote_addr.sun_path, sockname);

	p_client->socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
	CHECK_INT(p_client->socket_fd);

	CHECK_INT(connect(p_client->socket_fd
				, (struct sockaddr *) &p_client->remote_addr, sizeof p_client->remote_addr));
}

void zob_client_send(struct zob_client * p_client, int in_fd)
{
	int reading_count = read(in_fd, &p_client->buffer, DB_BUFFER_SIZE);
	CHECK_INT(reading_count);

	if(0 == reading_count)
		return;

	int writting_count = write(p_client->socket_fd, &p_client->buffer, reading_count);
	CHECK_INT(writting_count);

	if(reading_count != writting_count)
		zob_app_error(gp_app, "IO mismatch", __FILE__, __LINE__);
}

void zob_client_recv(struct zob_client * p_client)
{
	int reading_count = read(p_client->socket_fd, &p_client->buffer, DB_BUFFER_SIZE);
	CHECK_INT(reading_count);
	p_client->buffer[reading_count] = '\0';
	printf("%s\n", p_client->buffer);
	if(p_client->buffer != strstr(p_client->buffer, "Ok"))
		zob_app_error(gp_app, "No ack", __FILE__, __LINE__);
}
