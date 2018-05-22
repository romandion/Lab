
#ifndef __PERF_H
#define __PERF_H 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <map>
#include "stdint.h"

class tick_counter{
public:
    tick_counter() ;

    void start() ;
    void stop() ;

    DWORD elapse() const;

    double perf(DWORD times) const ;

    inline DWORD start_tick() const {return start_tick_ ;}
    inline DWORD end_tick() const {return end_tick_ ;}

private:
    DWORD start_tick_ ;
    DWORD end_tick_ ;
} ;

class mem_mgr{
public:
    mem_mgr() ;

    bool add(void * addr , size_t size , uint32_t key) ;
    bool del(void * addr) ;

    size_t historic_count() const ;
    size_t count() const ;
    size_t size() const ;

    inline const std::map<void * , uint32_t>& addrs() const {return addrs_ ;}

private:
    std::map<void * , uint32_t> addrs_ ;
    size_t historic_count_ ;
    size_t total_size_ ;
} ;

#endif  /** __PERF_H */
