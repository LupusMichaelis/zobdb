#ifndef H_ZOB_REQUEST_BUILDER
#	define H_ZOB_REQUEST_BUILDER

#	include "message.h"

#	include <stdbool.h>

struct zob_app;
struct zob_request_builder;

void zob_request_builder_alloc(struct zob_request_builder ** pp_rb);
void zob_request_builder_create(struct zob_request_builder ** pp_request_builderp);
void zob_request_builder_init(struct zob_request_builder * p_request_builder);
void zob_request_builder_dispose(struct zob_request_builder ** pp_rb);
void zob_request_builder_clean(struct zob_request_builder * p_rb, bool has_to_dispose);
void zob_request_builder_copy(struct zob_request_builder * p_orig, struct zob_request_builder * p_dest);

void zob_request_builder_parse(
		struct zob_request_builder * p_builder,
		const char * p_text,
		bool * need_moar
		);

void zob_request_builder_get_request(struct zob_request_builder * p_request, struct zob_message ** pp_message);

void zob_request_builder_is_bad_request(struct zob_request_builder * p_rb, bool * is_bad_request);

void zob_request_builder_find_verb(struct zob_request_builder * p_rb);
void zob_request_builder_parse_new(struct zob_request_builder * p_rb);
void zob_request_builder_parse_clone(struct zob_request_builder * p_rb);
void zob_request_builder_parse_read(struct zob_request_builder * p_rb);
void zob_request_builder_parse_delete(struct zob_request_builder * p_rb);
void zob_request_builder_parse_update(struct zob_request_builder * p_rb);
void zob_request_builder_parse_stop(struct zob_request_builder * p_rb);

#endif // H_ZOB_REQUEST_BUILDER
