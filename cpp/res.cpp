#include <immintrin.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void memset_intr(char *str1, char el, int n)
{
    size_t index = 0;
    
    __m256i zeros = _mm256_set1_epi8(el);

    for (int index = 0; index < n; index += 8)
    {        
        *(__m256i *)(str1 + index) = zeros;
    }

    index -= 8;

    for (; index < n; index ++)
    {
        str1[index] = el;  
    }
}

int main()
{
    __m256 zero = _mm256_setzero_ps();

    char *string = (char *)calloc(100000000000, sizeof(char));

    memset(string, 1, 100000000000);

    return 0;
}