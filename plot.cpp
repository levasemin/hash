#include "hash_table.h"


void create_graph(FILE *gnuplotPipe, hash_table *hash_table, double percent_outlier, const char *title, const char *path, int x_size, int y_size)
{
    double *array_y = (double *)calloc(hash_table->allocated, sizeof(double));
    double *array_x = (double *)calloc(hash_table->allocated, sizeof(double));
        
    int count = 0;

    for (int i = 0; i < hash_table->allocated; i++)
    {
        struct lists lists = hash_table->lists[i];

        if (lists.count > 0)
        {
            count++;
        }
    }
    
    struct heap *heap = heap_create(count * percent_outlier + 1, 0);
    
    for (int i = 0; i < hash_table->allocated; i++)
    {
        struct lists lists = hash_table->lists[i];

        if (heap->nodes < heap->allocated)
        {
            heap_add(heap, &(lists.count));
        }

        if (lists.count > heap->body[0])
        {
            heap_head_delete(heap);
            heap_add(heap, &(lists.count));
        }
    }

    size_t max_value = heap->body[0];

    heap_delete(heap);

    count = 0;
    
    for (int i = 0; i < hash_table->allocated; i++)
    {
        struct lists lists = hash_table->lists[i];
            
        size_t list_count = lists.count > max_value ? max_value : lists.count;

        if (list_count > 0)
        {
            array_y[count] = list_count;
            array_x[count] = i;

            count++;
        }
    }
    
    plot(gnuplotPipe, array_x, array_y, x_size, y_size, count, title, path, "linetype 7 linecolor 0 with linespoints");
}

FILE * multiplot(const char *title, int x, int y)
{
    FILE *gnuplotPipe = popen ("gnuplot -persistent", "w");

    fprintf(gnuplotPipe,   "set multiplot \
                            title \"%s\" \
                            layout %d, %d \
                            \n", title, x, y);

    return gnuplotPipe;
}

void plot(FILE *gnuplotPipe, const double *xvals, const double *yvals, int x_size, int y_size, int n, const char *title, const char *path, const char *special_com)
{
    char plot_command[LEN_PATH] = {};
    
    sprintf (plot_command, "plot \'%s\' %s", path, special_com);
    
    char title_command[LEN_PATH] = "set title ";

    sprintf(title_command, "set title \"%s\"", title);

    char size_command[LEN_PATH] = {};
    sprintf(size_command, "set terminal wxt size %d,%d", x_size, y_size);

    const char * commandsForGnuplot[] = {size_command, title_command, plot_command};

    FILE * temp = fopen(path, "w");
    
    for (int i=0; i < n; i++)
    {
        fprintf(temp, "%lf %lf \n", xvals[i], yvals[i]); //Write the data to a temporary file
    }

    for (int i=0; i < 3; i++)
    {
        fprintf(gnuplotPipe, "%s \n", commandsForGnuplot[i]); //Send commands to gnuplot one by one.
    }
}