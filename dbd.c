#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#include <sys/socket.h>
#include <sys/un.h>

#include <stdbool.h>

char buffer[1024];

#define CHECK(int_val)\
	if((int_val) < 0) {\
		fprintf(stderr, "%s: %s %s[%d]", *argv, strerror(errno), __FILE__, __LINE__);\
		return EXIT_FAILURE;\
	}

#define STEP(step)		fprintf(stderr, "%s:%d %s\n", __FILE__, __LINE__, "" step);

#define SOCK_NAME "./con"

int socket_fd = -1;
int io = -1;

void my_exit()
{ 
	if(socket_fd > -1) close(socket_fd);
	socket_fd = -1;
	unlink(SOCK_NAME);
}

void my_signal(int s) { exit(0); }

int main(int argc, char *argv[])
{
	atexit(my_exit);
	signal(SIGINT, my_signal);

	socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
	CHECK(socket_fd);

	struct sockaddr_un addr;
	memset(&addr, 0, sizeof addr);
	addr.sun_family = AF_UNIX;
	strcpy(addr.sun_path, SOCK_NAME);

	CHECK(bind(socket_fd, (struct sockaddr *) &addr, sizeof addr));
	STEP("Listen");
	CHECK(listen(socket_fd, 10));
	STEP("/Listen");

	STEP("Open");
	int store = open("./datas", 0600 | O_APPEND | O_WRONLY);
	CHECK(store);
	STEP("/Open");

	struct sockaddr_un remote;
	socklen_t remote_addr_size = sizeof remote;

	while(true)
	{
		STEP("Accept");
		CHECK(io = accept(socket_fd, (struct sockaddr *) &remote, &remote_addr_size));
		STEP("/Accept");

		while(true)
		{
			STEP("Recv");
			int reading_count = recv(io, &buffer, 1024, 0);
			CHECK(reading_count);
			STEP("/Recv");

			if(0 == reading_count)
				break;

			STEP("Write");
			int writting_count = write(store, &buffer, reading_count);
			CHECK(writting_count);
			STEP("/Write");

			if(reading_count != writting_count)
			{
				fprintf(stderr, "%d of %d bytes written\n", writting_count, reading_count);
				return EXIT_FAILURE;
			}
		}
		close(io);
	}

	fprintf(stderr, "Get out!\n");

	return EXIT_SUCCESS;
}
