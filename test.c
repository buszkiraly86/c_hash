#include "c_hash.h"
#include <time.h>

void print_table(hash_table_t *table)
{
    for (int i = 0; i < table->size; ++i) {
        printf("%d. ", i);
        node_t *node = table->nodes[i].next;
        while(node) {
            printf("%d-%s ", node->key, (char*)node->data); 
            node = node->next; 
        }
        printf("\n");
    }

    printf("\n");
    printf("size of the table: %d\n", table->size);
    printf("out of which is used: %d\n", table->filled_buckets);
}


int main()
{
    // create hash table
    hash_table_t *table = create_table(4);

    // insert element
    time_t t;
    srand((unsigned) time(&t)); 
    int keys[24000];
    for (int i = 0; i < 24000; ++i) {
        keys[i] = rand();
        insert_element(table, keys[i], "value", 7);
    }

    // retrieve element
    printf("\n");
    char *data = (char*)get_element(table, keys[0]);
    if (data) printf("element with key: %d is %s\n", 1, data);
    printf("\n");
    
    // delete element 
    for (int i = 0; i < 24000; ++i) {
        delete_element(table, keys[i]);
    }
    print_table(table);

    // delete table
    delete_table(table);
}
