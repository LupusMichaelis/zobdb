#ifndef H_ZOB_MESSAGE
#	define H_ZOB_MESSAGE

struct zob_app;
struct zob_server;

#	define ZOB_VERB_NEW			"new"
#	define ZOB_VERB_CLONE		"clone"
#	define ZOB_VERB_READ		"read"
#	define ZOB_VERB_DELETE		"delete"
#	define ZOB_VERB_UPDATE		"update"
#	define ZOB_VERB_STOP		"stop"

#	define OPT_READ_DEEP		"deep"
#	define OPT_READ_SHALLOW		"shallow"

#	include <stdbool.h>

struct zob_message;
struct zob_string;

void zob_message_create(struct zob_message ** pp_message);
void zob_message_clone(struct zob_message * p_message, struct zob_message ** pp_message);
void zob_message_dispose(struct zob_message ** pp_message);

void zob_message_alloc(struct zob_message ** pp_message);

void zob_message_init(struct zob_message * p_message);
void zob_message_copy(struct zob_message * p_message_orig, struct zob_message * p_message_dest);
void zob_message_clean(struct zob_message * p_message, bool has_to_dispose);

void zob_message_verb_set(struct zob_message * p_message, struct zob_string * p_verb);
void zob_message_verb_get(struct zob_message * p_message, struct zob_string * p_verb);

void zob_message_key_set(struct zob_message * p_message, struct zob_string * p_key);
void zob_message_key_get(struct zob_message * p_message, struct zob_string * p_key);

void zob_message_payload_set(struct zob_message * p_message, struct zob_string * p_payload);
void zob_message_payload_get(struct zob_message * p_message, struct zob_string * p_payload);
void zob_message_payload_append(struct zob_message * p_message, struct zob_string * p_payload);

void zob_message_add_arg(struct zob_message ** pp_message, char * p_name, char * p_value);

#endif // H_ZOB_MESSAGE
