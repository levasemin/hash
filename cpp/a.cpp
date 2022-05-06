#include <stdio.h>
#include <cmath>

int check(uint number)
{
    uint sq = sqrt(1000000);

    for (uint i = 2; i < sq; i ++)
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
    for (uint i = 1000000; i > 0;i++){
    if (check(i))
    {
        printf("%lld!", i);
        
        return 1;
    }}
}
