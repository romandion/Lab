
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

int main(int argc , char *argv[])
{
    CHAR buffer[1024] ;

    DWORD size =::GetTempPath(sizeof(buffer) , buffer) ;

    return 0 ;
}

