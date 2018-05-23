
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
    sequence_ = 0 ;
}


mem_mgr::item::item()
{
    addr = NULL ;
    key = 0 ;
    size = 0 ;
    seq = 0 ;
}

bool mem_mgr::item::valid() const 
{
    if(addr == NULL || size == 0 || seq == 0) 
        return false ;
    else
        return true ;    
}

void mem_mgr::item::assign(const item& data)
{
    addr    = data.addr ;
    key     = data.key ;
    size    = data.size ;
    seq     = data.seq ;
}

bool mem_mgr::item::operator==(const item& data) const 
{
    if(addr != data.addr) return false ;
    if(key != data.key) return false ;
    if(size != data.size) return false ;
    if(seq != data.seq) return false ;

    return true ;
}

bool mem_mgr::item::operator!=(const item& data) const 
{
    if(addr != data.addr) return true ;
    if(key != data.key) return true ;
    if(size != data.size) return true ;
    if(seq != data.seq) return true ;

    return false ;
}

bool mem_mgr::add(void * addr , size_t size , uint32_t key)
{
    mem_mgr::item new_item ;
    add_item(addr , key , size , new_item) ;
    size_t sequence = new_item.seq ;
    std::map<void * , size_t>::iterator aiter = addrs_.find(addr) ;
    if(aiter != addrs_.end())
        return false ;
    addrs_.insert(std::pair<void * , size_t>(addr , sequence)) ;

    std::map<uint32_t , std::set<size_t> >::iterator kiter = keys_.find(key) ;
    if(kiter == keys_.end())
        kiter = keys_.insert(std::pair<uint32_t , std::set<size_t> >(key , std::set<size_t>())).first ;

    std::set<size_t>& seqs = kiter->second ;
    seqs.insert(sequence) ;

    historic_count_++ ;
    total_size_ += size ;
    return true ;
}

bool mem_mgr::del(void * addr)
{
    std::map<void * , size_t>::iterator aiter = addrs_.find(addr) ;
    if(aiter == addrs_.end())
    {
        ::printf("can not find addr[%p] \n" , addr) ;
        return false ;
    }
    size_t addr_sequence = aiter->second ;
    addrs_.erase(aiter) ;

    mem_mgr::item addr_item ;
    if(find_item(addr_sequence , addr_item) == false)
    {
        ::printf("failed to find item , addr[%p] sequence[%u] \n" , addr , addr_sequence) ;
        return false ;
    }

    uint32_t key = addr_item.key ;   
    std::map<uint32_t , std::set<size_t> >::iterator kiter = keys_.find(key) ;
    if(kiter == keys_.end())
    {
        ::printf("can not find item , key[%u] addr[%p] sequence[%u] \n" , key , addr , addr_sequence) ;
        return false ;
    }

    std::set<size_t>& seqs = kiter->second ;
    std::set<size_t>::iterator siter = seqs.find(addr_sequence) ;
    if(siter == seqs.end())
    {
        ::printf("can not find key[%u]'s sequence , which addr is [%p] \n" , key , addr) ;
        return false ;
    }
    seqs.erase(siter) ;
    if(seqs.empty() == true)
        keys_.erase(kiter) ;

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

void mem_mgr::print_left() const 
{
    for(std::map<void * , size_t>::const_iterator aiter = addrs_.begin() ; aiter != addrs_.end() ; ++aiter)
    {
        mem_mgr::item addr_item;
        if(find_item(aiter->second , addr_item) == false)
            ::printf("failed to find left addrs , addr[%p] sequence[%u] \n" , aiter->first , aiter->second) ;
        else
        {
            ::printf("print left addrs , addr[%p] key[%u] size[%u] sequence[%u] \n" , 
                addr_item.addr , addr_item.key , addr_item.size , addr_item.seq) ;
        }
    }

    for(std::map<uint32_t , std::set<size_t> >::const_iterator kiter = keys_.begin() ; kiter != keys_.end() ; ++kiter)
    {
        uint32_t key = kiter->first ;
        std::set<size_t> seqs = kiter->second ;

        for(std::set<size_t>::const_iterator siter = seqs.begin() ; siter != seqs.end() ; ++siter)
        {
            size_t sequence = (*siter) ;
            mem_mgr::item item ;
            if(find_item(sequence , item) == false)
                ::printf("failed to find left keys , key[%p] sequence[%u] \n" , key , sequence) ;
            else
                ::printf("print left keys , addr[%p] key[%u] size[%u] sequence[%u] \n" , 
                    item.addr , item.key , item.size , item.seq) ;            
        }
    }
}

void mem_mgr::add_item(void * addr , uint32_t key , size_t size , mem_mgr::item& item) 
{
    ++sequence_ ;

    item.addr = addr ;
    item.key = key ;
    item.size = size ;
    item.seq = sequence_ ;

    items_.insert(std::pair<size_t , mem_mgr::item>(sequence_ , item)) ;
}

bool mem_mgr::find_item(size_t sequence , mem_mgr::item& item) const 
{
    std::map<size_t , mem_mgr::item>::const_iterator citer = items_.find(sequence) ;
    if(citer == items_.end())
        return false ;

    item = citer->second ;
    return item.valid() ;
}

void mem_mgr::print_difference(const char * desc , const mem_mgr::item& old_item , const mem_mgr::item& new_item) const 
{
    if(old_item == new_item)
        return ;

    ::printf("%s , \n"
        "    addr[%p->%p] \n"
        "    size[%u->%u] \n"
        "    key[%u->%u] \n"
        "    sequence[%u->%u] \n" , 
        desc , 
        old_item.addr , new_item.addr , 
        old_item.size , new_item.size , 
        old_item.key  , new_item.key , 
        old_item.seq ,  new_item.seq) ;
}
