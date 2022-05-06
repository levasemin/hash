#include "hash_table.h"

uint hash_stupid(const char *key)
{
    return 1;
}

uint hash_first_word(const char *key)
{
    return (uint)*key;
}

uint hash_ascii_sum(const char *key)
{
    uint hash = 0;

    for (; *key != '\0'; hash += *key, key++);
    
    return hash;
}

uint hash_len_word(const char *key)
{
    return strlen(key);
}

uint hash_super_ded(const char *key)
{
    uint hash = key[0];
    
    for(int i = 1; key[i] != '\0'; i++)
    {
        hash = ((hash >> 1) | (hash << 31)) ^ key[i];
    } 

    return hash;
}

uint hash_super_ded_asm(const char *key)
{
    uint hash = 0;

    __asm__  __volatile__ (
        ".intel_syntax noprefix ;" \
        "xor eax, eax ;"           \
        "xor edx, edx ;"           \
        "mov al, [%1];"            \
        "next_symbol%=: "          \

        "inc %1 ;"                 \
        "mov dl, [%1] ;"           \
        "cmp dl, 0 ;"              \
        "je out%= ;"               \
        "ror eax, 1 ;"             \
        "xor eax, edx ;"           \

        "jmp next_symbol%= ;"      \
        "out%=: "                  \
        "mov %0, eax ;"
        ".att_syntax prefix ;"     \
        :"=r"(hash)                \
        :"r"(key)                  \
        :"eax", "edx", "dl");

    return hash;
}

uint hash_crc32_intr(const char *key)
{    
    long long *array = (long long *)key;

    uint res = 0;

    for (int i = 0; i < 4; i++)
    {
        if (array[i] == 0)
        {
            break;
        }
        
        res = _mm_crc32_u64(res, array[i]);
    }

    return res;
}

uint hash_crc32(const char *key)
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