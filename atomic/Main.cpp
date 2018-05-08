
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "Test.h"

int main(int argc , char * argv[])
{
    for(int i = 0 ; i < kTestCaseSize ; ++i)
    {
        test_case test = __test_cases__[i] ;
        test() ;
    }

    ::GetTickCount() ;

    return 0 ;
}

