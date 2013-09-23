#include "message.h"
#include "request-builder.h"
#include "buffer.h"
#include "app.h"

#include <stdlib.h>

struct db_request_builder
{
	struct db_app * p_app;
	struct db_message * p_request;

	int verb;
	int options;

	bool has_verb;
	bool is_bad_request;
	bool need_moar;

	struct db_buffer * p_buffer;

	// boundaries of the current sequence to analyse in buffer
	size_t first;
	size_t last;

	// current position of the analyser in the buffer
	size_t current;
};

struct db_buffer;

// Internal code to avoid the builder to check all the verb string on every input
static char * gpc_verbs[] = {
	VERB_NEW,
	VERB_CLONE,
	VERB_READ,
	VERB_DELETE,
	VERB_UPDATE,
	(char *)NULL,
};

void db_request_builder_create(struct db_request_builder ** pp_rb, struct db_app * p_app)
{
	db_request_builder_new(pp_rb, p_app);
	db_request_builder_init(*pp_rb, p_app);
}

void db_request_builder_new(struct db_request_builder ** pp_rb, struct db_app * p_app)
{
	struct db_request_builder * p_rb = NULL;
	p_rb = calloc(1, sizeof *p_rb);
	CHECK_NULL(p_app, p_rb);

	p_rb->p_app = p_app;
	*pp_rb = p_rb;
}

void db_request_builder_init(struct db_request_builder * p_rb, struct db_app * p_app)
{
	p_rb->p_app = p_app;
	db_message_create(&p_rb->p_request, p_app);

	db_buffer_create(&p_rb->p_buffer, p_app);

	p_rb->is_bad_request = false;
	p_rb->need_moar = true;

	p_rb->first = 0;
	p_rb->last = 0;
	p_rb->current = 0;

}

void db_request_builder_dispose(struct db_request_builder * p_rb)
{
	db_buffer_dispose(&p_rb->p_buffer);
}

void db_request_builder_parse(
		struct db_request_builder * p_rb,
		const char * p_text,
		bool * p_need_moar
		)
{
	db_buffer_ensure(p_rb->p_buffer, p_rb->last, strlen(p_text));
	db_buffer_write(p_rb->p_buffer, &p_rb->last, p_text);

	do
	{
		if(!p_rb->has_verb)
			db_request_builder_find_verb(p_rb);
		else
			switch(p_rb->verb)
			{
				case 0 /* VERB_NEW */:
					db_request_builder_parse_new(p_rb);
					break;
				case 1 /* VERB_CLONE */:
					db_request_builder_parse_clone(p_rb);
					break;
				case 2 /* VERB_READ */:
					db_request_builder_parse_read(p_rb);
					break;
				case 3 /* VERB_DELETE */:
					db_request_builder_parse_delete(p_rb);
					break;
				case 4 /* VERB_UPDATE */:
					db_request_builder_parse_update(p_rb);
					break;
				default:
					db_app_error(p_rb->p_app, "Invalid verb identifier", __FILE__, __LINE__);
			}

		if(p_rb->is_bad_request)
			break;

		if(!p_rb->need_moar)
			break;

	} while(p_rb->last - p_rb->current > 0);

	*p_need_moar = p_rb->need_moar;
}

void db_request_builder_is_bad_request(struct db_request_builder * p_rb, bool * is_bad_request)
{
	*is_bad_request = p_rb->is_bad_request;
}

void db_request_builder_get_request(struct db_request_builder * p_rb, struct db_message ** pp_request)
{
	db_message_clone(p_rb->p_request, pp_request);
}

void db_request_builder_find_verb(struct db_request_builder * p_rb)
{
	bool has_found = false;
	size_t cursor = 0;
	db_buffer_find_char(p_rb->p_buffer, ' ', p_rb->first, p_rb->last, &cursor, &has_found);

	if(!has_found)
		return; // We don't have enough data to take a decision

	// We place the cursor after the space
	p_rb->current = cursor + 1;

	//
	char * p_first = NULL;
	db_buffer_get(p_rb->p_buffer, &p_first);
	p_first += p_rb->first;

	// Look up the table of verbs to find what the client want
	char ** pp_verbs = &gpc_verbs[0];
	do
		if(0 == strncmp(*pp_verbs, p_first, strlen(*pp_verbs)))
			break;
	while(*++pp_verbs);

	if(*pp_verbs)
	{
		p_rb->verb = pp_verbs - gpc_verbs;
		p_rb->has_verb = true;
		p_rb->first = cursor + 1;
	}
	else
	{
		p_rb->is_bad_request = true;
		p_rb->need_moar = false;
	}
}

void db_request_builder_parse_new(struct db_request_builder * p_rb)
{
	fprintf(stderr, "Parse new\n");
	p_rb->is_bad_request = true;
}

void db_request_builder_parse_clone(struct db_request_builder * p_rb)
{
	fprintf(stderr, "Parse clone\n");
	p_rb->is_bad_request = true;
}

void db_request_builder_parse_read(struct db_request_builder * p_rb)
{
	bool has_found = false;
	size_t line_feed = 0;

	db_buffer_find_char(p_rb->p_buffer, '\n', p_rb->first, p_rb->last, &line_feed, &has_found);
	if(!has_found)
		return; // We don't have enough data to take a decision

	size_t word_separator = 0;
	db_buffer_find_char(p_rb->p_buffer, ' ', p_rb->first, p_rb->last, &word_separator, &has_found);

	char * p_key = NULL;
	//char * p_option = NULL;

	db_buffer_get_string(p_rb->p_buffer, p_rb->first, has_found ? word_separator : line_feed, &p_key);

	db_message_set_verb(p_rb->p_request, gpc_verbs[p_rb->verb]);
	db_message_set_key(p_rb->p_request, p_key);
	free(p_key);

	/* TODO options!
	if(has_found)
	{
		// Look up the table of options to find how to read the value
		char ** pp_options = &gpc_read_options[0];
		do
			if(0 == strncmp(*pp_options, p_first, strlen(*pp_options)))
				break;
		while(*++pp_options);

		if(*pp_options)
		{
			p_rb->options |= 2 << pp_options - gpc_options;
			p_rb->has_verb = true;
		}
		else
		{
			p_rb->is_bad_request = true;
			p_rb->need_moar = false;
		}
	}
	*/

	p_rb->need_moar = false;
}

void db_request_builder_parse_delete(struct db_request_builder * p_rb)
{
	fprintf(stderr, "Parse delete\n");
	p_rb->is_bad_request = true;
}

void db_request_builder_parse_update(struct db_request_builder * p_rb)
{
	fprintf(stderr, "Parse update\n");
	p_rb->is_bad_request = true;
}

