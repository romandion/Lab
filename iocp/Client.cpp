
#include "Client.h"
#include "Logger.h"
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

    ::fprintf(logfile , "ClientInit[%d]\n" , client_id_) ;
}

Client::~Client()
{
    Final() ;
}

bool Client::Connect(const char * addr , int port)
{
    SOCKET s = ::socket(AF_INET , SOCK_STREAM , IPPROTO_TCP) ;
    if(s == INVALID_SOCKET)
    {
        ::fprintf(logfile , "failed to init client socket , ClientID[%d]\n" , client_id_) ;
        return false ;
    }
    else
    {
        ::fprintf(logfile , "ClientID[%d] succeed to init  SOCKET[%d]\n" , client_id_ , s) ;
    }

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
        else
            ::fprintf(logfile , "ClientID[%d] failed to connect[%s:%d] errorcode[%d]\n" , client_id_ , addr , port , err) ;
    }

    if(result == 0)
    {
        handle_ = s ;
        connected_ = true ;
        ::fprintf(logfile , "ClientID[%d] SOCKET[%d] succeed to connect[%s:%d] \n" , client_id_ , s , addr , port) ;
        return true ;
    }
    
    ::closesocket(s) ;
    return false ;
}

void Client::Close()
{
    if(handle_ != INVALID_SOCKET)
    {
        ::fprintf(logfile , "ClientID[%d] will close SOCKET[%d]\n" , client_id_ , handle_) ;
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
        ::fprintf(logfile , "1¡¢ClientID[%d] begin to send %d bytes , %s \n" , client_id_ , size , sbuf) ;
        int sent_bytes = Send(sbuf , size) ;
        int recv_bytes = 0 ;
        while(true)
        {
            ::fprintf(logfile , "ClientID[%d]  begin to recv \n" , client_id_) ;
            int recv_size = Recv(rbuf , sizeof(rbuf)) ;
            if(recv_size > 0)
                recv_bytes += recv_size ;
            else
                break ;

            if(recv_bytes >= sent_bytes)
                break ;
        }

        ::fprintf(logfile , "4¡¢ClientID[%d] end to recv %d bytes \n" , client_id_ , recv_bytes) ;

        if(sent_bytes != recv_bytes)
        {
            ::fprintf(logfile , "client[%d] occuse error , sent[%d] recv[%d]\n" , client_id_ , sent_bytes , recv_bytes) ;
            break ;
        }

        ::Sleep(1000) ;
    }

    ::fprintf(logfile , "ClientID[%d] have break main loop \n" , client_id_) ;
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

