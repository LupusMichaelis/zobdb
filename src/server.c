
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
#include "log.h"

struct zob_app;
struct zob_store;

struct zob_server
{
	char buffer[DB_BUFFER_SIZE];

	bool is_running;

	int socket_fd;
	int session_fd;

	struct sockaddr_un self_addr;
	struct sockaddr_un remote_addr;
	socklen_t remote_addr_size;

	struct zob_store * p_store;
};

APP_ALLOC(server)
APP_CREATE(server)

void zob_server_init(struct zob_server * p_server)
{
	p_server->is_running = true;

	p_server->remote_addr_size = sizeof p_server->remote_addr;
	p_server->session_fd = -1;

	memset(&p_server->self_addr, 0, sizeof p_server->self_addr);
	p_server->self_addr.sun_family = AF_UNIX;
}

void zob_server_clean(struct zob_server * p_server, bool has_to_dispose)
{
	if(has_to_dispose)
		if(p_server->p_store)
			zob_store_dispose(&p_server->p_store);

	memset(p_server, 0, sizeof *p_server);
}

int zob_server_run(struct zob_server * p_server)
{
	//daemon(1, 0);

	char * p_sock_name = NULL;
	zob_app_config_get(gp_app, "socket.name", &p_sock_name);

	zob_store_create(&p_server->p_store);
	zob_server_listen(p_server, p_sock_name);
	do
	{
		struct zob_request_builder * p_rb = NULL;
		zob_request_builder_create(&p_rb);

		zob_server_session_begin(p_server);

		char * p_buffer = NULL;

		bool need_moar = true;
		bool is_parse_error = false;
		do
		{
			zob_server_read(p_server, &p_buffer);
			zob_request_builder_parse(p_rb, p_buffer, &need_moar);
			free(p_buffer);

			zob_request_builder_is_bad_request(p_rb, &is_parse_error);
			if(is_parse_error)
				need_moar = false;

		} while(need_moar);

		struct zob_message * p_answer = NULL;
		if(is_parse_error)
		{
			zob_message_create(&p_answer);
			zob_message_set_verb(p_answer, "KO");
			zob_message_set_payload(p_answer, "Parse error");
			zob_server_answer(p_server, p_answer);
		}
		else
		{
			struct zob_message * p_request = NULL;
			zob_request_builder_get_request(p_rb, &p_request);

			zob_message_clone(p_request, &p_answer);

			zob_server_process(p_server, p_request, p_answer);
			zob_server_answer(p_server, p_answer);
			zob_message_dispose(&p_request);
		}

		zob_message_dispose(&p_answer);
		zob_request_builder_dispose(&p_rb);
		zob_server_session_end(p_server);
	}
	while(p_server->is_running);

	zob_store_dispose(&p_server->p_store);

	return EXIT_SUCCESS;
}

void zob_server_process(struct zob_server * p_server, struct zob_message * p_request, struct zob_message * p_answer)
{
	const char * p_verb = NULL;
	char * p_key = NULL, * p_payload = NULL;
	zob_message_get_verb(p_request, (char **) &p_verb);
	zob_message_get_key(p_request, (char **) &p_key);

	if(0 == strcmp("read", p_verb))
	{
		bool has_found = false;
		zob_store_read(p_server->p_store, p_key, &p_payload, &has_found);
		if(has_found)
		{
			char answer[100];
			snprintf(answer, sizeof answer / sizeof answer[0] - 1 , "Ok %s", p_payload);
			free(p_payload);
			zob_message_set_payload(p_answer, answer);
		}
		else
			zob_message_set_payload(p_answer, "Ko\nNot Found");

	}
	else if(0 == strcmp("new", p_verb) || 0 == strcmp("update", p_verb))
	{
		bool is_ok = false;
		bool is_overwrite = 0 == strcmp("update", p_verb);

		zob_message_get_payload(p_request, &p_payload);
		zob_store_write(p_server->p_store, p_key, p_payload, is_overwrite, &is_ok);
		free(p_payload);

		zob_app_log(gp_app, p_key, __FILE__, __LINE__);

		if(is_ok)
			zob_message_set_payload(p_answer, "Ok");
		else
			zob_message_set_payload(p_answer, "Ko");
	}
	else if(0 == strcmp("delete", p_verb))
	{
		bool is_ok = false;

		zob_message_get_payload(p_request, &p_payload);
		zob_store_delete(p_server->p_store, p_key, &is_ok);

		zob_app_log(gp_app, p_key, __FILE__, __LINE__);

		if(is_ok)
			zob_message_set_payload(p_answer, "Ok");
		else
			zob_message_set_payload(p_answer, "Ko");
	}
	else if(0 == strcmp("stop", p_verb))
	{
		zob_message_set_payload(p_answer, "Ok");
		p_server->is_running = false;
	}

	free(p_key);
}

void zob_server_listen(struct zob_server * p_server, const char * socket_path)
{
	p_server->socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
	CHECK_INT(p_server->socket_fd);

	strcpy(p_server->self_addr.sun_path, socket_path);

	CHECK_INT(bind(p_server->socket_fd
				, (struct sockaddr *) &p_server->self_addr
				, sizeof p_server->self_addr));

	CHECK_INT(listen(p_server->socket_fd, 0));
}

void zob_server_session_begin(struct zob_server * p_server)
{
	CHECK_INT(p_server->session_fd = accept(p_server->socket_fd
				, (struct sockaddr *) &p_server->remote_addr
				, &p_server->remote_addr_size));
}

void zob_server_session_end(struct zob_server * p_server)
{
	close(p_server->session_fd);
}

#define min(l, r) (((l) > (r)) ? (r) : (l))

void zob_server_read(struct zob_server * p_server, char **pp_payload)
{
	char * p_payload = NULL;
	int payload_size = 0;

	p_payload = calloc(DB_BUFFER_SIZE + 1, sizeof *p_payload);

	int reading_count = 0;
	do
	{
		reading_count = read(p_server->session_fd, &p_server->buffer, DB_BUFFER_SIZE);
		CHECK_INT(reading_count);

		if(0 == reading_count)
			break;

		payload_size += min(DB_BUFFER_SIZE, reading_count);
		p_payload = realloc(p_payload, (payload_size + 1) * sizeof *p_payload);
		CHECK_NULL(p_payload);
		strncat(p_payload, p_server->buffer, min(DB_BUFFER_SIZE, reading_count));

	} while(reading_count > DB_BUFFER_SIZE);

	*pp_payload = p_payload;
}

void zob_server_answer(struct zob_server * p_server, struct zob_message * p_answer)
{
	char * p_verb = NULL, * p_payload = NULL;
	zob_message_get_verb(p_answer, &p_verb);
	zob_message_get_payload(p_answer, &p_payload);

	if(!p_payload ||!p_verb)
		zob_app_error(gp_app, "Malformed answer", __FILE__, __LINE__);

	char buffer[1024];
	int write_count = snprintf(buffer, sizeof buffer / sizeof buffer[0] - 1, p_payload);
	CHECK_INT(write_count);

	int written = write(p_server->session_fd, buffer, min(write_count, 1024 - 1));
	CHECK_INT(written);

	free(p_payload);
}
