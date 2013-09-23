#ifndef H_DB_REQUEST_BUILDER
#	define H_DB_REQUEST_BUILDER

#	include "message.h"

#	include <stdbool.h>

struct db_app;
struct db_request_builder;

void db_request_builder_new(struct db_request_builder ** pp_rb, struct db_app * p_app);
void db_request_builder_create(struct db_request_builder ** pp_request_builder, struct db_app * p_app);
void db_request_builder_init(struct db_request_builder * p_request_builder, struct db_app * p_app);
void db_request_builder_dispose(struct db_request_builder * p_rb);

void db_request_builder_parse(
		struct db_request_builder * p_builder,
		const char * p_text,
		bool * need_moar
		);

void db_request_builder_get_request(struct db_request_builder * p_request, struct db_message ** pp_message);

void db_request_builder_is_bad_request(struct db_request_builder * p_rb, bool * is_bad_request);


void db_request_builder_find_verb(struct db_request_builder * p_rb);
void db_request_builder_parse_new(struct db_request_builder * p_rb);
void db_request_builder_parse_clone(struct db_request_builder * p_rb);
void db_request_builder_parse_read(struct db_request_builder * p_rb);
void db_request_builder_parse_delete(struct db_request_builder * p_rb);
void db_request_builder_parse_update(struct db_request_builder * p_rb);

#endif // H_DB_REQUEST_BUILDER
