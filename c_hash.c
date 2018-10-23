#include "c_hash.h"

static unsigned long hash(hkey_t key, unsigned long size)
{
    return key % size;
} 

hash_table_t* create_table(size_t min_size)
{
    if (!min_size) min_size = 128; 

    hash_table_t *table = (hash_table_t*)malloc(sizeof(hash_table_t));
    table->nodes = (node_t*)malloc(sizeof(node_t) * min_size);
    for (size_t i = 0; i < min_size; ++i) {
        table->nodes[i].data = NULL;
        table->nodes[i].next = NULL;
    }
    table->min_size = min_size;
    table->size = min_size;
    table->filled_buckets = 0;
    
    return table;
}

void delete_table(hash_table_t *table)
{
    for (hkey_t i = 0; i < table->size; ++i) {
        node_t *node = &table->nodes[i];
        while (node) {
            if (node->data) free(node->data);
            node = node->next;
        }
    }

    free(table->nodes);
    free(table);
}

static void resize_table(hash_table_t *table, size_t new_size)
{
    node_t *old_nodes = table->nodes;
    size_t old_size = table->size;
    
    table->nodes = (node_t*)malloc(sizeof(node_t) * new_size);
    for (size_t i = 0; i < new_size; ++i) {
        table->nodes[i].data = NULL;
        table->nodes[i].next = NULL;
    }
    table->size = new_size;
    table->filled_buckets = 0;

    for (hkey_t i = 0; i < old_size; ++i) {
        node_t *node = old_nodes[i].next;
        while (node) {
            insert_element(table, node->key, node->data, node->data_size);
            node_t *next = node->next;
            free(node->data);
            free(node);
            node = next;
        }
    }

    free(old_nodes);
}

void delete_element(hash_table_t *table, hkey_t key)
{
    unsigned long hkey = hash(key, table->size);
    node_t *node = table->nodes[hkey].next;
    node_t *prev_node = &table->nodes[hkey];

    // find the element with the same key
    while (node && node->key != key) {
        prev_node = node;
        node = node->next;
    }

    // delete if found
    if (node) {
        prev_node->next = node->next;
        free(node->data);
        free(node);

        if (prev_node == &table->nodes[hkey] && node->next == NULL) {
            --table->filled_buckets;
        }
    }

    if (table->filled_buckets < table->size / 4 && table->size / 2 >= table->min_size) {
        resize_table(table, table->size / 2);
    }
}

void insert_element(hash_table_t *table, hkey_t key, void *data, size_t data_size)
{
    // make a copy of the data
    void *copied_data = malloc(data_size);
    memcpy(copied_data, data, data_size);
    hkey_t hkey = hash(key, table->size);

    node_t *node_with_same_key = get_node(table, key);

    if (node_with_same_key) {
        node_with_same_key->data = copied_data;
        node_with_same_key->data_size = data_size;
    } else {
        // find the last element
        node_t *last = &table->nodes[hkey];
        while (last->next) {
            last = last->next;
        }

        // create the new element
        node_t *current = (node_t*)malloc(sizeof(node_t));
        current->key = key;
        current->data = copied_data;
        current->data_size = data_size;
        current->next = NULL;

        // update the list and the table
        last->next = current;
        if (last == &table->nodes[hkey]) {
            ++table->filled_buckets;
        }

        if (table->filled_buckets == table->size) {
            resize_table(table, table->size * 2);
        }
    }
}

node_t *get_node(hash_table_t *table, hkey_t key)
{
    unsigned long hkey = hash(key, table->size);
    node_t *node = table->nodes[hkey].next;
    while (node && node->key != key) {
        node = node->next;
    }

    return node;
}

void *get_element(hash_table_t *table, hkey_t key)
{
    node_t *node = get_node(table, key);

    return node ? node->data : NULL;
}
