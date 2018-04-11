
#ifndef __ASYN_RESULT_H
#define __ASYN_RESULT_H 1


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <winsock2.h>

typedef enum{
    OVLP_VOID ,
    OVLP_ACCEPT ,
    OVLP_INPUT ,
    OVLP_OUTPUT 
} OVLPType ;

/**
    做简单点，不需要考虑太多了。
*/
class AsynResult : public OVERLAPPED{
public:
    AsynResult() ;
    AsynResult(SOCKET& owner , OVLPType type) ;
    virtual ~AsynResult() ;
    void Complete(int bytes) ;
    bool Result(const char *& buf , int& size) ;
    int Write(const void * buf , int size) ;
    void PrepairRead() ;
    void ReadToWrite() ;

    inline int Status() const {return status_ ;}
    inline void Status(int s) {status_ = s ;}
    inline void Success() {status_ = 0 ;}
    inline void Failure(int error) {Status(error) ;}
    inline SOCKET& Owner() {return owner_ ;}
    inline void Owner(SOCKET& s) {owner_ = s ;}
    inline WSABUF * GetWSABUF() {return &wsa_ ;}
    inline OVLPType Type() const {return type_ ;}
    inline void Type(OVLPType type) {type_ = type ;}
    inline char * Buffer() {return buffer_ ;}
    inline int BufSize() const {return bufsize_ ;}
private:
    WSABUF wsa_ ;
    int bytes_ ;
    int status_ ;
    OVLPType type_ ;    
    SOCKET owner_ ;
    char * buffer_ ;
    int bufsize_ ;
} ;

#endif /** __ASYN_RESULT_H */
