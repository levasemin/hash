#include "hash_table.h"


void create_graph(FILE *gnuplotPipe, hash_table *hash_table, const char *title, const char *path, int x_size, int y_size)
{
    double *array_y = (double *)calloc(hash_table->allocated, sizeof(double));
        
    int count = 0;
    
    int res_count = 0;

    for (int i = 0; i < hash_table->allocated; i++)
    {
        struct lists lists = hash_table->lists[i];
            
        array_y[count] = lists.count;
    
        count++;
    }
    
    plot(gnuplotPipe, array_y, x_size, y_size, count, title, path, "");//"set boxwidth 0.9 relative\n set style data histograms\n set style fill solid 1.0 border -1");
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

void plot(FILE *gnuplotPipe, const double *yvals, int x_size, int y_size, int n, const char *title, const char *path, const char *special_com)
{
    char plot_command[LEN_PATH] = {};
    
    sprintf (plot_command, "plot \'%s\' %s using 1 linecolor \'black\'", path, special_com);
    
    char title_command[LEN_PATH] = "set title ";

    sprintf(title_command, "set title \"%s\"", title);

    char size_command[LEN_PATH] = {};
    sprintf(size_command, "set terminal wxt size %d,%d", x_size, y_size);

    
    const char * commandsForGnuplot[] = {"set style fill solid 1.0 border -1", "", "set style data histograms", size_command, title_command, plot_command};

    FILE * temp = fopen(path, "w");
    
    for (int i=0; i < n; i++)
    {
        fprintf(temp, "%lf \n", yvals[i]); //Write the data to a temporary file
    }

    for (int i=0; i < 6; i++)
    {
        fprintf(gnuplotPipe, "%s \n", commandsForGnuplot[i]); //Send commands to gnuplot one by one.
    }
}