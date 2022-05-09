#include "hash_table.h"

const char *book  = "/home/levce/projectsDED/hash/source/book.txt";
const char *eng_words = "/home/levce/projectsDED/hash/source/eng_words.txt"; 

uint (*hash_functions[])(const char *key) =  {
                                            hash_only_one,
                                            hash_first_word,
                                            hash_ascii_sum,
                                            hash_len_word,
                                            hash_rolling,
                                            hash_crc32,
                                            hash_ascii_sum_asm
                                            };

char hash_functions_names[][MAX_LEN] = {
                                "HashOnlyOne",
                                "HashFirstWord",
                                "HashAsciiSum",
                                "HashLenWord",
                                "HashRolling",                                    
                                "HashCrc32",
                                "HashAsciiSumAsm"
                                };
int main()
{
    char *string = read_file(book, "r");
        
    struct buffer *buffer = buffer_make(string, BUFFER_SIZE, MAX_LEN);
    
    char *words = read_file(eng_words, "r");

    struct buffer *buffer_test = buffer_make(words, BUFFER_SIZE, MAX_LEN);

    hash_functions[0] = hash_ascii_sum_asm;

    char **names_funcs = (char **)calloc(COUNT_HASH_FUNCS, sizeof(char *));

    for (int i = 0; i < COUNT_HASH_FUNCS + 1; i++)
    {
        names_funcs[i] = (char *)(hash_functions_names + i);
    }    

    test_functions(buffer, hash_functions + 2, names_funcs + 2, 1, buffer_test, 100);
    
    //test_functions(buffer, hash_functions, names_funcs, COUNT_HASH_FUNCS, buffer_test, 10, "graphes/", 1920, 1080);

    free(string);

    buffer_destroy(buffer);

    return 0;
}