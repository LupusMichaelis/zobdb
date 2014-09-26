#ifndef H_DB_STRING // H_DB_STRING
#	define H_DB_STRING // H_DB_STRING

struct db_string;
struct db_app;

#	include <stddef.h>
#	include <stdbool.h>

void db_string_vector_alloc(struct db_string *** ppp, struct db_app * p_app, size_t count);
void db_string_vector_create(struct db_string *** ppp, struct db_app * p_app, size_t count);
void db_string_vector_dispose(struct db_string *** pp);
void db_string_vector_clone(struct db_string ** pp, struct db_string *** ppp);

void db_string_create(struct db_string ** pp_string, struct db_app * p_app);
void db_string_init(struct db_string * p_string, struct db_app * p_app);

void db_string_set(struct db_string * p_string, char * p_data, int options);
void db_string_set_options(struct db_string * p_string, char * p_data);

void db_string_get(struct db_string * p_string, char ** pp_text);
void db_string_get_data(struct db_string * p_string, size_t first, size_t last, char ** p_datas);

void db_string_clone(struct db_string * p_string, struct db_string ** pp_string);
void db_string_copy(struct db_string * p_from, struct db_string * p_to);

void db_string_clean(struct db_string * p_string, bool has_to_dispose);
void db_string_dispose(struct db_string ** pp_string);

void db_string_ensure(struct db_string * p_string, size_t from, size_t input_size);
void db_string_write(struct db_string * p_string, size_t * p_from, const char * p_text);

void db_string_find_char(
		struct db_string * p_string,
		char needle,
		size_t first,
		size_t last,
		size_t * p_position,
		bool * p_has_found
		);

void db_string_find_string(
		struct db_string * p_string,
		char const * p_needle,
		size_t first,
		size_t last,
		size_t * p_position,
		bool * p_has_found
		);

void db_string_fill(struct db_string * p_string, size_t size, char value);

#endif // H_DB_STRING

