
#include "Client.h"
#include <time.h>
#include <stdio.h>
#include <string.h>

static int __global_client_id__ = 0 ;

Client::Client()
{
    terminated_ = false ;
    finaled_ = false ;
    connected_ = false ;

    handle_ = INVALID_SOCKET ;
    worker_ = INVALID_HANDLE_VALUE ;

    client_id_ = ++__global_client_id__ ;
}

Client::~Client()
{
    Final() ;
}

bool Client::Connect(const char * addr , int port)
{
    SOCKET s = ::socket(AF_INET , SOCK_STREAM , IPPROTO_TCP) ;
    if(s == INVALID_SOCKET)
        return false ;

    struct sockaddr_in sin ;
    sin.sin_family = AF_INET ;
    sin.sin_port = ::htons(port) ;
    sin.sin_addr.s_addr = ::inet_addr(addr) ;
    
    int result = ::connect(s , (const sockaddr *)&sin , sizeof(sin)) ;
    if(result != 0)
    {
        int err = ::WSAGetLastError() ;
        if(err == WSAEWOULDBLOCK)
            result = 0 ;
    }

    if(result == 0)
    {
        handle_ = s ;
        connected_ = true ;
        return true ;
    }
    
    ::closesocket(s) ;
    return false ;
}

void Client::Close()
{
    if(handle_ != INVALID_SOCKET)
    {
        ::shutdown(handle_ ,  SD_BOTH) ;
        ::closesocket(handle_) ;
        handle_ = INVALID_SOCKET ;
    }    
}

int Client::Send(const char *  buffer , int bufsize)
{
    return ::send(handle_ , buffer , bufsize , 0) ;
}

int Client::Recv(char * buffer , int bufsize)
{
    return ::recv(handle_ , buffer , bufsize , 0) ;
}

DWORD WINAPI ClientRoutine(LPVOID lpParameter)
{
    Client * client = (Client *)lpParameter ;
    client->Process() ;
    return 0 ;
}

void Client::Run()
{
    if(worker_ !=  INVALID_HANDLE_VALUE)
        return ;

    worker_ = ::CreateThread(NULL , 0 , ClientRoutine , this , 0 , NULL) ;
}

void Client::Process() 
{
    int sequence = 0 ;
    while(terminated_ == false && finaled_ == false)
    {
        ++sequence ;
        char sbuf[1024] , rbuf[1024] ;
        int size = ::sprintf(sbuf , "ClientID[%d] Time[%d] Sequence[%08d]" , client_id_ , (int)::time(NULL) , sequence) ;
        int sent_bytes = Send(sbuf , size) ;
        int recv_bytes = 0 ;
        while(true)
        {
            int recv_size = Recv(rbuf , sizeof(rbuf)) ;
            if(recv_size > 0)
                recv_bytes += recv_size ;
            else
                break ;

            if(recv_bytes >= sent_bytes)
                break ;
        }

        if(sent_bytes != recv_bytes)
        {
            ::printf("client[%d] occuse error \n" , client_id_) ;
            break ;
        }
    }
}

void Client::Terminate()
{
    terminated_ = true ;
}

void Client::Final()
{
    if(finaled_ == true)
        return ;
    finaled_ = true ;
    terminated_ = true ;
    if(worker_ != INVALID_HANDLE_VALUE)
    {
        ::WaitForSingleObject(worker_ , INFINITE) ;
        ::CloseHandle(worker_) ;
        worker_ = INVALID_HANDLE_VALUE ;
    }

    Close() ;
}


Client * StartClient(const char * addr , int port)
{
    Client * client = new Client() ;
    if(client->Connect(addr , port) == false)
    {
        delete client ;
        return NULL ;
    }

    client->Run() ;
    return client ;
}

void StopClient(Client * client)
{
    client->Terminate() ;
}

