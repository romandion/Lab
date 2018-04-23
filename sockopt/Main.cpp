
#include <stdio.h>
#include <stdlib.h>

#include "SockOpt.h"

int main(int argc , char * argv[])
{
    WSADATA wsaData ;
    int error = WSAStartup(MAKEWORD(2,2), &wsaData);
    if(error)
        return -1 ;

    SOCKET s = ::socket(AF_INET , SOCK_STREAM , IPPROTO_TCP) ;
    SockOpt opt(s) ;
    ::printf("SNDBUF = [%d] \n" , opt.SendBufferSize()) ;
    ::printf("SetSNDBUF's result [%s] , then [%d] \n" , opt.SendBufferSize(0) == true ? "TRUE" : "FALSE" , opt.SendBufferSize()) ; 

    WSACleanup();    

    return 0 ;

}

