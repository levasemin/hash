#include "hash_table.h"

size_t get_file_size(FILE *file)
{
    assert(file != nullptr);

    fseek(file, 0L, SEEK_END);

    int file_size = ftell(file);

    fseek(file, 0L, SEEK_SET);

    return file_size;
}

char *read_file(const char *file_name, const char *flag)
{
    FILE *file = fopen(file_name, flag);
    
    size_t file_size = get_file_size(file);

    char *string = (char *)calloc(file_size + 2, sizeof(char));

    fread(string, sizeof(char), file_size, file);

    string[file_size] = '\0';

    fclose(file);

    return string;
}

void fill_hash_table(struct hash_table *hash_table, char *string)
{
    size_t n = strlen((const char *)string);

    for (size_t i = 0; i < n; i++)
    {
        char word[MAX_LEN] = "";

        int count = 0;
        
        for (i; i < n && count < MAX_LEN; i++, count++)
        {
            char letter = tolower(string[i]);
            
            if (letter >= 97 && letter <= 122)
            {
                word[count] = letter;
            }

            else
            {
                break;
            }
        }
        
        word[count] = '\0';

        //printf("%f\n", ((float)i / 6293689 * 100));
        //fflush(stdout);
        
        if (strncmp(word, "wob", MAX_LEN) == 0)
        {
            printf("!%ld!\n", i);
            printf("YAH\n");
        }
        if (count > 2)
        {
            hash_table_insert(hash_table, word);
        }
    }

    return;
}