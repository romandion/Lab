
#include "AsynResult.h"


AsynResult::AsynResult()
{
    OVERLAPPED * ov = (OVERLAPPED *)this ;
    ::memset(ov , 0 , sizeof(OVERLAPPED)) ;
    wsa_.buf = NULL ;
    wsa_.len = 0 ;
    bytes_ = 0 ;
    status_ = 0 ;
    type_ = OVLP_VOID ;
    owner_ = NULL ;

    bufsize_ = 4096 ;
    buffer_ = (char *)::malloc(bufsize_) ;
}

AsynResult::AsynResult(SOCKET& owner , OVLPType type) 
{
    OVERLAPPED * ov = (OVERLAPPED *)this ;
    ::memset(ov , 0 , sizeof(OVERLAPPED)) ;

    wsa_.buf = NULL ;
    wsa_.len = 0 ;
    bytes_ = 0 ;
    status_ = 0 ;
    type_ = OVLP_VOID ;
    owner_ = NULL ;

    bufsize_ = 4096 ;
    buffer_ = (char *)::malloc(bufsize_) ;

    owner_ = owner ;
    type_ = type ;
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

bool AsynResult::Result(const char *& buf , int& size)
{
    buf = wsa_.buf ;
    size = bytes_ ;
    return (buf != NULL && size > 0) ;
}

int AsynResult::Write(const void * buf , int size)
{
    if(size > bufsize_)
        size = bufsize_ ;
    ::memcpy(buffer_ , buf , size) ;

    wsa_.buf =  buffer_ ;
    wsa_.len = size ;
    bytes_ = 0 ;
    return size ;
}

void AsynResult::PrepairRead() 
{
    //wsa_.buf = buffer_ ;
    //wsa_.len = bufsize_ ;
    wsa_.buf = NULL ;
    wsa_.len = 0 ;
    bytes_ = 0 ;
}

void AsynResult::ReadToWrite() 
{
    OVERLAPPED * ovlp = (OVERLAPPED *)this ;
    ::memset(ovlp , 0 , sizeof(OVERLAPPED)) ;

    wsa_.len = bytes_ ;
    bytes_ = 0 ;
    type_ = OVLP_OUTPUT ;
}


