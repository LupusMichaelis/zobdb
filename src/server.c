
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

void db_server_init(struct db_server * p_db, struct db_app * p_app)
{
	p_db->p_app = p_app;

	p_db->remote_addr_size = sizeof p_db->remote_addr;
	p_db->session_fd = -1;

	memset(&p_db->self_addr, 0, sizeof p_db->self_addr);
	p_db->self_addr.sun_family = AF_UNIX;
}

int db_server_run(struct db_server * p_server)
{
	daemon(1, 0);

	db_app_open_log(p_server->p_app, LOG_NAME);
	db_app_store_create(&p_server->p_store, p_server->p_app);

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
			db_app_store_write(p_server->p_store, p_buffer);
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

			char * p_verb = NULL, * p_key = NULL;
			db_message_get_verb(p_request, &p_verb);
			db_message_get_key(p_request, &p_key);
			fprintf(stderr, "Request '%s' key '%s'\n", p_verb, p_key);
			free(p_key);

			db_server_answer(p_server, p_answer);
			db_message_dispose(&p_request);
		}

		db_message_dispose(&p_answer);

		db_request_builder_dispose(&p_rb);
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

void db_server_answer(struct db_server * p_db, struct db_message * p_answer)
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

	int written = write(p_db->session_fd, buffer, min(write_count, 1024 - 1));
	CHECK_INT(p_db->p_app, written);

	free(p_key);
	free(p_payload);
}
