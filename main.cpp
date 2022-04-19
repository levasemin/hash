#include "hash_table.h"

const char *book = "/home/levce/projectsDED/hash/book.txt";

int main()
{
    char *string = read_file(book, "rb");
    
    struct hash_table *hash_table = hash_table_create(hash_crc32,  ALLOCATED);
    
    fill_hash_table(hash_table, string);
    
    struct list *list = hash_table_find(hash_table, "the");
    
    hash_table_erase(hash_table, "was");
    
    double *array_y = (double *)calloc(ALLOCATED, sizeof(double));
    double *array_x = (double *)calloc(ALLOCATED, sizeof(double));
        
    int count = 0;

    for (int i = 0; i < ALLOCATED; i++)
    {
        struct lists lists = hash_table->lists[i];

        if (lists.count > 0)
        {
            array_y[count] = lists.count;
            array_x[count] = i;

            count++;
        }
    }
    
    //printf("!%d!\n", count);
    //plot(array_x, array_y, count, __func_name__(hash_crc32), "obj/data", "linetype 7 linecolor 0 with linespoints");
}