#ifndef H_DB_BUFFER // H_DB_BUFFER
#	define H_DB_BUFFER // H_DB_BUFFER

struct db_buffer;
struct db_app;

#	include <stddef.h>
#	include <stdbool.h>

void db_buffer_create(struct db_buffer ** pp_buffer, struct db_app * p_app);
void db_buffer_dispose(struct db_buffer ** pp_buffer);
void db_buffer_ensure(struct db_buffer * p_buffer, size_t from, size_t input_size);
void db_buffer_write(struct db_buffer * p_buffer, size_t * p_from, const char * p_text);

void db_buffer_get_current(struct db_buffer * p_buffer, size_t * p_current_position );
void db_buffer_get(struct db_buffer * p_buffer, char ** pp_text);

void db_buffer_get_string(struct db_buffer * p_buffer, size_t first, size_t last, char ** p_string);

void db_buffer_find_char(
		struct db_buffer * p_db,
		char needle,
		size_t first,
		size_t last,
		size_t * p_position,
		bool * p_has_found
		);

#endif // H_DB_BUFFER
