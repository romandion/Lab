
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <errno.h>

int main(int argc , char * argv[])
{

    HANDLE iocp = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE , NULL , 0 , 0) ;
    if(iocp == NULL)
    {
        ::printf("failed to create iocp %u\n" , ::GetLastError()) ;
        return -1 ;
    }

    HANDLE hread = NULL , hwrite = NULL ;
    if(::CreatePipe(&hread , &hwrite , NULL , 0) == FALSE)
    {
        ::CloseHandle(iocp) ;
        return -2 ;
    }

    HANDLE ior = ::CreateIoCompletionPort(hread , iocp , 0 , 0) ;
    if(ior == NULL)
    {
        ::printf("failed to add hread , errcode = %u" , ::GetLastError()) ;
    }

    HANDLE iow = ::CreateIoCompletionPort(hwrite , iocp , 0 , 0) ;
    if(iow == NULL)
    {
        ::printf("failed to add hwrite , errcode = %u" , ::GetLastError()) ;
    }

    ::CloseHandle(iocp) ;
    ::CloseHandle(hread) ;
    ::CloseHandle(hwrite) ;   
    
    return 0 ;
}

