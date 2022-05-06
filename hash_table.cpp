#include "hash_table.h"

struct hash_table *hash_table_create (uint hash_func (const char *key), size_t allocated)
{
    struct hash_table *hash_table = (struct hash_table *)calloc(1, sizeof(struct hash_table));
    
    hash_table->lists = (struct lists *)calloc(allocated, sizeof(struct lists));
    
    hash_table->hash_func = hash_func;

    hash_table->allocated = allocated;

    return hash_table;
}

void hash_table_insert (struct hash_table *hash_table, const char *key)
{
    uint hash = hash_table->hash_func(key) % hash_table->allocated;

    struct lists *lists = hash_table->lists + hash;

    if (lists->count == 0)
    {
        lists->head = list_new(key); 

        lists->count = 1;

        hash_table->count ++;
    } 

    else 
    {
        lists->head = list_insert(lists->head, key);

        lists->count ++;
    
        hash_table->count ++;
    }
}

void hash_table_erase (struct hash_table *hash_table, const char *key)
{
    uint hash = hash_table->hash_func(key) % hash_table->allocated;

    struct lists *lists = &hash_table->lists[hash];

    struct list *list = lists->head;

    int res = 0;
    
    lists->head = list_erase(list, key, &res);
    
    lists->count -= res;
}

struct list *hash_table_find (struct hash_table *hash_table, const char *key)
{
    uint hash    = hash_table->hash_func(key) % hash_table->allocated;

    struct lists *lists = &hash_table->lists[hash];
    
    struct list *res_list = list_find(lists->head, key);

    return res_list;
}

struct lists *hash_table_get_lists(struct hash_table *hash_table, const char *key)
{
    uint hash = hash_table->hash_func(key) % hash_table->allocated;

    return &hash_table->lists[hash];
}

void hash_table_destroy(struct hash_table *hash_table)
{
    if (hash_table == NULL)
    {
        return ;
    }

    for (int i = 0; i < hash_table->allocated; i++)
    {
        list_delete(hash_table->lists[i].head);
    }

    free(hash_table->lists);
    free(hash_table);
}