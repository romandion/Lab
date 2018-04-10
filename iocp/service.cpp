
#include "service.h"

ServiceCallback::ServiceCallback()
{
    Owner = NULL ;
}

ServiceCallback::~ServiceCallback()
{
    Owner = NULL ;
}

bool ServiceCallback::OnAcceptConnect(SOCKET& s)
{
    return Owner->ProcessNewConnect(s) ;
}


Service::Service()
{
    inited_ = false ;
    terminated_ = false ;
    finaled_ = false ;
    iocp_ = INVALID_HANDLE_VALUE ;
    worker_ = INVALID_HANDLE_VALUE ;
    locker_ = ::CreateMutex(NULL , FALSE ,NULL) ;
    callback_.Owner = this ;
    acceptor_.Callback(&callback_) ;
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

    if(acceptor_.Init(port) == false)
        return false ;

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

    ::WaitForSingleObject(locker_ , INFINITE) ;
    for(std::set<SOCKET>::iterator iter = clients_.begin() ; iter != clients_.end() ; ++iter)
    {
        SOCKET s = *iter ;
        ::closesocket(s) ;
    }
    clients_.clear() ;
    ::ReleaseMutex(locker_) ;

    ::CloseHandle(iocp_) ;
    iocp_ = INVALID_HANDLE_VALUE ;
}


DWORD WINAPI ServiceRoutine(LPVOID lpParameter)
{
    Service * service = (Service *)lpParameter ;
    service->Process() ;
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

void Service::Process()
{
    while(terminated_ == false && finaled_ == false)
    {
        DWORD numTransfered = 0 ;
        ULONG_PTR completionKey = 0 ;
        OVERLAPPED * ovlp = NULL ;

        bool status = ::GetQueuedCompletionStatus(iocp_ , &numTransfered , &completionKey , &ovlp , 1000) ;
        if(status == false)
            continue ;

        AsynResult * result = (AsynResult *)ovlp ;
        if
    
    }
}

bool Service::ProcessNewConnect(SOCKET&s)
{
    if(::CreateIoCompletionPort((HANDLE)s , iocp_ , 0 , 0) == NULL)
        return false ;

    ::WaitForSingleObject(locker_ , INFINITE) ;
    clients_.insert(s) ;
    ::ReleaseMutex(locker_) ;

    StartReading(s) ;
    return true ;
}

void Service::StartReading(SOCKET& s)
{
    DWORD bytesReceived = 0 , flags = 0 ;
    AsynResult * result = new AsynResult() ;
    result->SetType(OVLP_INPUT) ;
    int status = ::WSARecv(s , result->GetWSABUF() , 1 , &bytesReceived , &flags , result , NULL) ;
    if(status == 0)
        return ;

    int error = ::WSAGetLastError() ;
    if(error != WSA_IO_PENDING)
        result->Status(error) ;
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





