
#include "app.h"
#include "request.h"

#include <stdlib.h>

struct db_request
{
	struct db_app * p_app;

	// head
	char * p_verb;
	char * p_key;

	// body
	char * p_payload;
};

void db_request_create(struct db_request ** pp_r, struct db_app * p_app)
{
	db_request_new(pp_r, p_app);
	db_request_init(*pp_r, p_app);
}

void db_request_new(struct db_request ** pp_r, struct db_app * p_app)
{
	struct db_request * p_r = NULL;
	p_r = calloc(1, sizeof *p_r);
	CHECK_NULL(p_app, p_r);

	p_r->p_app = p_app;
	*pp_r = p_r;
}

void db_request_clone(struct db_request * p_r, struct db_request ** pp_r)
{
	db_request_new(pp_r, p_r->p_app);
	db_request_copy(p_r, *pp_r);
}

void db_request_copy(struct db_request * p_r_orig, struct db_request * p_r_dest)
{
	p_r_dest->p_verb = p_r_orig->p_verb;
	p_r_dest->p_key = calloc(1 + strlen(p_r_orig->p_key), sizeof *p_r_orig->p_key);
	strcpy(p_r_dest->p_key, p_r_orig->p_key);
}

void db_request_init(struct db_request * p_r, struct db_app * p_app)
{
	p_r->p_app = p_app;
}

void db_request_set_verb(struct db_request * p_r, char * p_verb)
{
	p_r->p_verb = p_verb;
}

void db_request_get_verb(struct db_request * p_r, char ** pp_verb)
{
	*pp_verb = p_r->p_verb;
}

void db_request_set_key(struct db_request * p_r, char * p_key)
{
	p_r->p_key = realloc(p_r->p_key, 1 + strlen(p_key) * sizeof *p_key);
	CHECK_NULL(p_r->p_app, p_r->p_key);
	strcpy(p_r->p_key, p_key);
}

void db_request_get_key(struct db_request * p_r, char ** pp_key)
{
	char * p_key = calloc(1 + strlen(p_r->p_key), sizeof *p_key);
	CHECK_NULL(p_r->p_app, p_key);
	strcpy(p_key, p_r->p_key);

	*pp_key = p_key;
}
