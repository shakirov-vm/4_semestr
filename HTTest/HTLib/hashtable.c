#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "./hashtable.h"

int ht_realloc(hash_table* table, size_t new_capacity);
void ht_calc_load_factors(hash_table* table);
size_t ht_calc_hash(hash_table* table, KEY* key_);

#ifdef DEBUG
size_t alloc_mem = 0;
size_t alloc_mem_cal = 0;
#endif

#ifdef DEBUG
void* ht_malloc(size_t mem_size) {
	if (alloc_mem == 25) {
		alloc_mem = 0;
		return NULL;
	}
	else {
		alloc_mem++;
		return malloc(mem_size);
	}
}
void* ht_calloc(size_t number, size_t size) {
	if (alloc_mem_cal == 25) {
		alloc_mem_cal = 0;
		return NULL;
	}
	else {
		alloc_mem_cal++;
		return calloc(number, size);
	}	
}
#endif

#ifdef RELEASE
void* ht_malloc(size_t mem_size) { 
	return malloc(mem_size); 
}
void* ht_calloc(size_t number, size_t size) { 
	return calloc(number, size); 
}
#endif

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

table_element* ht_create_element(KEY key_, VALUE value_) {

	table_element* ret = (table_element*) ht_malloc (sizeof(table_element));
	if (ret == NULL) 
		return NULL;

	ret->key = key_;
	ret->value = value_;
	ret->deleted = 0;

	return ret;
}

hash_table* ht_create_container(hash_func hash) {

	hash_table* ret = (hash_table*) ht_calloc (1, sizeof(hash_table));
	if (ret == NULL) 
		return NULL;

	ret->count_hash = hash;
	ret->capacity = MINIMAL_TABLE;
	ret->size = 0;
	ret->size_deleted = 0;
	ret->vector = (table_element**) ht_calloc (ret->capacity, sizeof(table_element*));
	if (ret->vector == NULL) {

		free(ret);
		return NULL;
	}

	ht_calc_load_factors(ret);

	return ret;
}

void ht_delete_container(hash_table* table) {

	if (table == NULL) 
		return;

	for(size_t i = 0; i < table->capacity; i++) free(table->vector[i]);

	free(table->vector);
	free(table);
}

int ht_is_empty(hash_table* table) {
	
	if (table->size == 0) 
		return 1; // 1 - is empty - true
	
	return 0; // 0 - not empty - false
}

void ht_calc_load_factors(hash_table* table) {

	table->load_factor = (double) table->size / table->capacity;
	table->garbage_factor = (double) table->size_deleted / (table->size + 1);
}

size_t ht_calc_hash(hash_table* table, KEY* key_) {

	return table->count_hash((char*) key_) % (table->capacity);
}

int ht_insert(hash_table* table, KEY key_, VALUE value_) {

	// return 0 - insert true
	// return 1 - insert false, because this element is already in table
	// return 2 - insert false, because can't allocate memory

	int ret = -1;

	ht_calc_load_factors(table);

	if (table->load_factor > MAX_LOAD_FACTOR)
		ret = ht_resize(table);
	else if (table->garbage_factor > MAX_GARBAGE_FACTOR) 
		ret = ht_rehash(table);

    if (ret == 1) 
    	return 2;

    size_t iter = ht_calc_hash(table, &key_);
    size_t i = 0;
    size_t first_deleted = -1;
    										
    while (table->vector[iter] != NULL && i < table->capacity) {

        if (table->vector[iter]->value == value_ && table->vector[iter]->deleted == 0) 
        	return 1;
        if (table->vector[iter]->deleted && first_deleted == -1) 
        	first_deleted = iter;
        
        iter = (iter + 1) % table->capacity;
        ++i;
    }

    if (first_deleted == -1) {

        table->vector[iter] = ht_create_element(key_, value_);
        if (table->vector[iter] == NULL) 
        	return 2;

    }
    else {

    	--table->size_deleted;
        
        table->vector[first_deleted]->key = key_;
        table->vector[first_deleted]->value = value_;
        table->vector[first_deleted]->deleted = 0;
    }

    ++table->size;
    return 0;
}

table_element* ht_find(hash_table* table, KEY key_) { 

	size_t iter = ht_calc_hash(table, &key_);
	size_t i = 0;

	while (table->vector[iter] != NULL && i < table->capacity) {

		if (table->vector[iter]->key == key_) {

			if (table->vector[iter]->deleted == 1) 
				return NULL;
			else 
				return table->vector[iter];
		}
		iter = (iter + 1) % table->capacity;
		++i;
	}
	return NULL;
}

int ht_erase(hash_table* table, KEY key_) { 

	// return 0 - erase true
	// return 1 - erase false, because this element is already deleted
	// return 2 - erase false, because this element is not in the table

	size_t iter = ht_calc_hash(table, &key_);
	size_t i = 0;

	while (table->vector[iter] != NULL && i < table->capacity) {

		if (table->vector[iter]->key == key_) {
			
			if (table->vector[iter]->deleted == 1) 
				return 1;
			
			else {
					
				table->vector[iter]->deleted = 1;
				++table->size_deleted;
				--table->size;

				if (table->vector[iter + 1] == NULL) {

					while (table->vector[iter] != NULL && table->vector[iter]->deleted != 0) {
						
						free(table->vector[iter]);
						table->vector[iter] = NULL;
		
						iter = (iter - 1) % table->capacity;
						--table->size_deleted;
					}
				}

				return 0;
			}
		}
		iter = (iter + 1) % table->capacity;
		++i;
	}
	return 2;
}

int ht_realloc(hash_table* table, size_t new_capacity) {

	// return 0 - true
	// return 1 - false, because can't allocate memory
	
	table->size = 0;
	table->size_deleted = 0;
	
	table_element** old_vector = table->vector;
	ht_is_empty(table);
	table->vector = (table_element**) ht_calloc (new_capacity, sizeof(table_element*));
	if (table->vector == NULL) {
		table->vector = old_vector;
		return 1;
	}

	for (size_t i = 0; i < table->capacity; i++) {

		if (old_vector[i] != NULL && old_vector[i]->deleted != 1) {

			ht_insert(table, old_vector[i]->key, old_vector[i]->value); // This is recursion!!
			// There always true
		}
	}
	free(old_vector);

	table->capacity = new_capacity;
	
	return 0;
}

int ht_rehash(hash_table* table) {

	// return 0 - true
	// return 1 - false, because can't allocate memory

	int ret = ht_realloc(table, table->capacity);
	
	if (ret != 0) 
		return 1;

	return 0;
}

int ht_resize(hash_table* table) {

	// return 0 - true
	// return 1 - false, because can't allocate memory

	int ret = ht_realloc(table, table->capacity * 2);
	
	if (ret != 0) 
		return 1;

	return 0;
}

void ht_foreach(hash_table* table, void(*callback)(table_element*, void*), void* data) {

	for(size_t i = 0; i < table->capacity; i++) {
		
		//if (table->vector[i] != NULL) printf("[%d;%d;%d - %ld] ", table->vector[i]->key, table->vector[i]->value, table->vector[i]->deleted, i);
		//else printf("[NULL - %ld] ", i);
		
		if (table->vector[i] != NULL && table->vector[i]->deleted == 0) 
			callback(table->vector[i], data);
	}
}
