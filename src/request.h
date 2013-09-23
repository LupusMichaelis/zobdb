#ifndef H_DB_REQUEST
#	define H_DB_REQUEST

struct db_app;
struct db_server;

#define VERB_NEW			"new"
#define VERB_CLONE			"clone"
#define VERB_READ			"read"
#define VERB_DELETE			"delete"
#define VERB_UPDATE			"update"

#define OPT_READ_DEEP		"deep"
#define OPT_READ_SHALLOW	"shallow"

struct db_request;

void db_request_create(struct db_request ** pp_r, struct db_app * p_app);
void db_request_clone(struct db_request * p_r, struct db_request ** pp_r);

void db_request_new(struct db_request ** pp_r, struct db_app * p_app);

void db_request_init(struct db_request * p_r, struct db_app * p_app);
void db_request_copy(struct db_request * p_r_orig, struct db_request * p_r_dest);

void db_request_set_verb(struct db_request * p_r, char * p_verb);
void db_request_get_verb(struct db_request * p_r, char ** pp_verb);

void db_request_set_key(struct db_request * p_r, char * p_key);
void db_request_get_key(struct db_request * p_r, char ** pp_key);

void db_request_add_arg(struct db_request ** pp_r, char * p_name, char * p_value);

#endif // H_DB_REQUEST
