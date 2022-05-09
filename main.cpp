#include "hash_table.h"

const char *book  = "/home/levce/projectsDED/hash/source/book.txt";
const char *eng_words = "/home/levce/projectsDED/hash/source/eng_words.txt"; 

uint (*hash_functions[])(const char *key) =  {
                                            hash_only_one,
                                            hash_first_word,
                                            hash_ascii_sum,
                                            hash_len_word,
                                            hash_rolling,
                                            hash_crc32
                                            };

char hash_functions_names[COUNT_HASH_FUNCS][MAX_LEN] = {
                                "HashOnlyOne",
                                "HashFirstWord",
                                "HashAsciiSum",
                                "HashLenWord",
                                "HashRolling",                                    
                                "HashCrc32"
                                };
                                
int main()
{
    char *string = read_file(book, "r");
    
    struct hash_table *hash_table = hash_table_create(hash_crc32_intr,  ALLOCATED);
    
    struct buffer *buffer = buffer_make(string, BUFFER_SIZE, MAX_LEN);

    fill_hash_table(hash_table, buffer);
    

    char *words = read_file(eng_words, "r");

    struct buffer *buffer_test = buffer_make(words, BUFFER_SIZE, MAX_LEN);

    //run_test(hash_table, buffer, 100);

    char **names_funcs = (char **)calloc(COUNT_HASH_FUNCS, sizeof(char *));

    for (int i = 0; i < COUNT_HASH_FUNCS; i++)
    {
        names_funcs[i] = (char *)(hash_functions_names + i);
    }

    test_functions(buffer, hash_functions, names_funcs, COUNT_HASH_FUNCS, buffer_test, 10, "graphes/", 1920, 1080);

    free(string);

    buffer_destroy(buffer);

    hash_table_destroy(hash_table);

    return 0;
}