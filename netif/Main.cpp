
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
//#include <Netioapi.h>
#include <iphlpapi.h>

int main(int argc , char *argv[])
{

    //if_nametoindex
    NET_IFINDEX if_index = ::if_nametoindex(NULL) ;
    ::printf("%u \n" , if_index) ;

    //::printf("%u \n" , IF_NAMESIZE)

    for(size_t index = 0 ; index < 16 ; ++index)
    {   
        char name[256] ;
        ::memset(name , 0 , sizeof(name)) ;
        if(::if_indextoname(index , name) == NULL)
            continue ;

        ::printf("[%u] [%s] \n" , index , name) ;
    }
    return 0 ;
}

