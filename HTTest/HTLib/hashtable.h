#include "stddef.h"

#ifndef DEBUG
#define DEBUG // default
#endif

#ifdef RELEASE
#undef DEBUG
#endif

#ifndef MINIMAL_TABLE
#define MINIMAL_TABLE 10
#endif

#ifndef MAX_LOAD_FACTOR
#define MAX_LOAD_FACTOR 0.6
#endif

#ifndef MAX_GARBAGE_FACTOR
#define MAX_GARBAGE_FACTOR 1
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

table_element* ht_create_element(KEY key_, VALUE value_);

typedef struct hash_tbl {

	table_element** vector;
	size_t size;
	size_t size_deleted;
	size_t capacity;
	hash_func count_hash;
	double load_factor;
	double garbage_factor;
} hash_table;

hash_table* ht_create_container(hash_func hash);
void ht_delete_container(hash_table* table);
int ht_is_empty(hash_table* table);
int ht_insert(hash_table* table, KEY key_, VALUE value_);
	// return 0 - insert true
	// return 1 - insert false, because this element is already in table
	// return 2 - insert false, because can't allocate memory

table_element* ht_find(hash_table* table, KEY key_);

int ht_erase(hash_table* table, KEY key_);
	// return 0 - erase true
	// return 1 - erase false, because this element is already deleted
	// return 2 - erase false, because this element is not in the table

int ht_rehash(hash_table* table);
int ht_resize(hash_table* table);
	// return 0 - true
	// return 1 - false, because can't allocate memory

void ht_foreach(hash_table* table, void(*callback)(table_element*, void*), void* data);
