
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <map>

#include "rbtree.h"
#include "test.h"

int main(int argc , char * argv[])
{
    rb_root_t root = {NULL} ;
    test_node_t * nodes = build_test_nodes(kMaxSize) ;  

    int elapse = ::test_insert(&root , nodes , kMaxSize) ;
    ::printf("insert elapse = %d \n" , elapse) ;
    test_validate(&root) ;

    //≤‚ ‘À—À˜
    DWORD start_tick = ::GetTickCount() ;
    for(int idx = 0 ; idx < kMaxSize ; ++idx)
    {
        int result = 0 ;
        if((result = test_find(&root , idx)) != 0)
            ::printf("find'result = %d \n" , result) ;
    }
    DWORD end_tick = ::GetTickCount() ;
    elapse = (int)(end_tick - start_tick) ;
    ::printf("find elapse = %d \n" , elapse) ;

    //≤‚ ‘…æ≥˝
    start_tick = ::GetTickCount() ;
    for(int idx = 0 ; idx < kMaxSize ; ++idx)
    {
        int result = test_erase(&root , idx) ;
        if(result != 0)
            ::printf("erase'result = %d \n" , result) ;

        if(test_find(&root , idx) != -1)
            ::printf("erase failed\n") ;
    }
    end_tick = ::GetTickCount() ;
    elapse = (int)(end_tick - start_tick) ;
    ::printf("erase elapse = %d \n" , elapse) ;

    test_validate(&root) ;    

    return 0 ;
}
