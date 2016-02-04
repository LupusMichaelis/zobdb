#ifndef H_DB_MESSAGE
#	define H_DB_MESSAGE

struct db_app;
struct db_server;

#	define VERB_NEW				"new"
#	define VERB_CLONE			"clone"
#	define VERB_READ			"read"
#	define VERB_DELETE			"delete"
#	define VERB_UPDATE			"update"

#	define OPT_READ_DEEP		"deep"
#	define OPT_READ_SHALLOW		"shallow"

#	include <stdbool.h>

struct db_message;

void db_message_create(struct db_message ** pp_message);
void db_message_clone(struct db_message * p_message, struct db_message ** pp_message);
void db_message_dispose(struct db_message ** pp_message);

void db_message_alloc(struct db_message ** pp_message);

void db_message_init(struct db_message * p_message);
void db_message_copy(struct db_message * p_message_orig, struct db_message * p_message_dest);
void db_message_clean(struct db_message * p_message, bool has_to_dispose);

void db_message_set_verb(struct db_message * p_message, char * p_verb);
void db_message_get_verb(struct db_message * p_message, char ** pp_verb);

void db_message_set_key(struct db_message * p_message, char * p_key);
void db_message_get_key(struct db_message * p_message, char ** pp_key);

void db_message_set_payload(struct db_message * p_message, char * p_payload);
void db_message_get_payload(struct db_message * p_message, char ** pp_payload);
void db_message_append_payload(struct db_message * p_message, char * p_payload);

void db_message_add_arg(struct db_message ** pp_message, char * p_name, char * p_value);

#endif // H_DB_MESSAGE
