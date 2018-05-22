
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
    mem_mgr mems ;
    uint8_t bits[3] = {10 , 11 , 11} ;

    rx_tree_init(&tree , bits , 3 , NULL) ;
    tree.mems = &mems ;

    tick_counter ticks ;

    ticks.start() ;
    uint32_t max_number = 0x1000000 ;
    for(uint32_t idx = 0 ; idx < max_number ; ++idx)
    {
        rx_tree_insert(&tree , idx , idx) ;
    }
    ticks.stop() ;
    print_performce(ticks.start_tick() , ticks.end_tick() , max_number) ;

    int err_counter = 0 ;
    ticks.start() ;
    for(uint32_t idx = 0 ; idx < max_number ; ++idx)
    {
        //if(idx == 63929)
        //    ::printf("maybe error \n") ;
        rx_node_t * node = rx_tree_find(&tree , idx) ;
        if(node == NULL ||node->value != idx)
        {
            ::printf("can not find [%u] \n" , idx) ;
            ++err_counter ;
            break ;
        }
    }
    ticks.stop() ;
    print_performce(ticks.start_tick() , ticks.end_tick() , max_number) ;
    if(err_counter != 0)
        ::printf("failed to find key , counter[%d] \n" , err_counter) ;

    err_counter = 0 ;
    ticks.start() ;
    for(uint32_t idx = 0 ; idx < max_number ; ++idx)
    {
        if(rx_tree_erase(&tree , idx) == false)
            ++err_counter ;
    }
    ticks.stop() ;
    print_performce(ticks.start_tick() , ticks.end_tick() , max_number) ;
    if(err_counter != 0)
        ::printf("failed to erase key , counter[%d] \n" , err_counter) ;



    rx_tree_final(&tree) ;
    printf("page count[%u] historic count[%u] memsize[%u]\n" , mems.count() , mems.historic_count() , mems.size()) ;

    const std::map<void * , uint32_t>& addrs = mems.addrs() ;
    for(std::map<void * , uint32_t>::const_iterator citer = addrs.begin() ; citer != addrs.end() ; ++citer)
    {
        ::printf("addr = %p key = %u \n" , citer->first , citer->second) ;
    }

    return 0 ;
}

