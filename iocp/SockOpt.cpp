
#include "SockOpt.h"
#include <stdio.h>
#include <stdlib.h>


SockOpt::SockOpt(SOCKET& s) 
:socket_(s) 
{
    //
}

SockOpt::~SockOpt()
{
    //
}

bool SockOpt::KeepAlive() const
{
    BOOL optval = FALSE ;
    int optlen = sizeof(optval) ;
    if(::getsockopt(socket_ , SOL_SOCKET , SO_KEEPALIVE , (char *)&optval , &optlen) == 0)
        return (optval == TRUE) ;
    printf("keepalive error[%d]\n" , WSAGetLastError()) ;
    return false ;
}

bool SockOpt::KeepAlive(bool val)
{
    BOOL optval = (val ? TRUE : FALSE) ;
    return (::setsockopt(socket_ , SOL_SOCKET , SO_KEEPALIVE , (const char *)&optval , sizeof(optval)) == 0) ;
}

int SockOpt::RecvBufferSize() const
{
    int optval = 0 , optlen = sizeof(optval) ;
    if(::getsockopt(socket_ , SOL_SOCKET , SO_RCVBUF , (char *)&optval , &optlen) == 0)
        return optval ;

    printf("RecvBufferSize error[%d]\n" , WSAGetLastError()) ;
    return 0 ;
}

bool SockOpt::RecvBufferSize(int size)
{
    return (::setsockopt(socket_ , SOL_SOCKET , SO_RCVBUF , (const char *)&size , sizeof(size)) == 0) ;
}

DWORD SockOpt::RecvTimeout() const
{
    DWORD optval = 0 ;
    int optlen = sizeof(optval) ;
    if(::getsockopt(socket_ , SOL_SOCKET , SO_RCVTIMEO , (char *)&optval , &optlen) == 0)
        return optval ;
    printf("SO_RCVTIMEO error[%d]\n" , WSAGetLastError()) ;
    return 0 ;
}

bool SockOpt::RecvTimeout(DWORD timeout)
{
    return (::setsockopt(socket_ , SOL_SOCKET , SO_RCVTIMEO , (const char *)&timeout , sizeof(timeout)) == 0) ;
}

int SockOpt::SendBufferSize() const
{
    int optval = 0 , optlen = sizeof(optval) ;
    if(::getsockopt(socket_ , SOL_SOCKET , SO_SNDBUF , (char *)&optval , &optlen) == 0)
        return optval ;
    printf("SO_SNDBUF error[%d]\n" , WSAGetLastError()) ;
    return 0 ;
}

bool SockOpt::SendBufferSize(int size)
{
    return (::setsockopt(socket_ , SOL_SOCKET , SO_SNDBUF , (const char *)&size , sizeof(size)) == 0) ;
}

DWORD SockOpt::SendTimeout() const 
{
    DWORD optval = 0 ;
    int optlen = sizeof(optval) ;
    if(::getsockopt(socket_ , SOL_SOCKET , SO_SNDTIMEO , (char *)&optval , &optlen) == 0)
        return optval ;
    printf("SO_SNDTIMEO error[%d]\n" , WSAGetLastError()) ;
    return 0 ;
}

bool SockOpt::SendTimeout(DWORD timeout)
{
    return (::setsockopt(socket_ , SOL_SOCKET , SO_SNDTIMEO , (const char *)&timeout , sizeof(timeout)) == 0) ;
}

void SockOpt::ToString(char * str , size_t size) const 
{
    ::sprintf(str , "KeepAlive[%s] RecvBufferSize[%d] RecvTimeout[%d] SendBufferSize[%d] SendTimeout[%d]" ,
        ((KeepAlive() == true)?"True":"False") , RecvBufferSize() , (int)RecvTimeout() , 
        SendBufferSize() , (int)SendTimeout()) ;
}

