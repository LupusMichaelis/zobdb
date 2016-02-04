#include "observable.h"
#include "app.h"
#include "object.h"

struct zob_observable
{
	zob_observable_callback * p_callbacks;
};

APP_ALLOC(observable)
APP_CREATE(observable)
//APP_CLONE(observable)
APP_DISPOSE(observable)

void zob_observable_init(struct zob_observable * p_observable)
{
	if(!p_observable->p_callbacks)
	{
		p_observable->p_callbacks = malloc(10 * sizeof *p_observable->p_callbacks);
		memset(p_observable->p_callbacks, 0, 10 * sizeof *p_observable->p_callbacks);
	}
}

void zob_observable_clean(struct zob_observable * p_observable, bool has_to_dispose)
{
	if(has_to_dispose)
		if(p_observable->p_callbacks)
			free(p_observable->p_callbacks);
}

void zob_observable_listen(struct zob_observable * p_observable, zob_observable_callback callback)
{
	zob_observable_callback *p_callback = p_observable->p_callbacks;

	do if(p_callback && !*p_callback)
	{
		*p_callback = callback;
		break;
	}
	while(++p_callback < p_observable->p_callbacks + 10);
}

void zob_observable_notify(struct zob_observable * p_observable, void * p_data)
{
	zob_observable_callback *p_callback = p_observable->p_callbacks;

	do if(p_callback && *p_callback)
		(*p_callback)(p_observable, p_data);
	while(++p_callback < p_observable->p_callbacks + 10);
}

