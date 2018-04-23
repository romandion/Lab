
#ifndef __SOCK_OPT_H
#define __SOCK_OPT_H 1

#include <winsock2.h>

class SockOpt{
public:
    SockOpt(SOCKET& s) ;
    ~SockOpt() ;

    bool KeepAlive() const;
    bool KeepAlive(bool val);

    int RecvBufferSize() const ;
    bool RecvBufferSize(int size);

    DWORD RecvTimeout() const ;
    bool RecvTimeout(DWORD timeout);

    int SendBufferSize() const ;
    bool SendBufferSize(int size);

    DWORD SendTimeout() const ;
    bool SendTimeout(DWORD timeout);

    bool NonBlock(bool enable) ;

    DWORD ReadableSize() const ;


    void ToString(char * str , size_t size) const ;

private:
    SOCKET& socket_ ;
} ;


#endif /** __SOCK_OPT_H */
