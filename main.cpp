#include "hash_table.h"

const char *book  = "/home/levce/projectsDED/hash/source/book.txt";
const char *eng_words = "/home/levce/projectsDED/hash/source/eng_words.txt"; 

uint (*hash_functions[COUNT_HASH_FUNCS])(const char *key) =  {
                                            hash_only_one,
                                            hash_first_letter,
                                            hash_ascii_sum,
                                            hash_len_word,
                                            hash_rolling,
                                            hash_crc32,
                                            hash_crc32_intr,
                                            hash_ascii_sum_asm,
                                            hash_rolling_asm
                                            };

char hash_functions_names[COUNT_HASH_FUNCS][MAX_LEN] = {
                                "HashOnlyOne",
                                "HashFirstLetter",
                                "HashAsciiSum",
                                "HashLenWord",
                                "HashRolling",                                    
                                "HashCrc32",
                                "HashCrc32Intr",
                                "HashAsciiSumAsm",
                                "HashRollingAsm"
                                };
int main()
{
    char *string = read_file(book, "r");
        
    struct buffer *buffer = buffer_make(string, BUFFER_SIZE, MAX_LEN);
    
    char *words = read_file(eng_words, "r");

    struct buffer *buffer_test = buffer_make(words, BUFFER_SIZE, MAX_LEN);

    char **names_funcs = (char **)calloc(COUNT_HASH_FUNCS, sizeof(char *));

    for (int i = 0; i < COUNT_HASH_FUNCS; i++)
    {
        names_funcs[i] = (char *)(hash_functions_names + i);
    }    
    
    //test_functions(buffer, hash_functions, names_funcs, COUNT_HASH_FUNCS, buffer_test, 100);
    
    test_functions(buffer, hash_functions, names_funcs, COUNT_TYPE_HASH_FUNCS, buffer_test, 1, "graphes/", 1920, 1080);
    //hash_table *hash_table = hash_table_create(hash_crc32_intr, ALLOCATED);
    //fill_hash_table(hash_table, buffer);
    //run_test(hash_table, buffer_test, "HashCrc32Intr", 100);
    free(string);

    buffer_destroy(buffer);

    return 0;
}