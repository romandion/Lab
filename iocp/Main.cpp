
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Service.h"
#include "Client.h"

int service_port = 56677 ;

int main(int argc , char ** argv)
{
    WSADATA wsaData ;
    int error = WSAStartup(MAKEWORD(2,2), &wsaData);
    if(error)
        return -1 ;

    Service svr;
    if(svr.Init(service_port) == false)
    {
        ::WSACleanup() ;
        return -1 ;
    }
    svr.Run() ;

    ::Sleep(3000) ;

    Client cli ;
    cli.Connect("127.0.01" , service_port);
    cli.Run() ;

    int chr = 0 ;
    while((chr = ::getchar()) != 'q')
    {
        continue ;
    }

    cli.Final() ;
    svr.Final() ;

    WSACleanup();
    return 0 ;
}

