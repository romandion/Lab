
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rbtree.h"
#include "test.h"

int main(int argc , char * argv[])
{
    rb_root_t root = {NULL} ;
    test_node_t * nodes = build_test_nodes(kMaxSize) ;  

    int elapse = ::test_insert(&root , nodes , kMaxSize) ;
    ::printf("elapse = %d \n" , elapse) ;
    test_validate(&root) ;

    for(int idx = 0 ; idx < kMaxSize ; ++idx)
    {
        
    }


    return 0 ;
}
