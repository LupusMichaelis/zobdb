
#include "app.h"
#include "message.h"

#include <stdlib.h>

struct zob_message
{
	// head
	char * p_verb;
	char * p_key;

	// body
	char * p_payload;
};

#include "object.h"

APP_ALLOC(message)
APP_CREATE(message)
APP_CLONE(message)
APP_DISPOSE(message)

void zob_message_init(struct zob_message * p_message)
{
	memset(p_message, 0, sizeof *p_message);
}

void zob_message_copy(struct zob_message * p_message_orig, struct zob_message * p_message_dest)
{
	p_message_dest->p_verb = p_message_orig->p_verb;

	if(p_message_orig->p_key)
	{
		p_message_dest->p_key = calloc(1 + strlen(p_message_orig->p_key), sizeof *p_message_orig->p_key);
		strcpy(p_message_dest->p_key, p_message_orig->p_key);
	}

	if(p_message_orig->p_payload)
	{
		p_message_dest->p_payload = calloc(1 + strlen(p_message_orig->p_payload), sizeof *p_message_orig->p_payload);
		strcpy(p_message_dest->p_payload, p_message_orig->p_payload);
	}
}

void zob_message_clean(struct zob_message * p_message, bool has_to_dispose)
{
	if(has_to_dispose)
	{
		if(p_message->p_key)
		{
			free(p_message->p_key);
			p_message->p_key = NULL;
		}

		if(p_message->p_payload)
		{
			free(p_message->p_payload);
			p_message->p_payload = NULL;
		}
	}

	p_message->p_verb = NULL;
}

void zob_message_set_verb(struct zob_message * p_message, char * p_verb)
{
	p_message->p_verb = p_verb;
}

void zob_message_get_verb(struct zob_message * p_message, char ** pp_verb)
{
	*pp_verb = p_message->p_verb;
}

void zob_message_set_key(struct zob_message * p_message, char * p_key)
{
	p_message->p_key = realloc(p_message->p_key, 1 + strlen(p_key) * sizeof *p_key);
	CHECK_NULL(p_message->p_key);
	strcpy(p_message->p_key, p_key);
}

void zob_message_get_key(struct zob_message * p_message, char ** pp_key)
{
	char * p_key = calloc(1 + strlen(p_message->p_key), sizeof *p_key);
	CHECK_NULL(p_key);
	strcpy(p_key, p_message->p_key);

	*pp_key = p_key;
}

void zob_message_get_payload(struct zob_message * p_message, char ** pp_payload)
{
	if(NULL == p_message->p_payload)
	{
		*pp_payload = NULL;
		return;
	}

	char * p_payload = calloc(1 + strlen(p_message->p_payload), sizeof *p_payload);
	CHECK_NULL(p_payload);
	strcpy(p_payload, p_message->p_payload);

	*pp_payload = p_payload;
}

void zob_message_set_payload(struct zob_message * p_message, char * p_payload)
{
	if(p_message->p_payload)
	{
		free(p_message->p_payload);
		p_message->p_payload = NULL;
	}

	p_message->p_payload = calloc(1 + strlen(p_payload), sizeof *p_message->p_payload);
	CHECK_NULL(p_message->p_payload);
	strcpy(p_message->p_payload, p_payload);
}

/*
void zob_message_append_payload(struct zob_message * p_message, char * p_payload)
{
	CHECK_NULL(p_payload);
	CHECK_NULL(p_message);

	size_t new_size = (1 + strlen(p_message->p_payload) + strlen(p_payload)) * sizeof *p_payload;
	p_message->p_payload = realloc(p_message->p_payload, new_size);
	CHECK_NULL(p_message->p_payload);
	strcat(p_message->p_payload, p_payload);
}
*/

