
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "Test.h"

void * atomic_load_ptr(void * volatile * target)
{
    return InterlockedCompareExchangePointer(target , NULL , NULL) ;
}

void * atomic_store_ptr(void * volatile * target , void * value)
{
    return InterlockedExchangePointer(target , value) ;
}


typedef struct {unsigned char v ;} u8_t ;

int main(int argc , char * argv[])
{
    //for(int i = 0 ; i < kTestCaseSize ; ++i)
    //{
    //    test_case test = __test_cases__[i] ;
    //    test() ;
    //}

    //::GetTickCount() ;

    ::printf("size of bt = %u \n" , sizeof(u8_t)) ;

    return 0 ;
}

