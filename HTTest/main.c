#include <stdio.h>

#include "./HTLib/hashtable.h"

void print_elem(table_element* elem, void* data) {

	//printf("[%d;%d;%d] ", elem->key, elem->value, elem->deleted);
}

int dump_wrap(hash_table* table) {

	printf("DUMP:\n");
	ht_foreach(table, print_elem, NULL);
	printf("\n");

	return 0;
}

int main() {

	int ret;
	
	hash_table* first = ht_create_container(hash_pol);
	ret = ht_insert(first, 1, 2);
	ret = ht_insert(first, 2, 4);

	dump_wrap(first);

	for(int i = 0; i < 100; i++) {
		printf("[%d;%d]\n", hash_pol((char*) &i) % 10, i);
		//if (ret != 0) printf("insert error - %d\n", i); 
	}
	

	/*for (int i = 0; i < 1001; i++) { // if alloc_mem every 25 <- there last is NULL
		hash_table* tmp = ht_create_container(hash_pol);
		if  (tmp != NULL) ht_delete_container(tmp);
	}
	// [start new circle]
	hash_table* first = ht_create_container(hash_pol);
	ht_is_empty(first);

	int ret = 0;
	table_element* finded = NULL;



	fr(int i = 0; i < 3000; i++) {
		ret = ht_insert(first, i, i * i);
		//if (ret != 0) printf("insert error - %d\n", i); 
	}
	ht_is_empty(first);
	dump_wrap(first);
	for(int i = 0; i < 2000; i++) {
		finded = ht_find(first, i);
		//if (finded == NULL) printf("find error - %d\n", i);
	}
	for(int i = 1000; i < 3000; i++) {
		ret = ht_erase(first, i);
		//if (ret != 0) printf("erase error[%d] - %d\n", ret, i);
	}
	for(int i = 1000; i < 3000; i++) {
		ret = ht_erase(first, i);
		//if (ret != 0) printf("erase error[%d] - %d\n", ret, i);
	}
	for(int i = 0; i < 1500; i++) {
		ret = ht_insert(first, i, i * i);
		//if (ret != 0) printf("insert error - %d\n", i); 
	}
	//dump_wrap(first);
	for(int i = 0; i < 2000; i++) {
		finded = ht_find(first, i);
		//if (finded == NULL) printf("find error - %d\n", i);
	}
	ht_delete_container(first);

	hash_table* second = ht_create_container(hash_pol);

	for(int j = 0; j < 100; j++) {
	
		for(int i = j * 4; i < j * 4 + 4; i++) {
			ret = ht_insert(second, i, i * i);
			//if (ret != 0) printf("insert error - %d\n", i); 
		}
		for(int i = j * 4; i < j * 4 + 4; i++) {
			ret = ht_erase(second, i);
			//if (ret != 0) printf("insert error - %d\n", i); 
		}
	}
	ht_is_empty(second);

	for (int i = 0; i < 5; i++) { // if alloc_mem every 25 <- there last is NULL
		hash_table* tmp = ht_create_container(hash_pol);
		if  (tmp != NULL) ht_delete_container(tmp);
	}
	printf("And last\n");
	for(int i = 0; i < 10000; i++) {
		ret = ht_insert(second, i, i);
		//if (ret != 0) printf("insert error - %d\n", i); 
	}
	ht_delete_container(second);

	return 0;*/
}
