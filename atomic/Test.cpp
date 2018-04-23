
#include "Test.h"

bool test_compare_exchange()
{
    volatile LONG counter = 0 ;
    int value = ::InterlockedCompareExchange(&counter , 1 , 0) ;
    ::printf("InterlockedCompareExchange , return value[%d] current value[%d] \n" , value , counter) ;
    return true ;
}

bool test_increment()
{
    volatile LONG counter = 0 ;
    int value = ::InterlockedIncrement(&counter) ;
    ::printf("InterlockedIncrement , return value[%d] current value[%d] \n" , value , counter) ;
    return true ;
}

bool test_decrement()
{
    volatile LONG counter = 0 ;
    int value = ::InterlockedDecrement(&counter) ;
    ::printf("InterlockedDecrement , return value[%d] current value[%d] \n" , value , counter) ;
    return true ;
}
