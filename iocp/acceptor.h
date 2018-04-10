
#ifndef __ACCEPTOR_H
#define __ACCEPTOR_H 1

#include <winsock2.h>
#include <windows.h>
#include <list>

class AcceptorCallback{
public:
    AcceptorCallback() ;
    virtual ~AcceptorCallback() ;
    virtual bool OnAcceptConnect(SOCKET& s) ;
} ;

class Acceptor{
public:
    Acceptor() ;
    virtual ~Acceptor() ;

    bool Init(int port) ;
    void Final() ;
    
    void Process() ;    

    bool Pop(SOCKET& s) ;
    void Push(SOCKET& s) ;
    int Travel() ;

    inline void Callback(AcceptorCallback * cb) {callback_ = cb ;}
private:
    bool finaled_ ;
    SOCKET socket_ ;
    HANDLE worker_ ;
    HANDLE locker_ ;
    std::list<SOCKET> clients_ ;
    AcceptorCallback * callback_ ;

    bool InitListenPort(int port) ;
} ;

#endif /** __ACCEPTOR_H */
