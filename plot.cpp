#include "hash_table.h"

void plot(const double *xvals, const double *yvals, int n, char *title, char *path, char *special_com)
{
    char plot_command[LEN_PATH] = "plot \'";
    
    strcat(plot_command, path);
    strcat(plot_command, "\'");    
    strcat(plot_command, special_com);
    
    char title_command[LEN_PATH] = "set title ";

    strcat(title_command, "\"");
    strcat(title_command, title);
    strcat(title_command, "\"");

    char * commandsForGnuplot[] = {"set title \"Graphic\"", "set terminal wxt size 1920,1080", title_command, plot_command};

    FILE * temp = fopen(path, "w");
    FILE * gnuplotPipe = popen ("gnuplot -persistent", "w");
        
    for (int i=0; i < n; i++)
    {
        fprintf(temp, "%lf %lf \n", xvals[i], yvals[i]); //Write the data to a temporary file
    }

    for (int i=0; i < 4; i++)
    {
        fprintf(gnuplotPipe, "%s \n", commandsForGnuplot[i]); //Send commands to gnuplot one by one.
    }

}