
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rxtree.h"
#include <windows.h>

void print_performce(DWORD start_tick , DWORD end_tick , DWORD times)
{
    DWORD elapse = end_tick - start_tick ;
    double perf = times * 1000;
    if(elapse != 0)
        perf = perf / elapse ;

    ::printf("Performce = [%0.3f] elapse[%d]\n" , perf , (int)elapse) ;
}

int main(int argc , char * argv[])
{
    rx_tree_t tree ;
    uint8_t bits[3] = {10 , 11 , 11} ;

    rx_tree_init(&tree , bits , 3 , NULL) ;

    DWORD start_tick = ::GetTickCount() ;

    uint32_t max_number = 0x10000 ;
    for(uint32_t idx = 0 ; idx < max_number ; ++idx)
    {
        rx_tree_insert(&tree , idx , idx) ;
    }

    DWORD end_tick = ::GetTickCount() ;
    print_performce(start_tick , end_tick , max_number) ;

    int err_counter = 0 ;
    start_tick = end_tick ;
    for(uint32_t idx = 0 ; idx < max_number ; ++idx)
    {
        if(idx == 63929)
            ::printf("maybe error \n") ;
        rx_node_t * node = rx_tree_find(&tree , idx) ;
        if(node == NULL ||node->value != idx)
        {
            ::printf("can not find [%u] \n" , idx) ;
            break ;
        }
    }
    end_tick = ::GetTickCount() ;
    print_performce(start_tick , end_tick , max_number) ;
    if(err_counter != 0)
        ::printf("failed to find key , counter[%d] \n" , err_counter) ;

    //rx_tree_final(&tree) ;    
    return 0 ;
}

