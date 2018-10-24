#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_SIZE 128

typedef unsigned long hkey_t;

typedef struct node {
    hkey_t key;
    void *data;
    size_t data_size;
    struct node* next;
} node_t;

typedef struct {
    node_t *nodes; 
    size_t size;
    size_t filled_buckets;
} hash_table_t;

void insert_element(hash_table_t *table, hkey_t key, void *data, size_t data_size);
node_t *get_node(hash_table_t *table, hkey_t key);
hash_table_t* create_table(size_t size);
void delete_table(hash_table_t *table);
void delete_element(hash_table_t *table, hkey_t key);
void insert_element(hash_table_t *table, hkey_t key, void *data, size_t data_size);
void *get_element(hash_table_t *table, hkey_t key);
