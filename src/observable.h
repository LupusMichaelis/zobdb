#ifndef H_ZOB_OBSERVABLE // H_ZOB_OBSERVABLE
#	define H_ZOB_OBSERVABLE // H_ZOB_OBSERVABLE

struct zob_observable;
struct zob_app;
typedef void (*zob_observable_callback) (struct zob_observable * p_observable, void * p_data);

#	include <stddef.h>
#	include <stdbool.h>

void zob_observable_create(struct zob_observable ** pp_observable);
void zob_observable_init(struct zob_observable * p_observable);
void zob_observable_clone(struct zob_observable * p_observable, struct zob_observable ** pp_observable);
void zob_observable_clean(struct zob_observable * p_observable, bool has_to_dispose);
void zob_observable_dispose(struct zob_observable ** pp_observable);

void zob_observable_listen(struct zob_observable * p_observable, zob_observable_callback callback);
void zob_observable_notify(struct zob_observable * p_observable, void * p_data);

#endif // H_ZOB_OBSERVABLE

