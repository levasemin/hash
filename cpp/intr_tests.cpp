#include <immintrin.h>
#include <stdio.h>
#include <ctime>
#include <string.h>

int strcmp_intr(const char *str1, const char *str2)
{
    __m256 arr1 = _mm256_load_ps((float *)str1);
    __m256 arr2 = _mm256_load_ps((float *)str2);
    
    __m256 cmp_res = _mm256_cmp_ps(arr1, arr2, _CMP_NEQ_OQ);
    
    int res = _mm256_movemask_ps(cmp_res);

    return res;
}

int main()
{
    char *str1 = (char *)aligned_alloc(32, 32);
    char *str2 = (char *)aligned_alloc(32, 32);

    memcpy(str1, "1234567890123456123456789012345", 32);
    memcpy(str2, "9999999999999999999999999999999", 32);

    for (int i = 0; i < 1000000000; i++)
    {
        strcmp_intr(str1, str2);
    }
}