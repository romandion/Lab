
#include "service.h"

Service::Service()
{
    inited_ = false ;
    terminated_ = false ;
    finaled_ = false ;
    iocp_ = INVALID_HANDLE_VALUE ;
    serv_handle_ = INVALID_SOCKET ;
    worker_ = INVALID_HANDLE_VALUE ;
}

Service::~Service()
{
    Final() ;
}

bool Service::InitIOCP() 
{
    HANDLE h = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE , NULL , 0 , 0) ;
    if(h == NULL || h == INVALID_HANDLE_VALUE)
        return false ;

    iocp_ = h ;
    return true ;
}

bool Service::Init(int port)
{
    if(InitIOCP() == false)
        return false ;

    SOCKET s = ::socket(AF_INET , SOCK_STREAM , IPPROTO_TCP) ;
    if(s == INVALID_SOCKET)
        return false ;

    struct sockaddr_in sin ;
    sin.sin_family = AF_INET ;
    sin.sin_port = ::htons(port) ;
    sin.sin_addr.s_addr = INADDR_ANY ;

    if(::bind(s , (const sockaddr *)&sin , sizeof(sin)) == SOCKET_ERROR)
        return false ;
    if(::listen(s , 16) == SOCKET_ERROR)
        return false ;

    //°ó¶¨µ½IOCPÖÐ
    if(::CreateIoCompletionPort((HANDLE)s , iocp_ , 0 , 0) == NULL)
        return false ;

    serv_handle_ = s ;

    //::WSAAccept(

    inited_ = true ;
    return true ;
}

void Service::Final()
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

    for(std::set<SOCKET>::iterator iter = clients_.begin() ; iter != clients_.end() ; ++iter)
    {
        SOCKET s = *iter ;
        ::closesocket(s) ;
    }
    clients_.clear() ;
    ::CloseHandle(iocp_) ;
    iocp_ = INVALID_HANDLE_VALUE ;
}


DWORD WINAPI ServiceRoutine(LPVOID lpParameter)
{
    Service * service = (Service *)lpParameter ;
    service->Loop() ;
    return 0 ;
}

void Service::Run()
{
    if(worker_ !=  INVALID_HANDLE_VALUE)
        return ;

    worker_ = ::CreateThread(NULL , 0 , ServiceRoutine , this , 0 , NULL) ;
}

void Service::Terminate()
{
    terminated_ = true ;
}

void Service::Loop()
{
    while(terminated_ == false && finaled_ == false)
    {
        DWORD bytes = 0 ;
        ULONG_PTR key = 0 ;
        LPOVERLAPPED ovlp = NULL ;
        if(::GetQueuedCompletionStatus(iocp_ , &bytes , &key , &ovlp , 1000) == FALSE)
            continue ;

        //if(key == 

    }
}


AsynResult::AsynResult()
{
    wsa_.buf = NULL ;
    wsa_.len = 0 ;
    bytes_ = 0 ;
    status_ = 0 ;
    type_ = OVLP_VOID ;

    bufsize_ = 4096 ;
    buffer_ = (char *)::malloc(bufsize_) ;
}

AsynResult::~AsynResult()
{
    if(buffer_ != NULL)
    {
        ::free(buffer_) ;
        buffer_ = NULL ;
    }
}

void AsynResult::Complete(int bytes)
{
    bytes_ = bytes ;
}





