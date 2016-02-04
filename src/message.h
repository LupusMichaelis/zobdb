#ifndef H_ZOB_MESSAGE
#	define H_ZOB_MESSAGE

struct zob_app;
struct zob_server;

#	define VERB_NEW				"new"
#	define VERB_CLONE			"clone"
#	define VERB_READ			"read"
#	define VERB_DELETE			"delete"
#	define VERB_UPDATE			"update"

#	define OPT_READ_DEEP		"deep"
#	define OPT_READ_SHALLOW		"shallow"

#	include <stdbool.h>

struct zob_message;

void zob_message_create(struct zob_message ** pp_message);
void zob_message_clone(struct zob_message * p_message, struct zob_message ** pp_message);
void zob_message_dispose(struct zob_message ** pp_message);

void zob_message_alloc(struct zob_message ** pp_message);

void zob_message_init(struct zob_message * p_message);
void zob_message_copy(struct zob_message * p_message_orig, struct zob_message * p_message_dest);
void zob_message_clean(struct zob_message * p_message, bool has_to_dispose);

void zob_message_set_verb(struct zob_message * p_message, char * p_verb);
void zob_message_get_verb(struct zob_message * p_message, char ** pp_verb);

void zob_message_set_key(struct zob_message * p_message, char * p_key);
void zob_message_get_key(struct zob_message * p_message, char ** pp_key);

void zob_message_set_payload(struct zob_message * p_message, char * p_payload);
void zob_message_get_payload(struct zob_message * p_message, char ** pp_payload);
void zob_message_append_payload(struct zob_message * p_message, char * p_payload);

void zob_message_add_arg(struct zob_message ** pp_message, char * p_name, char * p_value);

#endif // H_ZOB_MESSAGE
