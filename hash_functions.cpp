#include "hash_table.h"

long long hash_stupid(const char *key)
{
    return 1;
}

long long hash_first_word(const char *key)
{
    return (long long)*key;
}

long long hash_sum_word(const char *key)
{
    long long hash = 0;

    for (; *key != '\0'; hash += *key, key++);
    
    return hash;
}

long long hash_len_word(const char *key)
{
    long long hash = 0;

    for (; *key != '\0'; hash++, key++);

    return hash;
}

long long hash_super_ded32(const char *key)
{
    long long hash = 0;

    return 0;
}

long long hash_crc32(const char *key)
{
    unsigned ret = 0xFFFFFFFF;
    
    while (*key != '\0')
    {
        ret ^= *key & 0xFF;
        ret = (ret >> 8) ^ crc32_table[ret & 0xFF];
        
        key++;
    }

    return ret ^ 0xFFFFFFFF;
}