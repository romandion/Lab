
#include "RingBuffer.h"

RingBuffer::RingBuffer()
{
    buffer_ = NULL ;
    capacity_ = 0 ;
    offset_ = head_ = tail_ = 0 ;
}

RingBuffer::RingBuffer(int capacity)
{
    buffer_ = NULL ;
    capacity_ = 0 ;
    offset_ = head_ = tail_ = 0 ;
    Init(capacity) ;
}

RingBuffer::~RingBuffer() 
{
    Final() ;
}

bool RingBuffer::Init(int capacity)
{
    buffer_ = (char *)::malloc(capacity) ;
    if(buffer_ == NULL)
        return false ;

    capacity_ = capacity ;
    return true ;
}

void RingBuffer::Final()
{
    if(buffer_ != NULL)
    {
        ::free(buffer_) ;
        buffer_ = NULL ;
    }

    buffer_ = NULL ;
    capacity_ = 0 ;
    offset_ = head_ = tail_ = 0 ;
}

int RingBuffer::Write(const void * buf , int size)
{
    void * space = NULL ;
    int space_size = 0 ;

    if(Space(space , space_size) == false)
        return 0 ;

    if(space_size > size)
        space_size = size ;

    ::memcpy(space , buf , space_size) ;
    if(ProduceSize(space_size) == false)
        return 0 ;
    else
        return space_size ;
}

int RingBuffer::Read(void * buf , int size)
{
    const char * data = NULL ;
    int data_size = 0 ;
    if(Data(data , data_size) == false)
        return 0 ;

    if(data_size > size)
        data_size = size ;

    ::memcpy(buf , data , data_size) ;
    if(ComsumeSize(data_size) == false)
        return 0 ;
    else
        return data_size ;
}

bool RingBuffer::Space(void *& buf , int& size)
{
    if(buffer_ == NULL || capacity_ <= 0)
        return false ;

    //如果已经到头了，回绕
    if(tail_ <= capacity_)
    {
        //如果已经满了，则返回
        if(head_ == 0)
            return false ;
        else
            tail_ = 0 ;
    }

    buf = buffer_ + tail_ ;
    size = capacity_ - tail_ ;
    return true ;
}

bool RingBuffer::ProduceSize(int size)
{
    tail_ += size ;
    return true ;
}

bool RingBuffer::Data(const char *& buf , int& size) const
{
    if(buffer_ == NULL || capacity_ <= 0)
        return false ;

    buf = buffer_ + head_ ;
    size = tail_ - head_ ;
    return true ;
}

bool RingBuffer::ComsumeSize(int size)
{
    head_ += size ;
    return true ;
}

int RingBuffer::DataSize() const
{
    return (tail_ - head_) ;
}

