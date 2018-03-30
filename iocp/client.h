
#ifndef __CLIENT_H
#define __CLIENT_H 1

#include <windows.h>
#include <winsock2.h>

class Client{
public:
    Client() ;
    virtual ~Client() ;

    bool Connect(const char * addr , int port) ;
    void Close() ;

    int Send(const char *  buffer , int bufsize) ;
    int Recv(char * buffer , int bufsize) ;

    void Run() ;
    void Terminate() ;
    void Final() ;
    void Loop() ;

    inline int ClientID() const {return client_id_ ;}
private:
    bool terminated_ ;
    bool finaled_ ;
    bool connected_ ;
    SOCKET handle_ ;
    HANDLE worker_ ;
    int client_id_ ;
} ;

Client * StartClient(const char * addr , int port) ;
void StopClient(Client * client) ;

#endif /** __CLIENT_H */
