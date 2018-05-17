
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <winnt.h>
#include <sddl.h>

int main(int argc , char * argv[])
{
    HANDLE token = NULL ;
    ::OpenProcessToken(GetCurrentProcess() , TOKEN_QUERY , &token) ;

    char ubuf[1024] ;
    DWORD usize = 0 ;
    ::GetTokenInformation(token , TokenUser , &ubuf , sizeof(ubuf) , &usize) ;
    TOKEN_USER * user = (TOKEN_USER *)ubuf ;
    char name[256] ;
    SID_NAME_USE snu ;
    DWORD nsize = sizeof(name) ;
    if(::LookupAccountSid(NULL , user->User.Sid , name , &nsize , NULL , &usize , &snu) == FALSE)
    {
        ::printf("lookup account sid failed , error code[%u] \n" , ::GetLastError()) ;
        ::CloseHandle(token) ;
        return -1 ;
    }

    char * uid = NULL ;
    if(::ConvertSidToStringSid(user->User.Sid , &uid) == TRUE)
        ::printf("sid = %s \n" , uid) ;

    ::CloseHandle(token) ;
     return 0 ;
}
