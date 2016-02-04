#ifndef H_ZOB_CONFIG // H_ZOB_CONFIG
#	define H_ZOB_CONFIG // H_ZOB_CONFIG

struct zob_config;
struct zob_app;

#	include "pair.h"

#	include <stddef.h>
#	include <stdbool.h>

void zob_config_vector_alloc(struct zob_config *** ppp, size_t count);
void zob_config_vector_create(struct zob_config *** ppp, size_t count);
void zob_config_vector_dispose(struct zob_config *** pp);
void zob_config_vector_clone(struct zob_config ** pp, struct zob_config *** ppp);
void zob_config_vector_copy(struct zob_config ** pp_from, struct zob_config ** pp_to);
void zob_config_vector_clean(struct zob_config ** pp, bool has_to_dispose);

void zob_config_vector_set(struct zob_config ** pp, size_t position, struct pair * p_pair);
void zob_config_vector_find(struct zob_config ** pp, size_t position, void * p_value);
void zob_config_vector_get(struct zob_config ** pp, size_t position, void ** pp_value);
void zob_config_vector_get_by_name(struct zob_config ** pp, char * p_name, void ** pp_value);

void zob_config_create(struct zob_config ** pp_config);
void zob_config_init(struct zob_config * p_config);

void zob_config_set(struct zob_config * p_config, struct pair * p_key_value);
void zob_config_update(struct zob_config * p_config, struct pair key_value);

void zob_config_get(struct zob_config * p_config, struct pair * p_pairs);
void zob_config_get_data(struct zob_config * p_config, size_t first, size_t last, void ** p_datas);

void zob_config_clone(struct zob_config * p_config, struct zob_config ** pp_config);
void zob_config_copy(struct zob_config * p_from, struct zob_config * p_to);

void zob_config_clean(struct zob_config * p_config, bool has_to_dispose);
void zob_config_dispose(struct zob_config ** pp_config);

#endif // H_ZOB_CONFIG
