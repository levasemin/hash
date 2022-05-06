#include "hash_table.h"

int strcmp_intr(const char *str1, const char *str2)
{
    __m256 arr1 = _mm256_load_ps((float *)str1);
    __m256 arr2 = _mm256_load_ps((float *)str2);
    
    __m256 cmp_res = _mm256_cmp_ps(arr1, arr2, _CMP_NEQ_OQ);
    
    int res = _mm256_movemask_ps(cmp_res);

    return res;
}

void memcpy_intr(char *str1, const char *str2, int n)
{
    __m256 *str1_256 = (__m256 *)str1;
    __m256 *str2_256 = (__m256 *)str2;

    *str1_256 = *str2_256;
}
