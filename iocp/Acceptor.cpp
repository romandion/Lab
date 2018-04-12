
#include "Acceptor.h"
#include "Logger.h"

AcceptorCallback::AcceptorCallback()
{
    //
}

AcceptorCallback::~AcceptorCallback()
{
    //
}

bool AcceptorCallback::OnAcceptConnect(SOCKET& s) 
{
    return false ;
}

Acceptor::Acceptor()
{
    finaled_ = false ;
    callback_ = NULL ;
    socket_ = INVALID_SOCKET ;
    worker_ = INVALID_HANDLE_VALUE ;
    locker_ = ::CreateMutex(NULL , FALSE , NULL) ;
}

Acceptor::~Acceptor()
{
    Final() ;
}

bool Acceptor::InitListenPort(int port) 
{
    socket_ = ::socket(AF_INET , SOCK_STREAM , IPPROTO_TCP) ;
    if(socket_ == INVALID_SOCKET)
        return false ;

    struct sockaddr_in sin ;
    sin.sin_family = AF_INET ;
    sin.sin_port = ::htons(port) ;
    sin.sin_addr.s_addr = INADDR_ANY ;

    if(::bind(socket_ , (const sockaddr *)&sin , sizeof(sin)) == SOCKET_ERROR)
        return false ;
    if(::listen(socket_ , 16) == SOCKET_ERROR)
        return false ;

    ::printf("we have init listen port[%d] socket[%u]\n" , port , socket_) ;
    return true ;
}


DWORD WINAPI AcceptorRoutine(LPVOID lpParameter)
{
    Acceptor * acceptor = (Acceptor *)lpParameter ;
    acceptor->Process() ;
    return 0 ;
}

bool Acceptor::Init(int port)
{
    if(InitListenPort(port) == false)
    {
        if(socket_ != INVALID_SOCKET)
        {
            ::closesocket(socket_) ;
            socket_ = INVALID_SOCKET ;
        }

        return false ;
    }
    locker_ = ::CreateMutex(NULL , FALSE , NULL) ;
    worker_ = ::CreateThread(NULL , 0 , AcceptorRoutine , this , 0 , NULL) ;
    return true ;
}

void Acceptor::Final()
{
    if(finaled_ == true)
        return ;

    ::fprintf(logfile , "Acceptor begin to final \n") ;
    finaled_ = true ;
    if(worker_ != NULL)
    {
        ::WaitForSingleObject(worker_ , INFINITE) ;
        ::CloseHandle(worker_) ;
        worker_ = INVALID_HANDLE_VALUE  ;
    }

    ::WaitForSingleObject(locker_ , INFINITE) ;

    if(socket_ != INVALID_SOCKET)
    {
        ::closesocket(socket_) ;
        socket_ = INVALID_SOCKET ;
    }

    ::ReleaseMutex(locker_) ;
    ::CloseHandle(locker_) ;
}

int Acceptor::Travel() 
{
    if(callback_ == NULL)
        return 0 ;

    int counter = 0 ;
    ::WaitForSingleObject(locker_ , INFINITE) ;

    while(clients_.empty() == false)
    {
        SOCKET s = clients_.front() ;
        if(callback_->OnAcceptConnect(s) == false)
            break ;

        clients_.pop_front() ;
        ++counter ;
    }

    ::ReleaseMutex(locker_) ;
    return counter ;
}

void Acceptor::Process()
{
    while(finaled_ == false)
    {
        Travel() ;
        struct sockaddr_in saClient ;
        int saSize =sizeof(saClient) ;
        SOCKET client =  ::WSAAccept(socket_ , (SOCKADDR *)&saClient , &saSize , NULL , NULL) ;
        if(client == INVALID_SOCKET)
            continue ;

        ::fprintf(logfile , "accept a new in connection[%u] [%s]\n" , client , ::inet_ntoa(saClient.sin_addr)) ;
        u_long nonblock =1 ;
        ::ioctlsocket(client , FIONBIO , &nonblock) ;

        Push(client) ; 
        Travel() ;
    }
}

bool Acceptor::Pop(SOCKET& s)
{
    s = INVALID_SOCKET ;
    ::WaitForSingleObject(locker_ , INFINITE) ;

    if(clients_.empty() == false)
    {
        s = clients_.front() ;
        clients_.pop_front() ;
    }

    ::ReleaseMutex(locker_) ;
    return (s != INVALID_SOCKET) ;
}

void Acceptor::Push(SOCKET& s)
{
    ::WaitForSingleObject(locker_ , INFINITE) ;
    clients_.push_back(s) ;
    ::ReleaseMutex(locker_) ;
}
