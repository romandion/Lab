

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mdll.h"

class CZ1{
public:
    CZ1() {value_ = 0 ;}
private:
    int value_ ;
} ;

int main(int argc , char * argv[])
{
    size_t size = 1024 ;
    void * addr = malloc(size)  ;
    if(addr != NULL)
        free(addr) ;

    CZ1 * z1 = new CZ1() ;
    delete z1 ;

    return 0 ;
}

