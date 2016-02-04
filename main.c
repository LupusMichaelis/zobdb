#include "src/app.h"

#include <stdlib.h>

int main(int argc, char *argv[])
{
	struct zob_app * p_app;
	zob_app_alloc(&p_app);
	zob_app_init(p_app);
	zob_app_singleton_set(p_app);

	zob_app_command(gp_app, argc, argv);
	zob_app_setup(gp_app);
	return zob_app_run(gp_app);
}
