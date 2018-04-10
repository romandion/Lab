
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "service.h"
#include "client.h"

int main(int argc , char ** argv)
{
    WSADATA wsaData ;
    int error = WSAStartup(MAKEWORD(2,2), &wsaData);
    if(error)
        return -1 ;


    WSACleanup();
    return 0 ;
}

