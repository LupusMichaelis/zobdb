
#include "app.h"
#include "message.h"
#include "object.h"
#include "string.h"


#include <stdlib.h>

struct zob_message
{
	// head
	struct zob_string * p_verb;
	struct zob_string * p_key;

	// body
	struct zob_string * p_payload;
};

APP_ALLOC(message)
APP_CREATE(message)
APP_CLONE(message)
APP_DISPOSE(message)

void zob_message_init(struct zob_message * p_message)
{
	memset(p_message, 0, sizeof *p_message);
	zob_string_create(&p_message->p_verb);
	zob_string_create(&p_message->p_key);
	zob_string_create(&p_message->p_payload);
}

void zob_message_copy(struct zob_message * p_message_orig, struct zob_message * p_message_dest)
{
	zob_string_copy(p_message_orig->p_key, p_message_dest->p_key);
	zob_string_copy(p_message_orig->p_verb, p_message_dest->p_verb);
	zob_string_copy(p_message_orig->p_payload, p_message_dest->p_payload);
}

void zob_message_clean(struct zob_message * p_message, bool has_to_dispose)
{
	if(has_to_dispose)
	{
		if(p_message->p_verb)
			zob_string_dispose(&p_message->p_verb);

		if(p_message->p_key)
			zob_string_dispose(&p_message->p_key);

		if(p_message->p_payload)
			zob_string_dispose(&p_message->p_payload);
	}
}

void zob_message_verb_set(struct zob_message * p_message, struct zob_string * p_verb)
{
	zob_string_copy(p_verb, p_message->p_verb);
}

void zob_message_verb_get(struct zob_message * p_message, struct zob_string * p_verb)
{
	zob_string_copy(p_message->p_verb, p_verb);
}

void zob_message_key_set(struct zob_message * p_message, struct zob_string * p_key)
{
	zob_string_copy(p_key, p_message->p_key);
}

void zob_message_key_get(struct zob_message * p_message, struct zob_string * p_key)
{
	zob_string_copy(p_message->p_key, p_key);
}

void zob_message_payload_set(struct zob_message * p_message, struct zob_string * p_payload)
{
	zob_string_copy(p_payload, p_message->p_payload);
}

void zob_message_payload_get(struct zob_message * p_message, struct zob_string * p_payload)
{
	zob_string_copy(p_message->p_payload, p_payload);
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

