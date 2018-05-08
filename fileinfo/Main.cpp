
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include <sys/stat.h>

static const char * file_name = "TestFileInfo1.txt" ;

int main(int argc ,char * argv[])
{
    HANDLE h = ::CreateFile(file_name , GENERIC_WRITE|GENERIC_READ , 0 , NULL , CREATE_ALWAYS , FILE_ATTRIBUTE_NORMAL , NULL) ;
    if(h == INVALID_HANDLE_VALUE)
    {
        ::printf("failed to open file \n") ;
        return -1 ;
    }

    BY_HANDLE_FILE_INFORMATION fi ;
    ::memset(&fi , 0 , sizeof(fi)) ;
    if(::GetFileInformationByHandle(h , &fi) == FALSE)
    {
        ::printf("failed to get file information \n") ;
    }
    else
    {
        LARGE_INTEGER li;
        li.LowPart = fi.nFileIndexLow ;
        li.HighPart = fi.nFileIndexHigh ;
        ::printf("Attributes[%08X] VolumeSerialNumber[%08X] \nFileIndexHigh[%08X] FileIndexLow[%08X]\n" , 
            fi.dwFileAttributes , fi.dwVolumeSerialNumber , fi.nFileIndexHigh , fi.nFileIndexLow) ;
    }

    ::CloseHandle(h) ;

     struct _stat32 s32 ;
     ::memset(&s32 , 0 , sizeof(s32)) ;

    _stat32(file_name , &s32) ;

    ::printf("st_dev=[%d] st_ino=[%d] \n" , s32.st_dev , s32.st_ino) ;

    return 0 ;
}
