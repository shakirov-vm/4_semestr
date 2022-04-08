
#include "stddef.h"

#define MINIMAL_TABLE 10

#define KEY char*
#define VALUE char*

#define hash_func size_t(*)(KEY)

size_t hash_pol(char* data_);

struct table_element {

	KEY key;
	VALUE value;
	int deleted; // 0 is not deleted, 1 is deleted
};

struct table_element* create_table_element(KEY key_, VALUE value_);

struct hash_table {

	struct table_element** vector;
	size_t size;
	size_t size_with_del;
	size_t capacity;
	hash_func count_hash;
};

struct hash_table* create_hash_table(hash_func hash);
void delete_hash_table(struct hash_table* table);
int hash_table_is_empty(struct hash_table* table);
size_t calc_hash(struct hash_table* table, KEY key_);
int insert(struct hash_table* table, KEY key_, VALUE value_);
struct table_element* find(struct hash_table* table, KEY key_);
int erase(struct hash_table* table, KEY key_);
void rehash(struct hash_table* table);
void resize(struct hash_table* table);

