
#ifndef __RING_BUFFER_H
#define __RING_BUFFER_H 1

#include <stdlib.h>
#include <string.h>

class RingBuffer{
public:
    RingBuffer() ;
    RingBuffer(int capacity) ;
    virtual ~RingBuffer() ;

    bool Init(int capacity) ;
    void Final() ;

    int Write(const void * buf , int size) ;
    int Read(void * buf , int size) ;

    //剩余的连续空间，紧跟数据地址
    bool Space(void *& buf , int& size) ;
    bool ProduceSize(int size) ;

    bool Data(const char *& buf , int& size) const;
    bool ComsumeSize(int size) ;

    int DataSize() const ;

    inline bool Extend(int capacity) {return false ;}
    inline int Capacity() const {return capacity_ ;}
    inline int Head() const {return head_ ;}
    inline int Tail() const {return tail_ ;}
private:
    char * buffer_ ;
    int capacity_ ;
    int offset_ ;
    int head_ ;
    int tail_ ;
} ;

#endif /** __RING_BUFFER_H */
