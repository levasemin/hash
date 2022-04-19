#include "hash_table.h"


struct hash_table *hash_table_create (long long hash_func (const char *key), size_t allocated)
{
    struct hash_table *hash_table = (struct hash_table *) calloc(1, sizeof(struct hash_table));
    
    hash_table->lists = (struct lists *)calloc(allocated, sizeof(struct lists));
    
    hash_table->hash_func = hash_func;

    hash_table->allocated = allocated;

    return hash_table;
}

void hash_table_insert (struct hash_table *hash_table, const char *key)
{
    long long hash = hash_table->hash_func(key) % hash_table->allocated;

    struct lists *lists = &hash_table->lists[hash];

    if (lists->count == 0)
    {
        lists->head = (struct list *)calloc(1, sizeof(struct list));
        
        memcpy(lists->head->elem, key, MAX_LEN);

        lists->count = 1;

        hash_table->count ++;
    } 

    else 
    {
        struct list *current_list = lists->head;

        while (current_list->next_list != NULL && (strcmp(current_list->elem, key) != 0))
        {
            current_list = current_list->next_list;
        }
        
        if (strcmp(current_list->elem, key) == 0)
        {
            return;
        }

        struct list *new_list = list_new(key);
        
        list_insert_after(lists->head, current_list, new_list);

        lists->count ++;

        hash_table->count ++;
    }
}

void hash_table_erase (struct hash_table *hash_table, const char *key)
{
    long long hash    = hash_table->hash_func(key) % hash_table->allocated;

    struct lists *lists = &hash_table->lists[hash];

    struct list *list = lists->head;

    int res = 0;
    
    lists->head = list_erase(list, key, &res);
    
    lists->count -= res;
}

struct list *hash_table_find (struct hash_table *hash_table, const char *key)
{
    long long hash    = hash_table->hash_func(key) % hash_table->allocated;

    struct lists *lists = &hash_table->lists[hash];

    struct list *res_list = list_find(lists->head, key);

    return res_list;
}

struct lists *hash_table_get_lists(struct hash_table *hash_table, const char *key)
{
    long long hash = hash_table->hash_func(key) % hash_table->allocated;

    return &hash_table->lists[hash];
}

void hash_table_destroy(struct hash_table *hash_table)
{
    for (size_t i = 0; i < hash_table->allocated; i++)
    {
        list_delete(hash_table[i].lists->head);
    }

    free(hash_table->lists);
    free(hash_table);
}