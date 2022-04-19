#include <stdio.h>
#include <cmath>

int check(long long number)
{
    long long sq = sqrt(1000000);

    for (long long i = 2; i < sq; i ++)
    {
        if (number % i == 0)
        {
            return 0;
        }
    }
    
    return 1;
}

int main()
{
    for (long long i = 1000; i > 0;i++){
    if (check(i))
    {
        printf("%lld", i);
        
        return 1;
    }}
}
