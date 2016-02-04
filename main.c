#include "src/app.h"

#include <stdlib.h>

int main(int argc, char *argv[])
{
	struct db_app * p_app;
	db_app_alloc(&p_app);
	db_app_init(p_app);
	db_app_singleton_set(p_app);

	db_app_command(gp_app, argc, argv);
	db_app_setup(gp_app);
	return db_app_run(gp_app);
}
