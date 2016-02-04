#ifndef H_ZOB_CLIENT
#	define H_ZOB_CLIENT

struct zob_client;
struct zob_app;

#	include <stdbool.h>

void zob_client_create(struct zob_client ** pp_client);
void zob_client_alloc(struct zob_client ** pp_client);
void zob_client_init(struct zob_client * p_client);
void zob_client_clean(struct zob_client * p_client, bool has_to_dispose);

int zob_client_run(struct zob_client * p_client);

void zob_client_connect(struct zob_client * p_client, char const * sockname);
void zob_client_send(struct zob_client * p_client, int fd);
void zob_client_recv(struct zob_client * p_client);

#endif //H_ZOB_CLIENT
