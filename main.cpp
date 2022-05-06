#include "hash_table.h"

const char *book  = "/home/levce/projectsDED/hash/source/book.txt";
const char *eng_words = "/home/levce/projectsDED/hash/source/eng_words.txt"; 

int main()
{
    char *string = read_file(book, "r");
    
    struct hash_table *hash_table = hash_table_create(hash_crc32,  ALLOCATED);
    
    struct buffer *buffer = make_buffer(string, BUFFER_SIZE, MAX_LEN);
    
    fill_hash_table(hash_table, buffer);
    

    char *words = read_file(eng_words, "r");

    struct buffer *buffer_test = make_buffer(words, BUFFER_SIZE, MAX_LEN);

    run_test(hash_table, buffer, 2);

    
    return 0;
}