
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <winternl.h>

static const int kTestValue = 8585 ;
static const size_t kStackMaxDepth = 256 ;
static void * kStack[kStackMaxDepth] ;
static size_t kStackDepth = 0 ;


const int * GetTestValue() ;
size_t GetCallStack(void ** stack , size_t max_depth) ;
void Unwind(size_t frame , void * addr , void * return_value) ;


int main(int argc , char * argv[])
{
    ::memset(kStack , 0 , sizeof(kStack)) ;

    const int *result = GetTestValue() ;
    if(result != NULL)
        ::printf("result = %d \n" , *result) ;

    return 0 ;
}

const int * GetTestValue() 
{
    const int * result = &kTestValue ;
    kStackDepth =  GetCallStack(kStack , kStackMaxDepth) ;

    for(size_t fidx = 0 ; fidx < kStackMaxDepth ; ++fidx)
    {
        void * addr = kStack[fidx] , *return_value = NULL;
        //Unwind(fidx , addr , return_value) ;
    }

    return result ;
}

size_t GetCallStack(void ** stack , size_t max_depth)
{
    return (size_t)::CaptureStackBackTrace(0 , kStackMaxDepth , stack , NULL) ;
}

void Unwind(size_t frame , void * addr , void * return_value) 
{
    /**
    EXCEPTION_RECORD except ;
    ::memset(&except , 0 , sizeof(except)) ;

    typedef VOID (*RtlUnwind_t)(PVOID TargetFrame , PVOID TargetIp , PEXCEPTION_RECORD ExceptionRecord , PVOID ReturnValue);

    HMODULE module = GetModuleHandle("kernel32.dll") ;
    RtlUnwind_t f = (RtlUnwind_t)::GetProcAddress(module , "RtlUnwind") ;
    if(f == NULL)
    {
        DWORD errcode = ::GetLastError() ;
        return ;
    }

    f(&frame , addr , &except , return_value) ;
    */
}

