#include "hash_table.h"

inline void memcpy_intr(char *str1, const char *str2, int n)
{
    __m256 *str1_256 = (__m256 *)str1;
    __m256 *str2_256 = (__m256 *)str2;

    *str1_256 = *str2_256;
}

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


struct buffer *buffer_make(char *string, size_t buffer_size, size_t max_len)
{
    struct buffer *buffer = (struct buffer *)calloc(1, sizeof(struct buffer));

    char *text = (char *)aligned_alloc(32, buffer_size * max_len);

    memset(text, 0, buffer_size * max_len);
    
    buffer->buffer = (char **)calloc(buffer_size, sizeof(char *));
    
    buffer->max_len = max_len;

    for (size_t i = 0; string[i] && buffer->count < buffer_size; i++)
    {
        int index = 0;
        
        buffer->buffer[buffer->count] = text + buffer->count * MAX_LEN;

        for (; string[i] && index < MAX_LEN; i++, index++)
        {            
            if (isalpha(string[i]))
            {
                buffer->buffer[buffer->count][index] = string[i];
            }

            else
            {
                break;
            }
        }

        buffer->buffer[buffer->count][index] = '\0';
        
        if (index > 0)
        {
            buffer->count ++;
        }
    }

    return buffer;
}

void buffer_destroy(struct buffer *buffer)
{
    free(buffer->buffer[0]);
    free(buffer);
}

void fill_hash_table(struct hash_table *hash_table, struct buffer *buffer)
{
    for (size_t i = 0; i < buffer->count; i++)
    {
        hash_table_insert(hash_table, buffer->buffer[i]);
    }

    return;
}