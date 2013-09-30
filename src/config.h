#ifndef H_DB_CONFIG // H_DB_CONFIG
#	define H_DB_CONFIG // H_DB_CONFIG

struct db_config;
struct db_app;

#	include "pair.h"

#	include <stddef.h>
#	include <stdbool.h>

void db_config_vector_alloc(struct db_config *** ppp, struct db_app * p_app, size_t count);
void db_config_vector_create(struct db_config *** ppp, struct db_app * p_app, size_t count);
void db_config_vector_dispose(struct db_config *** pp);
void db_config_vector_clone(struct db_config ** pp, struct db_config *** ppp);

void db_config_vector_set(struct db_config ** pp, size_t position, struct pair * p_pair);
void db_config_vector_find(struct db_config ** pp, size_t position, char * p_value);
void db_config_vector_get(struct db_config ** pp, size_t position, char ** pp_value);
void db_config_vector_get_by_name(struct db_config ** pp, char * p_name, char ** pp_value);
void db_config_vector_get_by_value(struct db_config ** pp, char * p_value, char ** pp_value);

void db_config_create(struct db_config ** pp_config, struct db_app * p_app);
void db_config_init(struct db_config * p_config, struct db_app * p_app);

void db_config_set(struct db_config * p_config, struct pair * p_key_value);
void db_config_update(struct db_config * p_config, struct pair key_value);

void db_config_get(struct db_config * p_config, char ** pp_text);
void db_config_get_data(struct db_config * p_config, size_t first, size_t last, char ** p_datas);

void db_config_clone(struct db_config * p_config, struct db_config ** pp_config);
void db_config_copy(struct db_config * p_from, struct db_config * p_to);

void db_config_clean(struct db_config * p_config, bool has_to_dispose);
void db_config_dispose(struct db_config ** pp_config);

#endif // H_DB_CONFIG
