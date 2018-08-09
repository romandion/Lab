
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <psapi.h>


typedef BOOL (WINAPI *GetProcessMemoryInfo_t)(HANDLE Process,PPROCESS_MEMORY_COUNTERS ppsmemCounters,DWORD cb);

int main(int argc , char * argv[])
{
    PROCESS_MEMORY_COUNTERS memory ;
    ::memset(&memory , 0 , sizeof(memory)) ;
    if(::GetProcessMemoryInfo(::GetCurrentProcess() , &memory , sizeof(memory)) == TRUE)
    {
        ::printf("%u \n" , memory.WorkingSetSize) ;
    }

    HMODULE handle = ::GetModuleHandle("kernel32.dll") ;
    GetProcessMemoryInfo_t K32GetProcessMemoryInfo = (GetProcessMemoryInfo_t)::GetProcAddress(handle , "K32GetProcessMemoryInfo") ;
    if(K32GetProcessMemoryInfo(::GetCurrentProcess() , &memory , sizeof(memory)) == TRUE)
    {
        ::printf("%u \n" , memory.WorkingSetSize) ;
    }
    
    return 0 ;
}

