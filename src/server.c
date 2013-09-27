
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

#include "request-builder.h"
#include "server.h"
#include "store.h"
#include "app.h"
#include "message.h"
#include "object.h"

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

APP_ALLOC(server)
APP_CREATE(server)

void db_server_init(struct db_server * p_server, struct db_app * p_app)
{
	p_server->p_app = p_app;

	p_server->remote_addr_size = sizeof p_server->remote_addr;
	p_server->session_fd = -1;

	memset(&p_server->self_addr, 0, sizeof p_server->self_addr);
	p_server->self_addr.sun_family = AF_UNIX;
}

void db_server_clean(struct db_server * p_server, bool has_to_dispose)
{
	if(has_to_dispose)
		if(p_server->p_store)
			db_store_dispose(&p_server->p_store);

	memset(p_server, 0, sizeof *p_server);
}

int db_server_run(struct db_server * p_server)
{
	daemon(1, 0);

	db_app_open_log(p_server->p_app, LOG_NAME);
	db_store_create(&p_server->p_store, p_server->p_app);

	db_server_listen(p_server, SOCK_NAME);
	do
	{
		struct db_request_builder * p_rb = NULL;
		db_request_builder_create(&p_rb, p_server->p_app);

		db_server_session_begin(p_server);

		char * p_buffer = NULL;

		bool need_moar = true;
		bool is_parse_error = false;
		do
		{
			db_server_read(p_server, &p_buffer);
			db_request_builder_parse(p_rb, p_buffer, &need_moar);
			free(p_buffer);

			db_request_builder_is_bad_request(p_rb, &is_parse_error);
			if(is_parse_error)
				need_moar = false;

		} while(need_moar);

		struct db_message * p_answer = NULL;
		if(is_parse_error)
		{
			db_server_answer(p_server, p_answer);
		}
		else
		{
			struct db_message * p_request = NULL;
			db_request_builder_get_request(p_rb, &p_request);

			db_message_clone(p_request, &p_answer);

			db_server_process(p_server, p_request, p_answer);
			db_server_answer(p_server, p_answer);
			db_message_dispose(&p_request);
		}

		db_message_dispose(&p_answer);
		db_request_builder_dispose(&p_rb);
		db_server_session_end(p_server);
	}
	while(true);

	db_store_dispose(&p_server->p_store);
}

void db_server_process(struct db_server * p_server, struct db_message * p_request, struct db_message * p_answer)
{
	const char * p_verb = NULL;
	char * p_key = NULL, * p_payload = NULL;
	db_message_get_verb(p_request, (char **) &p_verb);
	db_message_get_key(p_request, (char **) &p_key);

	if(0 == strcmp("read", p_verb))
	{
		/*
		bool has_found = false;
		db_store_read(p_server->fd, p_key, &p_payload, &has_found);
		if(!has_found)
		{
			goto end;
		}

		db_message_set_payload(p_answer, p_payload);
		*/

		// Trololol, I'm a read server!
		db_message_set_payload(p_answer, "Ko Write only");
	}
	else if(0 == strcmp("new", p_verb))
	{
		char * p_ticket = NULL;
		bool is_ok = false;

		db_message_get_payload(p_request, &p_payload);
		db_store_write(p_server->p_store, p_key, p_payload, &p_ticket, &is_ok);
		free(p_payload);

		if(is_ok)
		{
			char answer[100];
			snprintf(answer, 99, "Ok %s", p_ticket);
			db_message_set_payload(p_answer, answer);
		}
		else
		{
		}

		//db_message_set_payload(p_answer, "Ko\nRead only");
	}

	free(p_key);
}

void db_server_listen(struct db_server * p_server, const char * socket_path)
{
	p_server->socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
	CHECK_INT(p_server->p_app, p_server->socket_fd);

	strcpy(p_server->self_addr.sun_path, socket_path);

	CHECK_INT(p_server->p_app, bind(p_server->socket_fd
				, (struct sockaddr *) &p_server->self_addr
				, sizeof p_server->self_addr));

	CHECK_INT(p_server->p_app, listen(p_server->socket_fd, 0));
}

void db_server_session_begin(struct db_server * p_server)
{
	CHECK_INT(p_server->p_app, p_server->session_fd = accept(p_server->socket_fd
				, (struct sockaddr *) &p_server->remote_addr
				, &p_server->remote_addr_size));
}

void db_server_session_end(struct db_server * p_server)
{
	close(p_server->session_fd);
}

#define min(l, r) (((l) > (r)) ? (r) : (l))

void db_server_read(struct db_server * p_server, char **pp_payload)
{
	char * p_payload = NULL;
	int payload_size = 0;

	p_payload = calloc(DB_BUFFER_SIZE + 1, sizeof *p_payload);

	int reading_count = 0;
	do
	{
		reading_count = read(p_server->session_fd, &p_server->buffer, DB_BUFFER_SIZE);
		CHECK_INT(p_server->p_app, reading_count);

		if(0 == reading_count)
			break;

		payload_size += min(DB_BUFFER_SIZE, reading_count);
		p_payload = realloc(p_payload, (payload_size + 1) * sizeof *p_payload);
		CHECK_NULL(p_server->p_app, p_payload);
		strncat(p_payload, p_server->buffer, min(DB_BUFFER_SIZE, reading_count));

	} while(reading_count > DB_BUFFER_SIZE);

	*pp_payload = p_payload;
}

void db_server_answer(struct db_server * p_server, struct db_message * p_answer)
{
	char * p_verb = NULL, * p_key = NULL, * p_payload = NULL;
	db_message_get_verb(p_answer, &p_verb);
	db_message_get_key(p_answer, &p_key);
	db_message_get_payload(p_answer, &p_payload);

	char buffer[1024];
	memset(&buffer, 0, sizeof buffer);

	int write_count = snprintf(buffer, 1024 - 1, "Request '%s' key '%s' value '%s'"
			, p_verb
			, p_key
			, p_payload ? p_payload : "(null)"
	);

	int written = write(p_server->session_fd, buffer, min(write_count, 1024 - 1));
	CHECK_INT(p_server->p_app, written);

	free(p_key);
	free(p_payload);
}
