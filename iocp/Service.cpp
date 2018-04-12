
#include "Service.h"
#include "AsynResult.h"
#include "Logger.h"

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
    {
        ::printf("failed to init IOCP \n") ;
        return false ;
    }

    iocp_ = h ;
    ::printf("succeed to init IOCP \n") ;
    return true ;
}

bool Service::Init(int port)
{
    if(InitIOCP() == false)
        return false ;

    if(acceptor_.Init(port) == false)
        return false ;

    inited_ = true ;
    ::printf("succeed to init Service \n") ;
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

        BOOL status = ::GetQueuedCompletionStatus(iocp_ , &numTransfered , &completionKey , &ovlp , 1000) ;
        if(status == FALSE)
            continue ;

        AsynResult * result = (AsynResult *)ovlp ;
        if(result == NULL)
            continue ;

        if(result->Status() != 0)
        {
            delete result ;
            continue ;
        }

        result->Complete((int)numTransfered) ;
        if(result->Type() == OVLP_OUTPUT || result->Status() != 0)
        {
            delete result ;
            continue ;
        }

        result->ReadToWrite() ;
        StartWriting(result->Owner() , result) ;

        //¼ÌÐø¶ÁÈ¡
        StartReading(result->Owner()) ;
    }
}

bool Service::ProcessNewConnect(SOCKET &s)
{
    ::printf("process new connection[%d] \n" , s) ;
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
    AsynResult * result = new AsynResult(s , OVLP_INPUT) ;
    result->PrepairRead() ;
    ::printf("SOCKET[%d] begin to read\n" , s) ;
    int status = ::WSARecv(s , result->GetWSABUF() , 1 , &bytesReceived , &flags , result , NULL) ;
    if(status == 0)
        return ;

    int error = ::WSAGetLastError() ;
    if(error != WSA_IO_PENDING)
    {
        ::printf("SOCKET[%d] failed to read , error code[%d]\n" , s , error) ;
        result->Failure(error) ;
        return ;
    }

    ::printf("SOCKET[%d] succeed to read [%d] bytes\n" , s , (int)bytesReceived) ;
}

void Service::StartWriting(SOCKET& s , AsynResult * result) 
{
    DWORD bytesSent = 0 ;
    int status = ::WSASend(s , result->GetWSABUF() , 1 , &bytesSent , 0 , result , NULL) ;
    if(status == 0)
        return ;

    int error = ::WSAGetLastError() ;
    if(error != WSA_IO_PENDING)
    {
        ::printf("SOCKET[%d] failed to send , error code[%d]\n" , s , error) ;
        result->Failure(error) ;
        return ;
    }

    ::printf("SOCKET[%d] succeed to write [%d] bytes\n" , s , (int)bytesSent) ;
}
