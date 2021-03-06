#ifndef H_ZOB_STRING // H_ZOB_STRING
#	define H_ZOB_STRING // H_ZOB_STRING

struct zob_string;
struct zob_app;

#	include <stddef.h>
#	include <stdbool.h>

void zob_string_vector_alloc(struct zob_string *** ppp, size_t count);
void zob_string_vector_create(struct zob_string *** ppp, size_t count);
void zob_string_vector_dispose(struct zob_string *** pp);
void zob_string_vector_clone(struct zob_string ** pp, struct zob_string *** ppp);
void zob_string_vector_clean(struct zob_string ** pp, bool has_to_dispose);
void zob_string_vector_copy(struct zob_string ** p_from, struct zob_string ** p_to);

void zob_string_create(struct zob_string ** pp_string);
void zob_string_init(struct zob_string * p_string);

void zob_string_create_from_cstring(struct zob_string ** pp_string, char * p_raw);

void zob_string_compare(struct zob_string * p_lhs, struct zob_string * p_rhs, int * p_diff);

void zob_string_set(struct zob_string * p_string, char * p_data);
void zob_string_set_options(struct zob_string * p_string, char * p_data);

void zob_string_size_set(struct zob_string * p_string, size_t size);
void zob_string_size_get(struct zob_string * p_string, size_t * p_size);

void zob_string_get(struct zob_string * p_string, char ** pp_text);
void zob_string_slice_get(struct zob_string * p_string, size_t first, size_t last, struct zob_string ** p_slice);

void zob_string_clone(struct zob_string * p_string, struct zob_string ** pp_string);
void zob_string_copy(struct zob_string * p_from, struct zob_string * p_to);

void zob_string_clean(struct zob_string * p_string, bool has_to_dispose);
void zob_string_dispose(struct zob_string ** pp_string);

void zob_string_write(struct zob_string * p_string, size_t from, const char * p_text, size_t * p_written);

void zob_string_find_char(
		struct zob_string * p_string,
		char needle,
		size_t first,
		size_t last,
		size_t * p_position,
		bool * p_has_found
		);

void zob_string_find_string(
		struct zob_string * p_string,
		char const * p_needle,
		size_t first,
		size_t last,
		size_t * p_position,
		bool * p_has_found
		);

void zob_string_fill(struct zob_string * p_string, size_t size, char value);

#endif // H_ZOB_STRING

