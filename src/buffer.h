#ifndef H_DB_BUFFER // H_DB_BUFFER
#	define H_DB_BUFFER // H_DB_BUFFER

struct db_buffer;
struct db_app;

#	define DB_B_AUTO_VAR 1 << 0
#	define DB_B_FIXED_SIZE 1 << 1

#	include <stddef.h>
#	include <stdbool.h>

void db_buffer_create(struct db_buffer ** pp_buffer, struct db_app * p_app);
void db_buffer_init(struct db_buffer * p_buffer, struct db_app * p_app);

void db_buffer_set_is_auto(struct db_buffer * p_buffer, bool is_auto);

void db_buffer_get(struct db_buffer * p_buffer, char ** pp_text);
void db_buffer_get_data(struct db_buffer * p_buffer, size_t first, size_t last, char ** p_string);

void db_buffer_clone(struct db_buffer * p_buffer, struct db_buffer ** pp_buffer);
void db_buffer_copy(struct db_buffer * p_from, struct db_buffer * p_to);

void db_buffer_clean(struct db_buffer * p_buffer, bool has_to_dispose);
void db_buffer_dispose(struct db_buffer ** pp_buffer);

void db_buffer_ensure(struct db_buffer * p_buffer, size_t from, size_t input_size);
void db_buffer_write(struct db_buffer * p_buffer, size_t * p_from, const char * p_text);

void db_buffer_find_char(
		struct db_buffer * p_buffer,
		char const needle,
		size_t const first,
		size_t const last,
		size_t * p_position,
		bool * p_has_found
		);

void db_buffer_find_string(
		struct db_buffer * p_buffer,
		char const * p_needle,
		size_t first,
		size_t last,
		size_t * p_position,
		bool * p_has_found
		);

void db_buffer_fill(struct db_buffer * p_buffer, size_t size, char value);

#endif // H_DB_BUFFER
