
#include <windows.h>
#include <stdio.h>

#include "intrin_dll.h"


int main(int argc , char ** argv[])
{
   int ffs = __builtin_ffs(32) ;
   printf("index = %d \n" , ffs) ;

   return 0 ;
}
