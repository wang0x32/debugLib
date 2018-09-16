#include <stdio.h>
#include "debug.h"

int main()
{
    int i = 0;
    
    while(i < 100000)
    {
        i++;
        JL_printf(JL_LOG_ERROR, "test.test.test.test.test.test.test.test***%d\n", i);
    }
}
