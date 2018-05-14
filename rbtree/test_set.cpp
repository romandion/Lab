
#include "test.h"

int test_set_insert()
{
    std::set<int> nodes ;

    DWORD start_tick = ::GetTickCount() ;

    for(int idx = 0 ; idx < kMaxSize ; ++idx)
    {
        nodes.insert(idx) ;
    }

    DWORD end_tick = ::GetTickCount() ;

    int elapse = (int)(end_tick - start_tick) ;
    if(elapse <= 0)
        elapse = 0 ;
    return elapse ;
}

int test_set_erase()
{
    return 0 ;
}

int test_set_find()
{
    return 0 ;
}
