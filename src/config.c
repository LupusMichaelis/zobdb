#include "config.h"
#include "buffer.h"
#include "app.h"
#include "object.h"
#include "object-vector.h"

#include <stdlib.h>
#include <stdlib.h>

struct db_app;

struct db_config
{
	struct db_app * p_app;

	struct pair key_value;
};


APP_VECTOR_ALLOC(config)
APP_VECTOR_CREATE(config)
APP_VECTOR_CLONE(config)
APP_VECTOR_DISPOSE(config)

void db_config_vector_set(struct db_config ** pp, size_t position, struct pair * p_pair)
{
	memcpy(&pp[position]->key_value, p_pair, sizeof *p_pair);
}

void db_config_vector_get(struct db_config ** pp, size_t position, char ** pp_value)
{
	// XXX We pray for no overflow
	*pp_value = (*(pp + position))->key_value.p_value;
}

void db_config_vector_get_by_value(struct db_config ** pp, char * p_value, char ** pp_value)
{
	while(*pp)
		if(0 == strcmp(p_value, (*pp)->key_value.p_value))
			break;
		else
			++pp;

	*pp_value = NULL == *pp ? NULL : (*pp)->key_value.p_value;
}

void db_config_vector_get_by_name(struct db_config ** pp, char * p_name, char ** pp_value)
{
	while(*pp)
		if(0 == strcmp(p_name, (*pp)->key_value.p_name))
			break;
		else
			++pp;

	*pp_value = NULL == *pp ? NULL : (*pp)->key_value.p_value;
}

APP_ALLOC(config)
APP_CREATE(config)
APP_CLONE(config)
APP_DISPOSE(config)

void db_config_init(struct db_config * p_config, struct db_app * p_app)
{
	p_config->p_app = p_app;
}

void db_config_clean(struct db_config * p_config, bool has_to_dispose)
{
	memset(p_config, 0, sizeof *p_config);
}

void db_config_copy(struct db_config * p_from, struct db_config * p_to)
{
	p_to->p_app = p_from->p_app;
	memcpy(&p_to->key_value, &p_from->key_value, sizeof p_to->key_value);
}
