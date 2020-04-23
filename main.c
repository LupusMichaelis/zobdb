#include "src/app.h"

#include <stdlib.h>
#ifdef DEBUG
#	include <mcheck.h>
#endif

int main(int argc, char *argv[])
{
#ifdef DEBUG
	mtrace();
#endif
	struct zob_app * p_app;
	zob_app_alloc(&p_app);
	zob_app_init(p_app);
	zob_app_singleton_set(p_app);

	zob_app_command(gp_app, argc, argv);
	zob_app_setup(gp_app);

	int status = zob_app_run(gp_app);
#ifdef DEBUG
	muntrace();
#endif

	return status;
}
