#ifndef H_ZOB_BUFFER // H_ZOB_BUFFER
#	define H_ZOB_BUFFER // H_ZOB_BUFFER

struct zob_buffer;
struct zob_app;

#	define DB_B_AUTO_VAR 1 << 0
#	define DB_B_FIXED_SIZE 1 << 1

#	include <stddef.h>
#	include <stdbool.h>

void zob_buffer_create(struct zob_buffer ** pp_buffer);
void zob_buffer_create_from_memory(struct zob_buffer ** pp_buffer, void * p_content, size_t size);
void zob_buffer_init(struct zob_buffer * p_buffer);

void zob_buffer_is_auto_set(struct zob_buffer * p_buffer, bool is_auto);

void zob_buffer_get(struct zob_buffer * p_buffer, char ** pp_text);
void zob_buffer_get_data(struct zob_buffer * p_buffer, size_t first, size_t last, char ** p_string);

void zob_buffer_clone(struct zob_buffer * p_buffer, struct zob_buffer ** pp_buffer);
void zob_buffer_copy(struct zob_buffer * p_from, struct zob_buffer * p_to);

void zob_buffer_clean(struct zob_buffer * p_buffer, bool has_to_dispose);
void zob_buffer_dispose(struct zob_buffer ** pp_buffer);

void zob_buffer_ensure(struct zob_buffer * p_buffer, size_t from, size_t input_size);
void zob_buffer_write(struct zob_buffer * p_buffer, size_t from, size_t length, const char * p_text, size_t * p_written);
void zob_buffer_write_at(struct zob_buffer * p_buffer, size_t at, char item);

void zob_buffer_size_set(struct zob_buffer * p_buffer, size_t size);
void zob_buffer_size_get(struct zob_buffer * p_buffer, size_t * p_size);

void zob_buffer_slice_get(struct zob_buffer * p_buffer, size_t from, size_t to, struct zob_buffer ** pp_slice);
void zob_buffer_compare(struct zob_buffer * p_lhs, struct zob_buffer * p_rhs, int * p_diff);

void zob_buffer_find_char(
		struct zob_buffer * p_buffer,
		char const needle,
		size_t const first,
		size_t const last,
		size_t * p_position,
		bool * p_has_found
		);

void zob_buffer_find_string(
		struct zob_buffer * p_buffer,
		char const * p_needle,
		size_t first,
		size_t last,
		size_t * p_position,
		bool * p_has_found
		);

void zob_buffer_fill(struct zob_buffer * p_buffer, size_t size, char value);

#endif // H_ZOB_BUFFER
