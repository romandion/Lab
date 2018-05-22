
#include "perf.h"

tick_counter::tick_counter()
{
    start_tick_ = end_tick_ = 0 ;
}

void tick_counter::start()
{
    start_tick_ = ::GetTickCount() ;
    end_tick_ = 0 ;
}

void tick_counter::stop()
{
    end_tick_ = ::GetTickCount() ;
}

DWORD tick_counter::elapse() const
{
    if(start_tick_ >= end_tick_)
        return 0 ;

    return (end_tick_ - start_tick_) ;
}

double tick_counter::perf(DWORD times) const
{
    double et = elapse() ;
    if(times == 0)
        return et ;

    return ((et * 1000)/ times) ;
}

mem_mgr::mem_mgr()
{
    total_size_ = 0 ;
    historic_count_ = 0 ;
}

bool mem_mgr::add(void * addr , size_t size , uint32_t key)
{
    std::map<void * , uint32_t>::iterator iter = addrs_.find(addr) ;
    if(iter != addrs_.end())
        return false ;
    addrs_.insert(std::pair<void * , uint32_t>(addr , key)) ;
    historic_count_++ ;
    total_size_ += size ;
    return true ;
}

bool mem_mgr::del(void * addr)
{
    std::map<void * , uint32_t>::iterator iter = addrs_.find(addr) ;
    if(iter == addrs_.end())
        return false ;
    addrs_.erase(iter) ;
    return true ;
}

size_t mem_mgr::historic_count() const 
{
    return historic_count_ ;
}

size_t mem_mgr::count() const
{
    return addrs_.size() ;
}

size_t mem_mgr::size() const 
{
    return total_size_ ;
}
