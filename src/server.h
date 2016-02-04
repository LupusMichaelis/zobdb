#ifndef H_ZOB_SERVER
#	define H_ZOB_SERVER

struct zob_app;
struct zob_server;
struct zob_message;

void zob_server_create(struct zob_server ** pp_server);
void zob_server_alloc(struct zob_server ** pp_server);
void zob_server_init(struct zob_server * p_server);
void zob_server_clean(struct zob_server * p_server, bool has_to_dispose);

int zob_server_run(struct zob_server * p_server);
void zob_server_listen(struct zob_server * p_server, const char * socket_path);
void zob_server_session_begin(struct zob_server * p_server);
void zob_server_session_end(struct zob_server * p_server);
void zob_server_read(struct zob_server * p_server, char **pp_payload);
void zob_server_answer(struct zob_server * p_server, struct zob_message * p_answer);
void zob_server_process(struct zob_server * p_server, struct zob_message * p_request, struct zob_message * p_answer);

#endif //H_ZOB_SERVER
