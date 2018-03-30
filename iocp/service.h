
#ifndef __SERVICE_H
#define __SERVICE_H 1

#include <windows.h>
#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <set>

typedef enum{
    OVLP_VOID ,
    OVLP_ACCEPT ,
    OVLP_INPUT ,
    OVLP_OUTPUT 
} ovlp_type ;

class AsynResult : public OVERLAPPED{
public:
    AsynResult() ;
    virtual ~AsynResult() ;
    inline WSABUF * GetWSABUF() {return &wsa_ ;}
    inline ovlp_type GetType() const {return type_ ;}
    inline void SetType(ovlp_type type) {type_ = type ;}
    void Complete(int bytes) ;
private:
    WSABUF wsa_ ;
    int bytes_ ;
    int status_ ;
    ovlp_type type_ ;
    char * buffer_ ;
    int bufsize_ ;
} ;

class Service{
public:
    Service() ;
    virtual ~Service() ;

    bool Init(int port) ;
    void Final() ;

    void Run() ;
    void Terminate() ;
    void Loop() ;
private:
    bool inited_ ;
    bool terminated_ ;
    bool finaled_ ;
    HANDLE iocp_ ;
    SOCKET serv_handle_ ;
    std::set<SOCKET> clients_ ;
    HANDLE worker_ ;

    bool InitIOCP() ;

    void ProcessAccept() ;
} ;

#endif /** __SERVICE_H */
