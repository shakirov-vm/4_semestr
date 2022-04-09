#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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

table_element* create_table_element(KEY key_, VALUE value_) {

	table_element* ret = (table_element*) malloc (sizeof(table_element));

	ret->key = key_;
	ret->value = value_;
	ret->deleted = 0;

	return ret;
}

hash_table* create_hash_table(hash_func hash) {

	hash_table* ret = (hash_table*) calloc (1, sizeof(hash_table));

	ret->count_hash = hash;
	ret->capacity = MINIMAL_TABLE;
	ret->size = 0;
	ret->vector = (table_element**) calloc (ret->capacity, sizeof(table_element*));

	return ret;
}

void delete_hash_table(hash_table* table) {

	for(size_t i = 0; i < table->capacity; i++) free(table->vector[i]);

	free(table->vector);
	free(table);
}

int hash_table_is_empty(hash_table* table) {

	if (table->size == 0) return 1; // 1 - is empty - true
	return 0; // 0 - not empty - false
}

size_t calc_hash(hash_table* table, KEY* key_) {

	return table->count_hash((char*) key_) % (table->capacity);
}

int insert(hash_table* table, KEY key_, VALUE value_) {

	// return 0 - insert true
	// return 1 - insert false, because this element is already in table

	if (table->size * 2 >= table->capacity) resize(table);
    else if (table->size_with_del > 2 * table->size) rehash(table); // When we do it?
    // ^^
    // || this is load_factors

    size_t iter = calc_hash(table, &key_);

    printf("key is %d, iter is %lld, size - %d, capacity - %d\n", key_, iter, table->size, table->capacity);

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

table_element* find(hash_table* table, KEY key_) { 

	size_t iter = calc_hash(table, &key_);
	size_t i = 0;

	while (table->vector[iter] != NULL && i < table->capacity) {
		//printf("key is %d, iter is %d, deleted is %d\n", key_, table->vector[iter]->key, table->vector[iter]->deleted);
		if (table->vector[iter]->key == key_) {

			if (table->vector[iter]->deleted == 1) return NULL;
			else return table->vector[iter];
		}
		iter = (iter + 1) % table->capacity;
		++i;
	}
	return NULL;
}

int erase(hash_table* table, KEY key_) { 

	// return 0 - erase true
	// return 1 - erase false, because this element is already deleted
	// return 2 - erase false, because this element is not in the table

	size_t iter = calc_hash(table, &key_);
	size_t i = 0;

	while (table->vector[iter] != NULL && i < table->capacity) {

		if (table->vector[iter]->key == key_) {
			printf("find in erase\n");
			if (table->vector[iter]->deleted == 1) return 1;
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

void rehash(hash_table* table) {

	printf("rehash, new capacity is %d\n", table->capacity);
	table->size = 0;
	table->size_with_del = 0;
	
	table_element** old_vector = table->vector;
	table->vector = (table_element**) calloc (table->capacity, sizeof(table_element*));

	for (size_t i = 0; i < table->capacity / 2; i++) { // capacity is divided by 2

		//if (old_vector[i] != NULL) printf("in rehash deleted is %d[%d]\n", old_vector[i]->deleted, i);
		if (old_vector[i] != NULL && old_vector[i]->deleted != 1) {

			insert(table, old_vector[i]->key, old_vector[i]->value); // This is recursion!!
			//printf("in rehash size - %d\n", table->size);
			// Or not?
		}
	}
	free(old_vector);
}

void resize(hash_table* table) {

	table->capacity *= 2;
	rehash(table);
}

void foreach(hash_table* table, void(*callback)(table_element*, void*), void* data) {

	for(size_t i = 0; i < table->capacity; i++) {
	
		if (table->vector[i] != NULL && table->vector[i]->deleted == 0) callback(table->vector[i], data);
	}
}
