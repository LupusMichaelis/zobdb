#ifndef H_DB_CONFIG // H_DB_CONFIG
#	define H_DB_CONFIG // H_DB_CONFIG

struct db_config;
struct db_app;

#	include "pair.h"

#	include <stddef.h>
#	include <stdbool.h>

void db_config_vector_alloc(struct db_config *** ppp, size_t count);
void db_config_vector_create(struct db_config *** ppp, size_t count);
void db_config_vector_dispose(struct db_config *** pp);
void db_config_vector_clone(struct db_config ** pp, struct db_config *** ppp);
void db_config_vector_copy(struct db_config ** pp_from, struct db_config ** pp_to);
void db_config_vector_clean(struct db_config ** pp, bool has_to_dispose);

void db_config_vector_set(struct db_config ** pp, size_t position, struct pair * p_pair);
void db_config_vector_find(struct db_config ** pp, size_t position, void * p_value);
void db_config_vector_get(struct db_config ** pp, size_t position, void ** pp_value);
void db_config_vector_get_by_name(struct db_config ** pp, char * p_name, void ** pp_value);

void db_config_create(struct db_config ** pp_config);
void db_config_init(struct db_config * p_config);

void db_config_set(struct db_config * p_config, struct pair * p_key_value);
void db_config_update(struct db_config * p_config, struct pair key_value);

void db_config_get(struct db_config * p_config, struct pair * p_pairs);
void db_config_get_data(struct db_config * p_config, size_t first, size_t last, void ** p_datas);

void db_config_clone(struct db_config * p_config, struct db_config ** pp_config);
void db_config_copy(struct db_config * p_from, struct db_config * p_to);

void db_config_clean(struct db_config * p_config, bool has_to_dispose);
void db_config_dispose(struct db_config ** pp_config);

#endif // H_DB_CONFIG
