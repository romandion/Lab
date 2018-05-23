
#ifndef __PERF_H
#define __PERF_H 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <map>
#include <set>
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

    class item{
    public:
        item() ;
        item(const item& data) {assign(data) ;}
        item& operator=(const item& data) {assign(data) ; return (*this) ;}
        bool operator==(const item& data) const ;
        bool operator!=(const item& data) const ;
        void *      addr ;
        uint32_t    key ;
        size_t      size ;
        size_t      seq ;
        bool valid() const ;
    private:
        void assign(const item& data) ;
    } ;


    bool add(void * addr , size_t size , uint32_t key) ;
    bool del(void * addr) ;

    size_t historic_count() const ;
    size_t count() const ;
    size_t size() const ;

    void print_left() const ;
private:
    //size_t add_item(void * addr , uint32_t key , size_t size) ;
    void add_item(void * addr , uint32_t key , size_t size , mem_mgr::item& item) ;
    bool find_item(size_t sequence , mem_mgr::item& item) const ;
    void print_difference(const char * desc , const mem_mgr::item& old_item , const mem_mgr::item& new_item) const ;

    std::map<size_t , mem_mgr::item> items_ ;
    std::map<void * , size_t> addrs_ ;                  //一个地址只会匹配一个key
    std::map<uint32_t , std::set<size_t> > keys_ ;      //每个key可能会有多个地址但最多不超过level
    size_t historic_count_ ;
    size_t total_size_ ;
    size_t sequence_ ;
} ;

#endif  /** __PERF_H */
