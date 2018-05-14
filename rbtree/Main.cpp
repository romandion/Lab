
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rbtree.h"
#include "test.h"

int main(int argc , char * argv[])
{
    rb_root_t root = {NULL} ;
    int elapse = ::test_insert(&root) ;

    //int elapse = ::test_set_insert();
    ::printf("elapse = %d \n" , elapse) ;
    return 0 ;
}
