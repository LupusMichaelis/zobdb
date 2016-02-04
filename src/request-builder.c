#include "message.h"
#include "request-builder.h"
#include "string.h"
#include "app.h"
#include "object.h"

#include <stdlib.h>
#include <assert.h>

struct db_request_builder
{
	struct db_message * p_request;

	int verb;
	int options;

	bool has_verb;
	bool is_bad_request;
	bool is_header_parsed;
	bool need_moar;

	struct db_string * p_buffer;
	struct db_string * p_payload;

	// boundaries of the current sequence to analyse in buffer
	size_t first;
	size_t last;

	// current position of the analyser in the buffer
	size_t current;
};

struct db_string;

// Internal code to avoid the builder to check all the verb string on every input
static char * gpc_verbs[] = {
	VERB_NEW,
	VERB_CLONE,
	VERB_READ,
	VERB_DELETE,
	VERB_UPDATE,
	"stop",
	(char *)NULL,
};

APP_ALLOC(request_builder)
APP_CREATE(request_builder)
APP_CLONE(request_builder)
APP_DISPOSE(request_builder)

void db_request_builder_init(struct db_request_builder * p_rb)
{
	db_message_create(&p_rb->p_request);
	db_string_create(&p_rb->p_buffer);
	db_string_create(&p_rb->p_payload);

	p_rb->is_bad_request = false;
	p_rb->is_header_parsed = false;
	p_rb->need_moar = true;

	p_rb->first = 0;
	p_rb->last = 0;
	p_rb->current = 0;

}

void db_request_builder_clean(struct db_request_builder * p_rb, bool has_to_dispose)
{
	if(has_to_dispose)
	{
		if(p_rb->p_buffer)
			db_string_dispose(&p_rb->p_buffer);

		if(p_rb->p_payload)
			db_string_dispose(&p_rb->p_payload);

		if(p_rb->p_request)
			db_message_dispose(&p_rb->p_request);
	}

	memset(p_rb, 0, sizeof *p_rb);
}

void db_request_builder_copy(struct db_request_builder * p_orig, struct db_request_builder * p_dest)
{
	db_message_clone(p_orig->p_request, &p_dest->p_request);

	p_dest->verb = p_orig->verb;
	p_dest->options = p_orig->options;

	p_dest->has_verb = p_orig->has_verb;
	p_dest->is_bad_request = p_orig->is_bad_request;
	p_dest->is_header_parsed = p_orig->is_header_parsed;
	p_dest->need_moar = p_orig->need_moar;

	db_string_clone(p_orig->p_buffer, &p_dest->p_buffer);
	db_string_clone(p_orig->p_payload, &p_dest->p_payload);

	// boundaries of the current sequence to analyse in buffer
	p_dest->first = p_orig->first;
	p_dest->last = p_orig->last;

	// current position of the analyser in the buffer
	p_dest->current = p_orig->current;
}

void db_request_builder_parse(
		struct db_request_builder * p_rb,
		const char * p_text,
		bool * p_need_moar
		)
{
	db_string_write(p_rb->p_buffer, &p_rb->last, p_text);

	do
	{
		if(!p_rb->has_verb)
			db_request_builder_find_verb(p_rb);
		else
		{
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
					db_app_error(gp_app, "Invalid verb identifier", __FILE__, __LINE__);
			}

			db_message_set_verb(p_rb->p_request, gpc_verbs[p_rb->verb]);
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
	db_string_find_char(p_rb->p_buffer, ' ', p_rb->first, p_rb->last, &cursor, &has_found);

	if(!has_found)
		return; // We don't have enough data to take a decision

	// We place the cursor after the space
	p_rb->current = cursor + 1;

	//
	char * p_first = NULL;
	db_string_get(p_rb->p_buffer, &p_first);
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

// Parse the header, and move set the start position to the body start
void db_request_builder_parse_header(struct db_request_builder * p_rb)
{
	if(p_rb->is_header_parsed)
		return;

	p_rb->current = 0;

	bool has_found = false;
	size_t line_feed = 0;

	db_string_find_char(p_rb->p_buffer, '\n', p_rb->first, p_rb->last, &line_feed, &has_found);
	if(!has_found)
		return; // We don't have enough data to take a decision

	db_message_set_verb(p_rb->p_request, gpc_verbs[p_rb->verb]);

	size_t word_separator = 0;
	char * p_key = NULL;
	db_string_get_data(p_rb->p_buffer, p_rb->first, line_feed, &p_key);
	db_message_set_key(p_rb->p_request, p_key);

	p_rb->first = has_found ? word_separator : line_feed;
	++p_rb->first;
	p_rb->is_header_parsed = true;
}

void db_request_builder_parse_new(struct db_request_builder * p_rb)
{
	bool has_found = false;
	size_t line_feed = 0;

	db_string_find_char(p_rb->p_buffer, '\n', p_rb->first, p_rb->last, &line_feed, &has_found);
	assert(has_found);

	size_t word_separator = 0;
	db_string_find_char(p_rb->p_buffer, ' ', p_rb->first, p_rb->last, &word_separator, &has_found);

	if(!has_found)
	{
		p_rb->need_moar = false;
		p_rb->is_bad_request = true;
		return;
	}

	db_message_set_verb(p_rb->p_request, gpc_verbs[p_rb->verb]);

	char * p_word = NULL;

	db_string_get_data(p_rb->p_buffer, p_rb->first, word_separator, &p_word);
	db_message_set_key(p_rb->p_request, p_word);
	free(p_word);

	db_string_get_data(p_rb->p_buffer, word_separator + 1, line_feed, &p_word);
	db_message_set_payload(p_rb->p_request, p_word);
	free(p_word);

	p_rb->need_moar = false;
}

void db_request_builder_parse_clone(struct db_request_builder * p_rb)
{
	fprintf(stderr, "Parse clone\n");
	p_rb->is_bad_request = true;
}

void db_request_builder_parse_read(struct db_request_builder * p_rb)
{
	db_request_builder_parse_header(p_rb);

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
	db_request_builder_parse_header(p_rb);

	// An update request requires a body, but the header consumed all bytes!
	if(p_rb->first > p_rb->last)
	{
		p_rb->need_moar = false;
		p_rb->is_bad_request = true;
		return;
	}

	////////////////////////////////////////////////////////////////////////////////
	bool has_found = false;
	size_t line_feed = 0;

	db_string_find_char(p_rb->p_buffer, '\n', p_rb->first, p_rb->last, &line_feed, &has_found);
	if(!has_found)
		return; // We don't have enough data to take a decision

	char * p_payload = NULL;
	db_string_get_data(p_rb->p_buffer, p_rb->first, line_feed, &p_payload);
	db_string_write(p_rb->p_payload, &p_rb->current, p_payload);
	free(p_payload);

	p_rb->need_moar = false;
}
