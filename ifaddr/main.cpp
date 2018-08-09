
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>
#include <iphlpapi.h>
#include <iptypes.h>

static void wchar_to_char(PWCHAR ws)
{
    char str[256] = {'\0'} ;

    int msize = ::WideCharToMultiByte(CP_ACP , 0 , ws , -1 , str , sizeof(str) , NULL , FALSE) ;
    str[msize] = '\0' ;
    ::printf("%s\n" , str) ;
}


int main(int argc , char * argv[])
{
    ULONG flags = GAA_FLAG_SKIP_ANYCAST | GAA_FLAG_SKIP_MULTICAST |
      GAA_FLAG_SKIP_DNS_SERVER | GAA_FLAG_INCLUDE_PREFIX;
    IP_ADAPTER_ADDRESSES* buf = NULL ;
    ULONG size = 0;

    while(true)
    {
        ULONG r = GetAdaptersAddresses(AF_UNSPEC,flags,NULL,buf,&size);
        if(r == ERROR_SUCCESS)
            break ;

        if(buf != NULL)
        {
            ::free(buf) ;
            buf = NULL ;
        }

        if(r == ERROR_BUFFER_OVERFLOW)
        {
            buf = (IP_ADAPTER_ADDRESSES *)::malloc(size) ;
            continue ;
        }
        else if(r == ERROR_NO_DATA)
            break ;
        else if(r == ERROR_ADDRESS_NOT_ASSOCIATED)
            break ;
        else if(r == ERROR_INVALID_PARAMETER)
            break ;
        break ;
    }

    IP_ADAPTER_ADDRESSES *adapter = NULL;
    int count = 0;
    for(adapter = buf ; adapter != NULL ; adapter = adapter->Next)
    {
        if (adapter->OperStatus != IfOperStatusUp || adapter->FirstUnicastAddress == NULL)
            continue;

        //int name_size = WideCharToMultiByte(CP_UTF8,0,adapter->FriendlyName,-1,NULL,0,NULL,FALSE);
        //if(name_size <= 0)
        //    continue ;
        ::printf("AdapterName = %s \n" , adapter->AdapterName) ;

        wchar_to_char(adapter->DnsSuffix) ;
        wchar_to_char(adapter->Description) ;
        wchar_to_char(adapter->FriendlyName) ;
        //::wprintf(L"DnsSuffix = %s \n" , adapter->DnsSuffix) ;
        //::wprintf(L"Description = %s \n" , adapter->Description) ;
        //::wprintf(L"FriendlyName = %s \n" , adapter->FriendlyName) ;
        
        ::printf("Mtu = %u \n" , adapter->Mtu) ;
        ::printf("IfType = %d \n" , adapter->IfType) ;
        ::printf("TransmitLinkSpeed = %llu \n" , adapter->TransmitLinkSpeed) ;
        ::printf("ReceiveLinkSpeed = %llu \n" , adapter->ReceiveLinkSpeed) ;

        ::printf("PhysicalAddress = ") ;
        for(ULONG idx = 0 ; idx < adapter->PhysicalAddressLength ; ++idx)
        {
            ::printf("%02x " , adapter->PhysicalAddress[idx]) ;
        }
        ::printf("\n") ;

        for (IP_ADAPTER_UNICAST_ADDRESS* unicast_address = (IP_ADAPTER_UNICAST_ADDRESS*)adapter->FirstUnicastAddress;
             unicast_address != NULL;unicast_address = unicast_address->Next)
        {
            SOCKADDR *sa = unicast_address->Address.lpSockaddr ;
            SOCKADDR_IN *sin = (SOCKADDR_IN *)sa ;
            ::printf("unicast = %s:%d \n" , ::inet_ntoa(sin->sin_addr) , sin->sin_port) ;

            count++;
        }
    
    }

    
    return 0 ;
}

