
#include "Service.h"
#include "AsynResult.h"
#include "Logger.h"
#include "SockOpt.h"

#include <MSWSock.h>

ServiceCallback::ServiceCallback()
{
    Owner = NULL ;

    //::ConnectEx
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
        ::fprintf(logfile , "failed to init IOCP \n") ;
        return false ;
    }

    iocp_ = h ;
    ::fprintf(logfile , "succeed to init IOCP \n") ;
    return true ;
}

bool Service::Init(int port)
{
    if(InitIOCP() == false)
        return false ;

    if(acceptor_.Init(port) == false)
        return false ;

    inited_ = true ;
    ::fprintf(logfile , "succeed to init Service \n") ;
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
        if(result->Type() == OVLP_INPUT && result->Status() == 0)
        {
            SOCKET s = result->Owner() ;
            if(Echo(s) == true)
            {
                //¼ÌÐø¶ÁÈ¡
                StartReading(s) ;
                continue ;
            }
        }

        if(result->Type() == OVLP_OUTPUT)
        {
            ::fprintf(logfile , "SOCKET[%d] sent [%u] bytes when iocompleted \n" , (int)result->Owner() , numTransfered) ;
        }

        delete result ;
        /**
        result->ReadToWrite() ;
        StartWriting(result->Owner() , result) ;
        */
    }
}

bool Service::Echo(SOCKET& s) 
{
    char buffer[32] ;
    int rsize = ::recv(s , buffer , sizeof(buffer) , 0) ;
    ::fprintf(logfile , "2¡¢SOCKET[%d] have recv [%d] bytes \n" , s , rsize) ;
    if(rsize <= 0)
        return false ;

    AsynResult * result = new AsynResult(s , OVLP_OUTPUT) ;
    result->Write(buffer , rsize) ;

    StartWriting(s , result) ;
    ::fprintf(logfile , "3¡¢SOCKET[%d] have Echo [%d] bytes \n" , s , rsize) ;
    return true ;
}

bool Service::ProcessNewConnect(SOCKET &s)
{
    SockOpt so(s) ;
    char buffer[1024] ;
    so.ToString(buffer , sizeof(buffer)) ;
    ::fprintf(logfile , "process new connection[%d] %s\n" , s , buffer) ;
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
    ::fprintf(logfile , "5¡¢SOCKET[%d] start to pre read\n" , s) ;
    int status = ::WSARecv(s , result->GetWSABUF() , 1 , &bytesReceived , &flags , result , NULL) ;
    if(status == 0)
        return ;

    int error = ::WSAGetLastError() ;
    if(error != WSA_IO_PENDING)
    {
        ::fprintf(logfile , "6¡¢SOCKET[%d] failed to read , error code[%d]\n" , s , error) ;
        result->Failure(error) ;
        return ;
    }

    ::fprintf(logfile , "6¡¢SOCKET[%d] succeed to pre read [%d] bytes\n" , s , (int)bytesReceived) ;
}

void Service::StartWriting(SOCKET& s , AsynResult * result) 
{
    DWORD bytesSent = 0 ;
    int status = ::WSASend(s , result->GetWSABUF() , 1 , &bytesSent , 0 , result , NULL) ;
    if(status != 0)
    {
        int error = ::WSAGetLastError() ;
        if(error != WSA_IO_PENDING)
        {
            ::fprintf(logfile , "SOCKET[%d] failed to send , error code[%d]\n" , s , error) ;
            result->Failure(error) ;
            return ;
        }
    }

    ::fprintf(logfile , "SOCKET[%d] succeed to write [%d] bytes immediatly \n" , s , (int)bytesSent) ;
}
