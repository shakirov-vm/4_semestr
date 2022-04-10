
#include "stddef.h"

#ifndef MINIMAL_TABLE
#define MINIMAL_TABLE 10
#endif

#ifndef MAX_LOAD_FACTOR
#define MAX_LOAD_FACTOR 0.5
#endif

#ifndef MAX_LOAD_FACTOR_WITH_DEL
#define MAX_LOAD_FACTOR_WITH_DEL 0.5
#endif

#ifndef KEY
#define KEY int
#endif

#ifndef VALUE
#define VALUE int
#endif

typedef size_t (*hash_func)(char*);

size_t hash_pol(char* data_);

typedef struct table_elem {

	KEY key;
	VALUE value;
	int deleted; // 0 is not deleted, 1 is deleted
} table_element;

table_element* create_table_element(KEY key_, VALUE value_);

typedef struct hash_tbl {

	table_element** vector;
	size_t size;
	size_t size_with_del;
	size_t capacity;
	hash_func count_hash;
	double load_factor;
	double load_factor_with_del;
} hash_table;

void foreach(hash_table* table, void(*callback)(table_element*, void*), void* data);
hash_table* create_hash_table(hash_func hash);
void delete_hash_table(hash_table* table);
int hash_table_is_empty(hash_table* table);
void calc_load_factors(hash_table* table);
size_t calc_hash(hash_table* table, KEY* key_);

int insert(hash_table* table, KEY key_, VALUE value_);
	// return 0 - insert true
	// return 1 - insert false, because this element is already in table
	// return 2 - insert false, because can't allocate memory

table_element* find(hash_table* table, KEY key_);

int erase(hash_table* table, KEY key_);
	// return 0 - erase true
	// return 1 - erase false, because this element is already deleted
	// return 2 - erase false, because this element is not in the table

int rehash(hash_table* table);
int resize(hash_table* table);
	// return 0 - true
	// return 1 - false, because can't allocate memory

void dump(hash_table* table);

