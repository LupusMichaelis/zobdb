#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/un.h>

#include <stdbool.h>

char buffer[1024];

#define CHECK(int_val)	if((int_val) < 0) { perror(*argv); return EXIT_FAILURE; }
#define STEP(step)		fprintf(stderr, "%s\n", "" step);

#define SOCK_NAME "./con"

int main(int argc, char *argv[])
{
	int io = socket(AF_UNIX, SOCK_STREAM, 0);
	CHECK(io);

	struct sockaddr_un addr;
	memset(&addr, 0, sizeof addr);
	addr.sun_family = AF_UNIX;
	strcpy(addr.sun_path, SOCK_NAME);

	STEP("Connect");
	CHECK(connect(io, (struct sockaddr *) &addr, sizeof addr));
	STEP("/Connect");

	while(true)
	{
		int reading_count = read(STDIN_FILENO, &buffer, 1024);
		CHECK(reading_count);
		
		if(0 == reading_count)
			break;

		STEP("Write");
		int writting_count = write(io, &buffer, reading_count);
		STEP("/Write");
		CHECK(writting_count);

		if(reading_count != writting_count)
		{
			fprintf(stderr, "%d of %d bytes written\n", writting_count, reading_count);
			return EXIT_FAILURE;
		}
	}

	return EXIT_SUCCESS;
}


