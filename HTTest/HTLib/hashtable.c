#include <stdlib.h>
#include <string.h>

#include "./hashtable.h"

size_t hash_pol(char* data_) {

	size_t hash = 0;
	size_t prime = 257;
	size_t len = strlen(data_);
	size_t degree = prime;

	for (size_t i = 0; i < len; i++) {
		hash += data_[i] * degree;
		degree *= prime;
	}
	return hash;
}

struct table_element* create_table_element(KEY key_, VALUE value_) {

	struct table_element* ret = (struct table_element*) malloc (sizeof(struct table_element));

	ret->key = key_;
	ret->value = value_;
	ret->deleted = 0;

	return ret;
}

struct hash_table* create_hash_table(hash_func hash) {

	struct hash_table* ret = (hash_table*) calloc (1, sizeof(struct hash_table));

	ret->count_hash = hash;
	ret->capacity = MINIMAL_TABLE;
	ret->size = 0;
	ret->vector = (table_element*) calloc (ret->capacity, sizeof(struct table_element*));
}

void delete_hash_table(struct hash_table* table) {

	for(size_t i = 0; i < table->capacity; i++) free(table->vector[i]);

	free(table->vector);
	free(table);
}

int hash_table_is_empty(struct hash_table* table) {

	if (table->size == 0) return 1; // 1 - is empty - true
	return 0; // 0 - not empty - false
}

size_t calc_hash(struct hash_table* table, KEY key_) {

	return table->hash((char*) key_) % (table->capacity);
}

int insert(struct hash_table* table, KEY key_, VALUE value_) {

	// return 0 - insert true
	// return 1 - insert false, because this element is already in table

	if (table->size * 2 >= table->capacity) resize(table);
    else if (table->size_with_del > 2 * table->size) rehash(table); // When we do it?
    // ^^
    // || this is load_factors

    size_t iter = calc_hash(table, key_);

    size_t i = 0;
    size_t first_deleted = -1;
    										
    while (table->vector[iter] != NULL && i < table->capacity) {

        if (table->vector[iter]->value == value_ && table->vector[iter]->deleted == 0) return 1;
        if (table->vector[iter]->deleted && first_deleted == -1) first_deleted = iter;
        iter = (iter + 1) % table->capacity;
        ++i;
    }

    if (first_deleted == -1) {

        table->vector[iter] = create_table_element(key_, value_);
        ++(table->size_with_del);
    }
    else {

        table->vector[first_deleted]->key = key_;
        table->vector[first_deleted]->value = value_;
        table->vector[first_deleted]->deleted = 0;
    }
    
    ++table->size;
    return 0;
}

struct table_element* find(struct hash_table* table, KEY key_) { 

	size_t iter = calc_hash(table, key_);
	size_t i = 0;

	while (table->vector[iter] != NULL && i < table->capacity) {
	
		if (table->vector[iter]->key == key_) {
			
			if (table->vector[iter]->deleted == 1) return NULL;
			else return table->vector[iter];
		}
		iter = (iter + 1) % table->capacity;
		++i;
	}
	return NULL;
}

int erase(struct hash_table* table, KEY key_) { 

	// return 0 - insert true
	// return 1 - insert false, because this element is already deleted
	// return 2 - insert false, because this element is not in the table

	size_t iter = calc_hash(table, key_);
	size_t i = 0;

	while (table->vector[iter] != NULL && i < table->capacity) {

		if (table->vector[iter]->key == key_) {

			if (table->vector[iter]->deleted == 0) return 1;
			else {
				table->vector[iter]->deleted = 1;
				return 0;
			}
		}
		iter = (iter + 1) % table->capacity;
		++i;
	}
	return 2;
}

void rehash(struct hash_table* table) {

	table->size = 0;

	struct table_element* old_vector = table->vector;
	table->vector = (struct table_element*) calloc (table->capacity, sizeof(struct table_element*));

	for (size_t i = 0; i < table->capacity / 2; i++) { // capacity is divided by 2

		if (table->vector[i] != NULL && table->vector[i]->deleted != 1) {

			insert(table, table->vector[i]->key, table->vector[i]->value); // This is recursion!!
			// Or not?
		}

		/*
		if (table->vector[i] != NULL && table->vector[i]->deleted != 1) {

			size_t new_iter = calc_hash(table, table->vector[i]->key);
			tmp_vector[new_iter] = table->vector[i];
		}*/
	}
	free(old_vector);
}

void resize(struct hash_table* table) {

	table->capacity *= 2;
	rehash(table);
}