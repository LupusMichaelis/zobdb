#include "app.h"
#include "log.h"
#include "message.h"
#include "object.h"
#include "request-builder.h"
#include "string.h"

#include <stdlib.h>
#include <assert.h>

struct zob_request_builder
{
	struct zob_message * p_request;

	int verb;
	int options;

	bool has_verb;
	bool is_bad_request;
	bool is_header_parsed;
	bool need_moar;

	struct zob_string * p_buffer;
	struct zob_string * p_payload;

	// boundaries of the current sequence to analyse in buffer
	size_t first;
	size_t last;

	// current position of the analyser in the buffer
	size_t current;
};

struct zob_string;

// Internal code to avoid the builder to check all the verb string on every input
static char * gpc_verbs[] = {
	VERB_NEW,
	VERB_CLONE,
	VERB_READ,
	VERB_DELETE,
	VERB_UPDATE,
	VERB_STOP,
	(char *)NULL,
};

APP_ALLOC(request_builder)
APP_CREATE(request_builder)
APP_CLONE(request_builder)
APP_DISPOSE(request_builder)

void zob_request_builder_init(struct zob_request_builder * p_rb)
{
	zob_message_create(&p_rb->p_request);
	zob_string_create(&p_rb->p_buffer);
	zob_string_create(&p_rb->p_payload);

	p_rb->is_bad_request = false;
	p_rb->is_header_parsed = false;
	p_rb->need_moar = true;

	p_rb->first = 0;
	p_rb->last = 0;
	p_rb->current = 0;

}

void zob_request_builder_clean(struct zob_request_builder * p_rb, bool has_to_dispose)
{
	if(has_to_dispose)
	{
		if(p_rb->p_buffer)
			zob_string_dispose(&p_rb->p_buffer);

		if(p_rb->p_payload)
			zob_string_dispose(&p_rb->p_payload);

		if(p_rb->p_request)
			zob_message_dispose(&p_rb->p_request);
	}

	memset(p_rb, 0, sizeof *p_rb);
}

void zob_request_builder_copy(struct zob_request_builder * p_orig, struct zob_request_builder * p_dest)
{
	zob_message_clone(p_orig->p_request, &p_dest->p_request);

	p_dest->verb = p_orig->verb;
	p_dest->options = p_orig->options;

	p_dest->has_verb = p_orig->has_verb;
	p_dest->is_bad_request = p_orig->is_bad_request;
	p_dest->is_header_parsed = p_orig->is_header_parsed;
	p_dest->need_moar = p_orig->need_moar;

	zob_string_clone(p_orig->p_buffer, &p_dest->p_buffer);
	zob_string_clone(p_orig->p_payload, &p_dest->p_payload);

	// boundaries of the current sequence to analyse in buffer
	p_dest->first = p_orig->first;
	p_dest->last = p_orig->last;

	// current position of the analyser in the buffer
	p_dest->current = p_orig->current;
}

void zob_request_builder_parse(
		struct zob_request_builder * p_rb,
		const char * p_text,
		bool * p_need_moar
		)
{
	zob_string_write(p_rb->p_buffer, 0, p_text, &p_rb->last);

	do
	{
		if(!p_rb->has_verb)
			zob_request_builder_find_verb(p_rb);

		if(p_rb->has_verb)
		{
			switch(p_rb->verb)
			{
				case 0 /* VERB_NEW */:
					zob_request_builder_parse_new(p_rb);
					break;
				case 1 /* VERB_CLONE */:
					zob_request_builder_parse_clone(p_rb);
					break;
				case 2 /* VERB_READ */:
					zob_request_builder_parse_read(p_rb);
					break;
				case 3 /* VERB_DELETE */:
					zob_request_builder_parse_delete(p_rb);
					break;
				case 4 /* VERB_UPDATE */:
					zob_request_builder_parse_update(p_rb);
					break;
				case 5 /* VERB_STOP */:
					zob_request_builder_parse_stop(p_rb);
					break;
				default:
					zob_app_error(gp_app, "Invalid verb identifier", __FILE__, __LINE__);
			}
		}
		if(p_rb->is_bad_request)
			break;

		if(!p_rb->need_moar)
			break;

	} while(p_rb->last - p_rb->current > 0);

	*p_need_moar = p_rb->need_moar;
}

void zob_request_builder_is_bad_request(struct zob_request_builder * p_rb, bool * is_bad_request)
{
	*is_bad_request = p_rb->is_bad_request;
}

void zob_request_builder_get_request(struct zob_request_builder * p_rb, struct zob_message ** pp_request)
{
	zob_message_clone(p_rb->p_request, pp_request);
}

void zob_request_builder_find_verb(struct zob_request_builder * p_rb)
{
	bool has_found = false;
	size_t cursor = 0;
	zob_string_find_char(p_rb->p_buffer, ' ', p_rb->first, p_rb->last, &cursor, &has_found);

	if(!has_found)
	{
		zob_string_find_char(p_rb->p_buffer, '\n', p_rb->first, p_rb->last, &cursor, &has_found);
		if(!has_found)
			return; // We don't have enough data to take a decision
	}

	// We place the cursor after the space
	p_rb->current = cursor + 1;

	//
	char * p_first = NULL;
	zob_string_get(p_rb->p_buffer, &p_first);
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
void zob_request_builder_parse_header(struct zob_request_builder * p_rb)
{
	if(p_rb->is_header_parsed)
		return;

	p_rb->current = 0;

	bool has_found = false;
	size_t line_feed = 0;

	zob_string_find_char(p_rb->p_buffer, '\n', p_rb->first, p_rb->last, &line_feed, &has_found);
	if(p_rb->verb != 5 && !has_found)
		return; // We don't have enough data to take a decision

	struct zob_string * p_verb = NULL;
	zob_string_create(&p_verb);
	zob_string_set(p_verb, gpc_verbs[p_rb->verb]);
	zob_message_verb_set(p_rb->p_request, p_verb);

	if(p_rb->verb == 5)
		return;

	size_t word_separator = 0;
	struct zob_string * p_key = NULL;
	zob_string_slice_get(p_rb->p_buffer, p_rb->first, line_feed, &p_key);
	zob_message_key_set(p_rb->p_request, p_key);
	zob_string_dispose(&p_key);

	p_rb->first = has_found ? word_separator : line_feed;
	++p_rb->first;
	p_rb->is_header_parsed = true;
}

void zob_request_builder_parse_new(struct zob_request_builder * p_rb)
{
	bool has_found = false;
	size_t line_feed = 0;

	zob_string_find_char(p_rb->p_buffer, '\n', p_rb->first, p_rb->last, &line_feed, &has_found);
	assert(has_found);

	size_t word_separator = 0;
	zob_string_find_char(p_rb->p_buffer, ' ', p_rb->first, p_rb->last, &word_separator, &has_found);

	if(!has_found)
	{
		p_rb->need_moar = false;
		p_rb->is_bad_request = true;
		return;
	}

	struct zob_string * p_verb = NULL;
	zob_string_create_from_cstring(&p_verb, gpc_verbs[p_rb->verb]);
	zob_message_verb_set(p_rb->p_request, p_verb);
	zob_string_dispose(&p_verb);

	struct zob_string * p_word = NULL;
	zob_string_slice_get(p_rb->p_buffer, p_rb->first, word_separator, &p_word);
	zob_message_key_set(p_rb->p_request, p_word);
	zob_string_dispose(&p_word);

	zob_string_slice_get(p_rb->p_buffer, word_separator + 1, line_feed, &p_word);
	zob_message_payload_set(p_rb->p_request, p_word);
	zob_string_dispose(&p_word);

	p_rb->need_moar = false;
}

void zob_request_builder_parse_clone(struct zob_request_builder * p_rb)
{
	struct zob_string * p_message = NULL;
	zob_string_create_from_cstring(&p_message, "Parse clone");
	zob_app_log(gp_app, __FILE__, __LINE__, ZOB_LOG_LEVEL_INFO, p_message);
	zob_string_dispose(&p_message);
	p_rb->is_bad_request = true;
}

void zob_request_builder_parse_read(struct zob_request_builder * p_rb)
{
	zob_request_builder_parse_header(p_rb);
	p_rb->need_moar = false;
}

void zob_request_builder_parse_delete(struct zob_request_builder * p_rb)
{
	bool has_found = false;
	size_t line_feed = 0;

	zob_string_find_char(p_rb->p_buffer, '\n', p_rb->first, p_rb->last, &line_feed, &has_found);
	assert(has_found);

	struct zob_string * p_word = NULL;
	zob_string_slice_get(p_rb->p_buffer, p_rb->first, line_feed, &p_word);
	zob_message_key_set(p_rb->p_request, p_word);
	zob_string_dispose(&p_word);

	struct zob_string * p_verb = NULL;
	zob_string_create_from_cstring(&p_verb, gpc_verbs[p_rb->verb]);
	zob_message_verb_set(p_rb->p_request, p_verb);
	zob_string_dispose(&p_verb);

	p_rb->is_bad_request = false;
	p_rb->need_moar = false;
}

void zob_request_builder_parse_update(struct zob_request_builder * p_rb)
{
	zob_request_builder_parse_new(p_rb);
}

void zob_request_builder_parse_stop(struct zob_request_builder * p_rb)
{
	zob_request_builder_parse_header(p_rb);
	p_rb->is_bad_request = false;
	p_rb->need_moar = false;
}
