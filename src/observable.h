#ifndef H_DB_OBSERVABLE // H_DB_OBSERVABLE
#	define H_DB_OBSERVABLE // H_DB_OBSERVABLE

struct db_observable;
struct db_app;
typedef void (*db_observable_callback) (struct db_observable * p_observable, void * p_data);

#	include <stddef.h>
#	include <stdbool.h>

void db_observable_create(struct db_observable ** pp_observable, struct db_app * p_app);
void db_observable_init(struct db_observable * p_observable, struct db_app * p_app);
void db_observable_clone(struct db_observable * p_observable, struct db_observable ** pp_observable);
void db_observable_clean(struct db_observable * p_observable);
void db_observable_dispose(struct db_observable ** pp_observable);

void db_observable_listen(struct db_observable * p_observable, db_observable_callback callback);
void db_observable_notify(struct db_observable * p_observable, void * p_data);

#endif // H_DB_OBSERVABLE

