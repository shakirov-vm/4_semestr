
#define MINIMAL_TABLE 10

#define KEY char*
#define VALUE char*

#define hash_func size_t(*)(KEY)

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

struct table_element {

	KEY key;
	VALUE value;
	int deleted; // 0 is not deleted, 1 is deleted
};

table_element* create_table_element(KEY key_, VALUE value_) {

	table_element* ret = (table_element*) malloc (sizeof(key_) + sizeof(value_)); //??

	ret->key = key_;
	ret->value = value_;
	ret->deleted = 0;

	return ret;
}

struct hash_table {

	table_element* vector;
	size_t size;
	size_t size_with_del;
	size_t capacity;
	hash_func count_hash;
};

hash_table* create_hash_table(hash_func hash) {

	hash_table* ret = (hash_table*) calloc (sizeof(hash_table), 1);

	ret->count_hash = hash;
	ret->capacity = MINIMAL_TABLE;
	ret->size = 0;
	ret->vector = (table_element*) calloc (capacity, 1);
}

void delete_hash_table(hash_table* table) {
	free(table->vector);
	free(table);
}

int hash_table_is_empty(hash_table* table) {

	if (size == 0) return 1; // 1 - is empty - true
	return 0; // 0 - not empty - false
}

void insert(hash_table* table, KEY key_, VALUE value_) { // ??????????????????????????????????

	if (table->size * 2 >= table->capacity) resize();
    else if (table->size_with_del > 2 * table->size) rehash();
    
    size_t iter = table->hash(table, value_, table->capacity); // ??

    int i = 0;
    int first_deleted = -1;
    
    while (table->vector[iter] != nullptr && i < table->capacity) {

        if (table->vector[iter]->value == value_ && !table->vector[iter]->deleted) return false;

        if (table->vector[iter]->deleted && first_deleted == -1) first_deleted = iter;
        ++iter;
        ++i;
    }
    if (first_deleted == -1) {

        table->vector[iter] = create_table_element(key_, value_);
        ++table->size_with_del;
    }
    else {
        table->vector[first_deleted]->value = value_;
        table->vector[first_deleted]->deleted = 0;
    }
    ++table->size;
    return;
}

table_element* find(KEY key_) { // ?????????????????????????????????????????
	//std::cout << "\n\nkey - " << key << std::endl;
	class pair<KEY, VALUE> node(key);
	size_t index = (*count_hash)(key) % capacity;
	Node<pair<KEY, VALUE>>* iterator = table[index].find_node(node);
	if (iterator == nullptr) return 0;
	return iterator->data.value;
}

void erase(KEY key_) { // we can return iterator // ??????????????????

	class pair<KEY, VALUE> node(key);
	size_t index = (*count_hash)(key) % capacity;
	table[index].delete_node(node);
}

void rehash(hash_table* table) {

	table->capacity *= 2;
	table->size = 0;

	table_element* tmp_vector = (table_element*) calloc (table->capacity, 1);

	table_element tmp_data; // no initile
	size_t index = 0;

	for (size_t i = 0; i < table->capacity / 2; i++) { // capacity divide by 2

		if (table[i] is valid) tmp_data = table[i];						// ????????????
	
		index = (*table->count_hash)(tmp_data.key) % table->capacity;

		if (tmp_table[index].is_empty()) size++;
		tmp_table[index].push_back(tmp_data);
	
	}
	delete[] table;
	table = tmp_table;
}

