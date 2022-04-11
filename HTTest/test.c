#include <stdio.h>

#include "./HTLib/hashtable.h"

extern size_t alloc_mem;
extern size_t alloc_mem_cal;

enum TEST {
	TEST_SUCCESS = 0,
	TEST_FAILED = 1,
	TEST_FAILED_INSERT = 2,
	TEST_FAILED_ERASE = 3,
	TEST_FAILED_REHASH = 4,
	TEST_FAILED_RESIZE = 5
};

void null_alloc_mem() {

	alloc_mem_cal = 0;
	alloc_mem 	  = 0;
}

int double_is_equal(double first, double second) {
	if (((first - second) < 0.00001) || ((second - first) < 0.00001)) return 1;
	return 0;
}

int check_elem(hash_table* table, int iter, int key_, int value_, int deleted_) {

	if (table->vector[iter]->key != key_) return TEST_FAILED;
	if (table->vector[iter]->value != value_) return TEST_FAILED;
	if (table->vector[iter]->deleted != deleted_) return TEST_FAILED;

	return TEST_SUCCESS;
}

int test_create_delete();
int test_insert_erase();
int test_is_empty();
int test_find();
int test_rehash_resize();
int test_foreach();

void print_elems(table_element* elem, void* data) {

	//printf("[%d;%d;%d] ", elem->key, elem->value, elem->deleted);
}

int dump_wrap(hash_table* table) {

	printf("DUMP:\n");
	ht_foreach(table, print_elems, NULL);
	printf("\n");

	return 0;
}

int main() {

	null_alloc_mem();
	int ret = 0;

	ret = test_create_delete();
	if (ret == TEST_FAILED) printf("Create failed\n");
	
	null_alloc_mem();

	ret = test_insert_erase();
	if (ret == TEST_FAILED_INSERT) printf("Insert failed\n");
	if (ret == TEST_FAILED_ERASE) printf("Erase failed\n");

	null_alloc_mem();

	ret = test_is_empty();
	if (ret == TEST_FAILED) printf("Is_empty failed\n");

	null_alloc_mem();

	ret = test_find();
	if (ret == TEST_FAILED) printf("Find failed\n");

	null_alloc_mem();

	ret = test_rehash_resize();
	if (ret == TEST_FAILED_REHASH) printf("Rehash failed\n");
	if (ret == TEST_FAILED_RESIZE) printf("Resize failed\n");

	null_alloc_mem();

	ret = test_foreach();
	if (ret == TEST_FAILED) printf("Foreach failed\n");

	printf("Tests end\n");
}

int test_create_delete() {

	hash_table* tmp = ht_create_container(hash_pol);

	if (tmp == NULL) return TEST_FAILED;
	if (tmp->vector == NULL) return TEST_FAILED;
	if (tmp->size != 0) return TEST_FAILED;
	if (tmp->capacity != MINIMAL_TABLE) return TEST_FAILED;
	if (tmp->count_hash != hash_pol) return TEST_FAILED;
	if (!double_is_equal(tmp->load_factor, 0)) return TEST_FAILED;
	if (!double_is_equal(tmp->load_factor_with_del, 0)) return TEST_FAILED;

	alloc_mem_cal = 25;
	hash_table* tmp_1 = ht_create_container(hash_pol);
	if (tmp_1 != NULL) return TEST_FAILED;

	alloc_mem_cal = 24;
	tmp_1 = ht_create_container(hash_pol);
	if (tmp_1 != NULL) return TEST_FAILED;

	ht_delete_container(tmp);
	ht_delete_container(tmp_1);

	return TEST_SUCCESS;
}

int test_insert_erase() {

	// There not expected rehash and resize
	// Expect that MINIMAL_TABLE == 10, LOAD_FACTORS - 0.6
 	int ret;

	hash_table* tmp = ht_create_container(hash_pol);

	ret = ht_insert(tmp, 1, 2);
	if (ret != 0) return TEST_FAILED_INSERT;
	ret = ht_insert(tmp, 2, 4);
	if (ret != 0) return TEST_FAILED_INSERT;

	if (check_elem(tmp, 4, 2, 4, 0) == TEST_FAILED) return TEST_FAILED_INSERT;
	if (check_elem(tmp, 7, 1, 2, 0) == TEST_FAILED) return TEST_FAILED_INSERT;

	for (int i = 0; i < 10; i++) {

		if (i != 4 && i != 7 && tmp->vector[i] != NULL) return TEST_FAILED_INSERT;
	}

	ret = ht_insert(tmp, 2, 4);
	if (ret != 1) return TEST_FAILED_INSERT;
	ret = ht_insert(tmp, 12, 144);
	if (ret != 0) return TEST_FAILED_INSERT;

	if (check_elem(tmp, 5, 12, 144, 0) == TEST_FAILED) return TEST_FAILED_INSERT;

	ret = ht_insert(tmp, 15, 225);
	if (ret != 0) return TEST_FAILED_INSERT;

	if (check_elem(tmp, 6, 15, 225, 0) == TEST_FAILED) return TEST_FAILED_INSERT;

	alloc_mem = 25;

	ret = ht_insert(tmp, 3, 9);
	if (ret != 2) return TEST_FAILED_INSERT;

	for (int i = 0; i < 10; i++) {

		if (i != 4 && i != 5 && i != 6 && i != 7 && tmp->vector[i] != NULL) return TEST_FAILED_INSERT;
	}

	ret = ht_erase(tmp, 12);
	if (ret != 0) return TEST_FAILED_ERASE;

	ret = ht_erase(tmp, 12);
	if (ret != 1) return TEST_FAILED_ERASE;

	if (tmp->vector[5] == NULL) return TEST_FAILED_ERASE;
	if (check_elem(tmp, 5, 12, 144, 1) == TEST_FAILED) return TEST_FAILED_ERASE;

	ret = ht_insert(tmp, 12, 144);
	if (ret != 0) return TEST_FAILED_INSERT;

	if (check_elem(tmp, 5, 12, 144, 0) == TEST_FAILED) return TEST_FAILED_INSERT;

	ret = ht_erase(tmp, 10);
	if (ret != 2) return TEST_FAILED_ERASE;

	ht_delete_container(tmp);

	return TEST_SUCCESS;
}

int test_is_empty() {

	int ret;

	hash_table* tmp = ht_create_container(hash_pol);

	ret = ht_is_empty(tmp);
	if (ret != 1) return TEST_FAILED;

	ht_insert(tmp, 1, 1);
	
	ret = ht_is_empty(tmp);
	if (ret != 0) return TEST_FAILED;

	return TEST_SUCCESS;
}

int test_find() {
	
	table_element* ret;

	hash_table* tmp = ht_create_container(hash_pol);

	ht_insert(tmp, 1, 2);

	ret = ht_find(tmp, 1);
	if (ret->key != 1) return TEST_FAILED;
	if (ret->value != 2) return TEST_FAILED;
	if (ret->deleted != 0) return TEST_FAILED;

	ht_erase(tmp, 1);

	ret = ht_find(tmp, 1);
	if (ret != NULL) return TEST_FAILED;

	ret = ht_find(tmp, 2);
	if (ret != NULL) return TEST_FAILED;

	ht_delete_container(tmp);

	hash_table* tmp_1 = ht_create_container(hash_pol);

	for (int i = 2; i < 42; i += 10) ht_insert(tmp_1, i, i);

	ret = ht_find(tmp_1, 32);
	if (ret->key != 32) return TEST_FAILED;
	if (ret->value != 32) return TEST_FAILED;
	if (ret->deleted != 0) return TEST_FAILED;

	ht_delete_container(tmp_1);

	return TEST_SUCCESS;
}

int test_rehash_resize() {

	int ret;

	hash_table* tmp = ht_create_container(hash_pol);

	for (int i = 0; i < 5; i++) ht_insert(tmp, i, i * i);
	for (int i = 0; i < 5; i++) ht_erase(tmp, i);

	ht_insert(tmp, 5, 25);
	ht_insert(tmp, 6, 36);
	ht_insert(tmp, 7, 49);

	if (check_elem(tmp, 2, 6, 36, 0) == TEST_FAILED) return TEST_FAILED;
	if (check_elem(tmp, 5, 5, 25, 0) == TEST_FAILED) return TEST_FAILED;
	if (check_elem(tmp, 9, 7, 49, 0) == TEST_FAILED) return TEST_FAILED;

	for (int i = 0; i < 10; i++) 
		if (i != 2 && i != 5 && i != 9 && tmp->vector[i] != NULL) 
			return TEST_FAILED;

	ht_delete_container(tmp);
//////////////////////////////////////////
	tmp = ht_create_container(hash_pol);

	for (int i = 0; i < 5; i++) ht_insert(tmp, i, i * i);
	for (int i = 0; i < 5; i++) ht_erase(tmp, i);

	ht_insert(tmp, 5, 25);
	ht_insert(tmp, 6, 36);

	alloc_mem_cal = 25;
	ret = ht_insert(tmp, 7, 49);
	if (ret != 2) return TEST_FAILED;

	if (check_elem(tmp, 0, 0, 0, 1) == TEST_FAILED) return TEST_FAILED;
	if (check_elem(tmp, 1, 3, 9, 1) == TEST_FAILED) return TEST_FAILED;
	if (check_elem(tmp, 2, 6, 36, 0) == TEST_FAILED) return TEST_FAILED;
	if (check_elem(tmp, 4, 2, 4, 1) == TEST_FAILED) return TEST_FAILED;
	if (check_elem(tmp, 5, 5, 25, 0) == TEST_FAILED) return TEST_FAILED;
	if (check_elem(tmp, 7, 1, 1, 1) == TEST_FAILED) return TEST_FAILED;
	if (check_elem(tmp, 8, 4, 16, 1) == TEST_FAILED) return TEST_FAILED;
	if (tmp->vector[3] != NULL) return TEST_FAILED;
	if (tmp->vector[6] != NULL) return TEST_FAILED;
	if (tmp->vector[9] != NULL) return TEST_FAILED;

	ht_delete_container(tmp);
//////////////////////////////
	tmp = ht_create_container(hash_pol);

	for (int i = 0; i < 8; i++) ht_insert(tmp, i, i * i);
	
	if (check_elem(tmp, 0, 0, 0, 0) == TEST_FAILED) return TEST_FAILED;
	if (check_elem(tmp, 2, 6, 36, 0) == TEST_FAILED) return TEST_FAILED;
	if (check_elem(tmp, 4, 2, 4, 0) == TEST_FAILED) return TEST_FAILED;
	if (check_elem(tmp, 5, 5, 25, 0) == TEST_FAILED) return TEST_FAILED;
	if (check_elem(tmp, 7, 1, 1, 0) == TEST_FAILED) return TEST_FAILED;
	if (check_elem(tmp, 8, 4, 16, 0) == TEST_FAILED) return TEST_FAILED;
	if (check_elem(tmp, 19, 7, 49, 0) == TEST_FAILED) return TEST_FAILED;

	for (int i = 0; i < 20; i++) 
		if (i != 0 && i != 2 && i != 4
	 	 && i != 5 && i != 7 && i != 8
	 	 && i != 19 && tmp->vector[i] != NULL) 
			return TEST_FAILED;

	ht_delete_container(tmp);
/////////////////////////////////////////

	tmp = ht_create_container(hash_pol);

	for (int i = 0; i < 7; i++) ht_insert(tmp, i, i * i);

	alloc_mem_cal = 25;
	ret = ht_insert(tmp, 7, 49);
	if (ret != 2) return TEST_FAILED;

	if (check_elem(tmp, 0, 0, 0, 0) == TEST_FAILED) return TEST_FAILED;
	if (check_elem(tmp, 1, 3, 9, 0) == TEST_FAILED) return TEST_FAILED;
	if (check_elem(tmp, 2, 6, 36, 0) == TEST_FAILED) return TEST_FAILED;
	if (check_elem(tmp, 4, 2, 4, 0) == TEST_FAILED) return TEST_FAILED;
	if (check_elem(tmp, 5, 5, 25, 0) == TEST_FAILED) return TEST_FAILED;
	if (check_elem(tmp, 7, 1, 1, 0) == TEST_FAILED) return TEST_FAILED;
	if (check_elem(tmp, 8, 4, 16, 0) == TEST_FAILED) return TEST_FAILED;
	if (tmp->vector[3] != NULL) return TEST_FAILED;
	if (tmp->vector[6] != NULL) return TEST_FAILED;
	if (tmp->vector[9] != NULL) return TEST_FAILED;

	ht_delete_container(tmp);

	return TEST_SUCCESS;
}

int test_foreach() {

	hash_table* tmp = ht_create_container(hash_pol);
	ht_foreach(tmp, print_elems, NULL);

	ht_insert(tmp, 1, 1);
	ht_foreach(tmp, print_elems, NULL);

	return TEST_SUCCESS;
}