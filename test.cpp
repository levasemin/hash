#include "hash_table.h"

extern __inline__ uint64_t rdtsc() {
   uint64_t x;
  __asm__ volatile ("rdtsc\n\tshl $32, %%rdx\n\tor %%rdx, %%rax" : "=a" (x) : : "rdx");
   return x;
 }

void plot_functions(const char *path, struct buffer *buffer, uint (**hash_functions)(const char *key), char **titles, size_t n)
{
    //FILE *gnuplotPipe = multiplot("Hash functions", 3, 3);
    
    size_t len = strlen(path);

    for (int i = 0; i < COUNT_HASH_FUNCS; i++)
    {
        struct hash_table *hash_table = hash_table_create(hash_functions[i],  ALLOCATED);
            
        fill_hash_table(hash_table, buffer);

        char *local_path = (char *)calloc(MAX_LEN, sizeof(char));

        memcpy_intr(local_path, path, len);

        sprintf(local_path, "%s", titles[i]);

        FILE *gnuplotPipe = popen("gnuplot -persistent", "w");

        create_graph(gnuplotPipe, hash_table, PERCENT_OUTLIER, titles[i], path);
        
        hash_table_destroy(hash_table);

        free(local_path);
    }
}


void run_test(hash_table *hash_table, struct buffer *buffer, int epoch)
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

    printf("Time : %lu\n", rdtsc() - i);

    return;
}