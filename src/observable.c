#include "observable.h"
#include "app.h"
#include "object.h"

struct db_observable
{
	struct db_app * p_app;

	db_observable_callback * p_callbacks;
};


APP_ALLOC(observable)
APP_CREATE(observable)
//APP_CLONE(observable)
APP_DISPOSE(observable)

void db_observable_init(struct db_observable * p_observable, struct db_app * p_app)
{
	p_observable->p_app = p_app;

	if(!p_observable->p_callbacks)
	{
		p_observable->p_callbacks = malloc(10 * sizeof *p_observable->p_callbacks);
		memset(p_observable->p_callbacks, 0, 10 * sizeof *p_observable->p_callbacks);
	}
}

void db_observable_clean(struct db_observable * p_observable)
{
	if(p_observable->p_callbacks)
		free(p_observable->p_callbacks);
}

void db_observable_listen(struct db_observable * p_observable, db_observable_callback callback)
{
	db_observable_callback *p_callback = p_observable->p_callbacks;

	do if(p_callback && !*p_callback)
	{
		*p_callback = callback;
		break;
	}
	while(++p_callback < p_observable->p_callbacks + 10);
}

void db_observable_notify(struct db_observable * p_observable, void * p_data)
{
	db_observable_callback *p_callback = p_observable->p_callbacks;

	do if(p_callback && *p_callback)
		(*p_callback)(p_observable, p_data);
	while(++p_callback < p_observable->p_callbacks + 10);
}

