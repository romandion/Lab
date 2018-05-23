
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

int main(int argc , char * argv[])
{
    const char * sem_name = "Global\\lab.ipc" ;
    HANDLE handle = ::CreateSemaphoreA(NULL , 0 , LONG_MAX , sem_name) ;

    LONG prev_count = 0 ;
    if(::ReleaseSemaphore(handle , 1 , &prev_count) == FALSE)
        ::printf("failed , errcode[%u] \n" , ::GetLastError()) ;
    else
        ::printf("prev count = %d \n" , (int)prev_count) ;

    int idx = 0 ;
    for(; idx < 200 ; ++idx)
    {
        DWORD result = 0 ;
        if((result = ::WaitForSingleObject(handle , INFINITE)) == WAIT_OBJECT_0)
            continue ;

        ::printf("idx = %d , result = %u , errcode = %u \n" ,idx , result , ::GetLastError()) ;
        break ;
    }

    ::CloseHandle(handle) ;

    /**
    handle = ::OpenSemaphoreA(SEMAPHORE_ALL_ACCESS , FALSE , sem_name) ;
    if(handle == NULL)
    {
        ::printf("failed to open sema , errcode = %u\n" , ::GetLastError()) ;
        return -1 ;
    }
    ::CloseHandle(handle) ;
    ::printf("succeed  to open sema \n") ;
    */
    return 0 ;
}

