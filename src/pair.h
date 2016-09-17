#ifndef DB_PAIR
#	define DB_PAIR

struct zob_string;

struct pair
{
	struct zob_string * p_name;
	struct zob_string * p_value;
};

#endif // DB_PAIR
