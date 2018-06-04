
#include <windows.h>

int main(int argc , char * argv[])
{
    wchar_t str[256] ;
    size_t size = 0 ;

    DWORD dsize = GetCurrentDirectoryW(sizeof(str) , str) ;

    return 0 ;
}

