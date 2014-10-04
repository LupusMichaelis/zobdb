

#include "../src/app.h"
#include "../src/observable.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int main(int argc, char ** argv)
{
	struct db_app * p_app = NULL;
	db_app_alloc(&p_app);
	db_app_init(p_app, argc, argv);

	struct db_observable * p_observable = NULL;

	{
		db_observable_create(&p_observable, p_app);
		db_observable_notify(p_observable, "Message");
		db_observable_dispose(&p_observable);
	}

	{
		static bool witness = false;
		char * p_received = NULL;

		void listener(struct db_observable * p_observable, void * p_message)
		{
			p_received = (char *) p_message;
			witness = true;
		}

		db_observable_create(&p_observable, p_app);
		db_observable_listen(p_observable, listener);

		char * p_given = "Message";
		db_observable_notify(p_observable, p_given);
		db_observable_dispose(&p_observable);

		assert(p_given == p_received);
		assert(witness);
	}

	{
		char * p_received = NULL;
		int received = 0;

		void get_string(struct db_observable * p_observable, void * p_message)
		{
			p_received = (char *) p_message;
		}

		void get_int(struct db_observable * p_observable, void * p_message)
		{
			received = atoi((char *) p_message);
		}

		db_observable_create(&p_observable, p_app);
		db_observable_listen(p_observable, get_string);
		db_observable_listen(p_observable, get_int);

		char * p_given = "42";
		db_observable_notify(p_observable, p_given);
		db_observable_dispose(&p_observable);

		assert(0 == strcmp(p_given, p_received));
		assert(42 == received);
	}

	printf("Huge success\n");
	return 0;
}
