
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

//GetDllDirectoryA

int main(int argc , char * argv[])
{
    char str[1024] ;

    DWORD slen = ::GetDllDirectoryA(sizeof(str) , str) ;
    if(slen == 0)
    {
        DWORD errcode = ::GetLastError() ;
        ::printf("GetDllDirectory failed , errcode[%u] \n" , errcode) ;
    }
    else
    {
        //::wprintf(_T("GetDllDirectory[%s] \n") , str) ;
    }

    return 0 ;
}

