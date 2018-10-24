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
    const int nr_elem = 25000;

    // insert element
    time_t t;
    srand((unsigned) time(&t)); 
    int keys[nr_elem];
    for (int i = 0; i < nr_elem; ++i) {
        //keys[i] = rand();
        keys[i] = i;
        insert_element(table, keys[i], "value", 7);
    }

    // retrieve element
    printf("\n");
    char *data = (char*)get_element(table, keys[0]);
    if (data) printf("element with key: %d is %s\n", 1, data);
    printf("\n");
    
    // delete element 
    for (int i = 0; i < nr_elem; ++i) {
        delete_element(table, keys[i]);
    }

    // delete table
    delete_table(table);
}
