#include "store.h"
#include "app.h"
#include "object.h"
#include "string.h"

#include <stdlib.h>
#include <unistd.h>
#include <db.h>

struct zob_store
{
	DB * p_dbh;
};

#	define CHECK_BDB_INT(int_val)\
		if((int_val) < 0) \
			zob_app_error(gp_app, db_strerror(int_val), __FILE__, __LINE__);


APP_ALLOC(store)
APP_CREATE(store)
APP_DISPOSE(store)
APP_CLONE(store)

void zob_store_init(struct zob_store * p_store)
{
	(void) *p_store;
}

void zob_store_connect(struct zob_store * p_store, struct zob_string * p_file_name)
{
	char * filename = NULL;
	int ret = 0;

	zob_string_get(p_file_name, &filename);
	ret = db_create(&p_store->p_dbh, NULL, 0);
	CHECK_BDB_INT(ret);

	ret = p_store->p_dbh->open(p_store->p_dbh, NULL, filename, NULL, DB_BTREE, DB_CREATE, 0);
	CHECK_BDB_INT(ret);
}

void zob_store_copy(struct zob_store * p_from, struct zob_store * p_to)
{
	zob_app_error(gp_app, "Not implemented", __FILE__, __LINE__);
}

void zob_store_clean(struct zob_store * p_store, bool has_to_dispose)
{
	if(has_to_dispose)
		if(p_store->p_dbh)
			p_store->p_dbh->close(p_store->p_dbh, 0);

	memset(p_store, 0, sizeof *p_store);
}

void zob_store_write(
		struct zob_store * p_store,
		struct zob_string * p_key,
		struct zob_string * p_value,
		bool is_overwrite,
		bool * p_is_ok)
{
	DBT key, data;
	memset(&key, 0, sizeof key);
	memset(&data, 0, sizeof data);

	zob_string_size_get(p_key, (size_t *) &key.size);
	zob_string_get(p_key, (char**) &key.data);

	zob_string_size_get(p_value, (size_t *) &data.size);
	zob_string_get(p_value, (char **)&data.data);

	int ret = p_store->p_dbh->put(p_store->p_dbh, NULL, &key, &data,
			is_overwrite ? 0 : DB_NOOVERWRITE);
	*p_is_ok = (ret == 0);
	if(DB_KEYEXIST == ret)
		return;

	CHECK_BDB_INT(ret);

	p_store->p_dbh->sync(p_store->p_dbh, 0);
}

void zob_store_read(
		struct zob_store * p_store,
		struct zob_string * p_key,
		struct zob_string * p_value,
		bool * p_is_ok)
{
	DBT key, data;
	char buffer[1024];

	memset(&key, 0, sizeof key);
	memset(&data, 0, sizeof data);
	memset(buffer, 0, sizeof buffer / sizeof buffer[0]);

	zob_string_size_get(p_key, (size_t *) &key.size);
	zob_string_get(p_key, (char **)&key.data);

	data.ulen = sizeof buffer;
	data.data = buffer;
	data.flags = DB_DBT_USERMEM;

	int ret = p_store->p_dbh->get(p_store->p_dbh, NULL, &key, &data, 0);
	*p_is_ok = (ret == 0);

	if(DB_NOTFOUND == ret)
		return;

	CHECK_BDB_INT(ret);

	zob_string_set(p_value, buffer);
}

void zob_store_delete(
		struct zob_store * p_store,
		struct zob_string * p_key,
		bool * p_is_ok)
{
	DBT key;
	memset(&key, 0, sizeof key);

	zob_string_size_get(p_key, (size_t *) &key.size);
	zob_string_get(p_key, (char **)&key.data);

	int ret = p_store->p_dbh->del(p_store->p_dbh, NULL, &key, 0);
	*p_is_ok = (ret == 0);

	if(DB_NOTFOUND == ret)
		return;

	CHECK_BDB_INT(ret);
}
