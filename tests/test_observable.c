

#include "../src/app.h"
#include "../src/observable.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int main(int argc, char ** argv)
{
	struct zob_app * p_app = NULL;
	zob_app_alloc(&p_app);
	zob_app_init(p_app, argc, argv);

	struct zob_observable * p_observable = NULL;

	{
		zob_observable_create(&p_observable, p_app);
		zob_observable_notify(p_observable, "Message");
		zob_observable_dispose(&p_observable);
	}

	{
		static bool witness = false;
		char * p_received = NULL;

		void listener(struct zob_observable * p_observable, void * p_message)
		{
			p_received = (char *) p_message;
			witness = true;
		}

		zob_observable_create(&p_observable, p_app);
		zob_observable_listen(p_observable, listener);

		char * p_given = "Message";
		zob_observable_notify(p_observable, p_given);
		zob_observable_dispose(&p_observable);

		assert(p_given == p_received);
		assert(witness);
	}

	{
		char * p_received = NULL;
		int received = 0;

		void get_string(struct zob_observable * p_observable, void * p_message)
		{
			p_received = (char *) p_message;
		}

		void get_int(struct zob_observable * p_observable, void * p_message)
		{
			received = atoi((char *) p_message);
		}

		zob_observable_create(&p_observable, p_app);
		zob_observable_listen(p_observable, get_string);
		zob_observable_listen(p_observable, get_int);

		char * p_given = "42";
		zob_observable_notify(p_observable, p_given);
		zob_observable_dispose(&p_observable);

		assert(0 == strcmp(p_given, p_received));
		assert(42 == received);
	}

	printf("Huge success\n");
	return 0;
}
