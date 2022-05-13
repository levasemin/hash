#include "hash_table.h"

extern __inline__ uint64_t rdtsc() {
   uint64_t x;
  __asm__ volatile ("rdtsc\n\tshl $32, %%rdx\n\tor %%rdx, %%rax" : "=a" (x) : : "rdx");
   return x;
 }

void test_functions(struct buffer *buffer, uint (**hash_functions)(const char *key), char **titles, int n, struct buffer *buffer_test, int epoch, const char *path, int x_size, int y_size)
{    
    size_t len = strlen(path);

    for (int i = 0; i < n; i++)
    {
        struct hash_table *hash_table = hash_table_create(hash_functions[i],  ALLOCATED);
            
        fill_hash_table(hash_table, buffer);

        char *local_path = (char *)calloc(MAX_LEN, sizeof(char));

        sprintf(local_path, "%s%s", path, titles[i]);

        FILE *gnuplotPipe = popen("gnuplot -persistent", "w");

        create_graph(gnuplotPipe, hash_table, titles[i], local_path, x_size, y_size);
        
        run_test(hash_table, buffer_test, titles[i], epoch);

        hash_table_destroy(hash_table);

        free(local_path);
    }
}

void test_functions(struct buffer *buffer, uint (**hash_functions)(const char *key), char **titles, int n, const char *path, int x_size, int y_size)
{    
    size_t len = strlen(path);

    for (int i = 0; i < n; i++)
    {
        struct hash_table *hash_table = hash_table_create(hash_functions[i],  ALLOCATED);
            
        fill_hash_table(hash_table, buffer);

        char *local_path = (char *)calloc(MAX_LEN, sizeof(char));

        sprintf(local_path, "%s%s", path, titles[i]);

        FILE *gnuplotPipe = popen("gnuplot -persistent", "w");

        create_graph(gnuplotPipe, hash_table, titles[i], local_path, x_size, y_size);
        
        hash_table_destroy(hash_table);

        free(local_path);
    }
}

void test_functions(struct buffer *buffer, uint (**hash_functions)(const char *key),  char **titles, int n, struct buffer *buffer_test, int epoch)
{
    for (int i = 0; i < n; i++)
    {
        struct hash_table *hash_table = hash_table_create(hash_functions[i],  ALLOCATED);
                
        fill_hash_table(hash_table, buffer);

        run_test(hash_table, buffer_test, titles[i], epoch);
    }
    
    return;
}

void run_test(hash_table *hash_table, struct buffer *buffer, char *title, int epoch)
{
    size_t i = 0;

    i = rdtsc();
    
    for (int ep = 0; ep < epoch; ep++)
    {
        for (int i = 0; i < buffer->count; i++)
        {
            hash_table_find(hash_table, buffer->buffer[i]);
        }
    }

    printf("Hash function: %s\nTime : %3lu\n\n", title, rdtsc() - i);

    return;
}
//142949718864
//129523917012