
#include "intrin_dll.h"
#include <intrin.h>

#pragma intrinsic(_BitScanForward)


int __builtin_ffs (int x) 
{ 
    unsigned long index = 0 ;
    unsigned char result = _BitScanForward(&index , (unsigned long)x) ;
    return ((result == 0) ? 0 : (int)(index + 1)) ;
}

