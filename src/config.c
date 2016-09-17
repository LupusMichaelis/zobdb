#include "config.h"
#include "buffer.h"
#include "string.h"
#include "app.h"
#include "object.h"
#include "object-vector.h"

#include <stdlib.h>

struct zob_config
{
	struct pair key_value;
};

APP_VECTOR_ALLOC(config)
APP_VECTOR_CREATE(config)
APP_VECTOR_CLONE(config)
APP_VECTOR_COPY(config)
APP_VECTOR_DISPOSE(config)

void zob_config_vector_set(struct zob_config ** pp, size_t position, struct pair * p_pair)
{
	memcpy(&pp[position]->key_value, p_pair, sizeof *p_pair);
}

void zob_config_vector_get(struct zob_config ** pp, size_t position, struct zob_string ** pp_value)
{
	// XXX We pray for no overflow
	*pp_value = (*(pp + position))->key_value.p_value;
}

void zob_config_vector_get_by_name(struct zob_config ** pp
		, struct zob_string * p_name, bool * p_found, struct zob_string ** pp_value)
{
	while(*pp)
	{
		int diff = 0;
		zob_string_compare(p_name, (*pp)->key_value.p_name, &diff);
		if(0 == diff)
			break;

		++pp;
	}

	*pp_value = NULL == *pp ? NULL : (*pp)->key_value.p_value;

	if(p_found)
		*p_found = (bool) *pp;
}

APP_ALLOC(config)
APP_CREATE(config)
APP_CLONE(config)
APP_DISPOSE(config)

void zob_config_init(struct zob_config * p_config)
{
}

void zob_config_clean(struct zob_config * p_config, bool has_to_dispose)
{
	if(has_to_dispose)
	{
		zob_string_dispose(&p_config->key_value.p_name);
		zob_string_dispose(&p_config->key_value.p_value);
	}

	memset(p_config, 0, sizeof *p_config);
}

void zob_config_copy(struct zob_config * p_from, struct zob_config * p_to)
{
	memcpy(&p_to->key_value, &p_from->key_value, sizeof p_to->key_value);
}
