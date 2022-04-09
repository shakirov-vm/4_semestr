#include <stdio.h>

#include "./HTLib/hashtable.h"

void print_elem(table_element* elem, void* data) {

	//printf("[%d;%d;%d] ", elem->key, elem->value, elem->deleted);
}

int dump_wrap(hash_table* table) {

	printf("DUMP:\n");
	foreach(table, print_elem, NULL);
	printf("\n");

	return 0;
}

int main() {

	hash_table* first = create_hash_table(hash_pol);
	hash_table_is_empty(first);

	int ret = 0;
	table_element* finded = NULL;

	for(int i = 0; i < 3000; i++) {
		ret = insert(first, i, i * i);
		//if (ret != 0) printf("insert error - %d\n", i); 
	}
	hash_table_is_empty(first);
	dump_wrap(first);
	for(int i = 0; i < 2000; i++) {
		finded = find(first, i);
		//if (finded == NULL) printf("find error - %d\n", i);
	}
	for(int i = 1000; i < 3000; i++) {
		ret = erase(first, i);
		//if (ret != 0) printf("erase error[%d] - %d\n", ret, i);
	}
	for(int i = 1000; i < 3000; i++) {
		ret = erase(first, i);
		//if (ret != 0) printf("erase error[%d] - %d\n", ret, i);
	}
	for(int i = 0; i < 1500; i++) {
		ret = insert(first, i, i * i);
		//if (ret != 0) printf("insert error - %d\n", i); 
	}
	//dump_wrap(first);
	for(int i = 0; i < 2000; i++) {
		finded = find(first, i);
		//if (finded == NULL) printf("find error - %d\n", i);
	}
	delete_hash_table(first);

	return 0;
}
