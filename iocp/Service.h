
#ifndef __SERVICE_H
#define __SERVICE_H 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <winsock2.h>
#include <string>
#include <set>

#include "Acceptor.h"
#include "AsynResult.h"

class Service ;
class ServiceCallback : public AcceptorCallback{
public:
    ServiceCallback() ;
    virtual ~ServiceCallback() ;
    virtual bool OnAcceptConnect(SOCKET& s) ;
    Service * Owner ;
} ;

class Service{
public:
    Service() ;
    virtual ~Service() ;

    bool Init(int port) ;
    void Final() ;

    void Run() ;
    void Terminate() ;
    void Process() ;
    bool ProcessNewConnect(SOCKET& s) ;
private:
    bool inited_ ;
    bool terminated_ ;
    bool finaled_ ;
    HANDLE iocp_ ;
    std::set<SOCKET> clients_ ;
    HANDLE worker_ ;
    HANDLE locker_ ;
    ServiceCallback callback_ ;
    Acceptor acceptor_ ;

    bool InitIOCP() ;

    void StartReading(SOCKET& s) ;
    void StartWriting(SOCKET& s , AsynResult * result) ;

    bool Echo(SOCKET& s) ;
} ;

#endif /** __SERVICE_H */
