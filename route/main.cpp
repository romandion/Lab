
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>
#include <iphlpapi.h>
#include <iptypes.h>
#include <ipmib.h>

static bool routes() ;

int main(int argc , char * argv[])
{

    routes() ;
    
    return 0 ;
}

void print_forward(MIB_IPFORWARDROW * forward) ;
int addr_to_str(char * str , int size , DWORD addr) ;

static bool routes()
{
    PMIB_IPFORWARDTABLE pift = NULL;
    ULONG size = 0;

    if (GetIpForwardTable (NULL, &size, FALSE) != ERROR_INSUFFICIENT_BUFFER)
        return false ;

    pift = (PMIB_IPFORWARDTABLE)::malloc(size) ;
    ::memset(pift , 0 , size) ;

    DWORD result = GetIpForwardTable (pift, &size, FALSE) ;
    if(result == NO_ERROR)
    {
        for(DWORD i = 0; i < pift->dwNumEntries; ++i)
        {
            //if (pift->table[i].dwForwardIfIndex == if_index 
            //&& pift->table[i].dwForwardMask == INADDR_BROADCAST)
            //    return pift->table[i].dwForwardDest;

            print_forward(&pift->table[i]) ;
        }
    }

    ::free(pift) ;
    return (result == NO_ERROR) ;
}

/**

typedef struct _MIB_IPFORWARDROW {
    DWORD dwForwardDest;
    DWORD dwForwardMask;
    DWORD dwForwardPolicy;
    DWORD dwForwardNextHop;
    IF_INDEX dwForwardIfIndex;
    union {
        DWORD dwForwardType;              // Old field name uses DWORD type.
        MIB_IPFORWARD_TYPE ForwardType;   // New field name uses enum type.
    };
    union {
        DWORD dwForwardProto;             // Old field name uses DWORD type.
        MIB_IPFORWARD_PROTO ForwardProto; // New field name uses enum type.
    };
    DWORD dwForwardAge;
    DWORD dwForwardNextHopAS;
    DWORD dwForwardMetric1;
    DWORD dwForwardMetric2;
    DWORD dwForwardMetric3;
    DWORD dwForwardMetric4;
    DWORD dwForwardMetric5;
} MIB_IPFORWARDROW, *PMIB_IPFORWARDROW;
*/
void print_forward(MIB_IPFORWARDROW * forward) 
{
    char str[1024] = {'\0'} ;
    int size = sizeof(str) , offset = 0 ;;

    offset +=::sprintf(str + offset , "Dest=") ;
    offset += addr_to_str(str + offset , size - offset , forward->dwForwardDest) ;

    offset +=::sprintf(str + offset , " Mask=") ;
    offset += addr_to_str(str + offset , size - offset , forward->dwForwardMask) ;

    offset +=::sprintf(str + offset , " Policy=%u", forward->dwForwardPolicy) ;

    offset +=::sprintf(str + offset , " NextHop=") ;
    offset += addr_to_str(str + offset , size - offset , forward->dwForwardNextHop) ;

    offset +=::sprintf(str + offset , " IfIndex=%u", forward->dwForwardIfIndex) ;
    offset +=::sprintf(str + offset , " ForwardType=%u", forward->ForwardType) ;
    offset +=::sprintf(str + offset , " ForwardProto=%u", forward->ForwardProto) ;
    offset +=::sprintf(str + offset , " ForwardAge=%u", forward->dwForwardAge) ;

    ::printf("%s\n" , str) ;
}

int addr_to_str(char * str , int size , DWORD addr) 
{
    unsigned char *bytes  = (unsigned char *)&addr ;
    return ::sprintf(str , "%hhu.%hhu.%hhu.%hhu" , bytes[0] , bytes[1] ,bytes[2] ,bytes[3]) ;
}


