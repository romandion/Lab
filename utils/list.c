/***********************************************************************************
    作者：陈铨
    日期：2007-11-02
    功能：
    版本历史：
    0.1.0  2007-11-02
    
***********************************************************************************/

#include <string.h>
#include "list.h"


int _default_compare(void *src , size_t size_src , void *dest , size_t size_dest)
{
    size_t _size = (size_src < size_dest) ? size_src : size_dest ;	
    int _cmp_result ;
    
    _cmp_result = memcmp(src , dest , _size) ;
    if(_cmp_result == 0)
        return (_size - size_dest) ;
    else
        return _cmp_result ;
}

#define DEFAULT_MALLOC(size) malloc(size)
#define DEFAULT_FREE(mem , size) free(mem)
#define DEFAULT_COMPARE(src , size_src , dest , size_dest) _default_compare(src , size_src , dest , size_dest) 


list_t *list_init(list_t *list , malloc_t malloc_fun = NULL , free_t free_fun = NULL , compare_t compare_fun = NULL , bool copy = false) 
{
    list_t *_tmp_list = list ;
    
    if(_tmp_list == NULL)
        _tmp_list = (list_t *)malloc(sizeof(list_t)) ;
        
    if(_tmp_list == NULL)
        return NULL ;
        
    memset(_tmp_list , 0 , sizeof(list_t)) ;
    
    if(malloc_fun == NULL) 
        _tmp_list->malloc_fun = DEFAULT_MALLOC ;
    else
        _tmp_list->malloc_fun = malloc_fun ;
        
    if(free_fun == NULL)
        _tmp_list->free_fun = DEFAULT_FREE ;
    else
        _tmp_list->free_fun = free_fun ;
        
    if(compare_fun == NULL)
        _tmp_list->compare_fun = DEFAULT_COMPARE ;
    else
        _tmp_list->compare_fun = compare_fun ;
    
    _tmp_list->copy = copy ;
        
    return _tmp_list ;
}

void list_final(list_t *list) 
{
    list_node_t *cur , *next;
    
    if(list == NULL)	
        return ;
        
    cur = list->head ;
    while(cur != NULL)
    {
    	next = cur->next ;
    	
        if(list->copy)
    	    list->free_fun(cur->value , cur->size) ;
    	    
    	list->free_fun(cur , sizeof(list_node_t)) ; 
    	
    	cur = next ;    	
    }
}

int list_insert(list_t *list , list_node_t *node , void *value , size_t size)
{
	list_node_t *cur , *tmp ;
	
    if(list == NULL || value == NULL || size <= 0)
        return -1 ;
    
    if((cur = (list_node_t *)list->malloc_fun(sizeof(list_node_t))) == NULL)
        return -1 ;    
    
    if(list->copy)
    {
        cur->value = list->malloc_fun(size) ;        
        memcpy(cur->value , value , size) ;
    }
    else
        cur->value = value ;
    
    cur->size = size ;
    
    //没有指定目标节点，则插入到最后
    if(node == NULL)
    {
        tmp = list->tail ;
        
        if(tmp == NULL)	
        {
        	cur->prev = NULL ;
        	cur->next = NULL ;
        	list->head = cur ;
        	list->tail = cur ;
        }
        else
        {
        	cur->next = tmp->next ;
        	tmp->next = cur ;
        	cur->prev = tmp ;
        	
        	list->tail = cur ;
        }        
    }
    else
    {
    	tmp = node->prev ;
    	
    	node->prev = cur ;
    	cur->next = node ;
    	
    	if(tmp == NULL)
    	{
    	    list->head = cur ;
    	    cur->prev = NULL ;    	    	
    	}
    	else
    	{
    	    tmp->next = cur ;
    	    cur->prev = tmp ;	
    	}
    }   
    
    list->count++ ;
    
    return 0 ; 
}

int list_delete(list_t *list , list_node_t *node) 
{
    list_node_t *cur , *prev , *next ;
    
    if(list == NULL)	
        return -1 ;
    
    if(node == NULL)    
        cur = list->tail ;
    else
        cur = node ;
        
    if(cur == NULL)
        return 0 ;
        
    prev = cur->prev ;
    next = cur->next ;
    
    if(list->copy)
        list->free_fun(cur->value , cur->size) ;	
        
    list->free_fun(cur , sizeof(list_node_t)) ;
    
    if(prev != NULL)
        prev->next = next ;
    else
        list->head = next ;
    
    if(next != NULL)
        next->prev = prev ;
    else
        list->tail = prev ;
        
    list->count-- ;
        
    return 0 ;
}

int list_push(list_t *list , void *value , size_t size) 
{
	if(list == NULL)
	    return -1 ;
	    
    return list_insert(list , list->head , value , size) ;	
}

list_node_t *list_pop(list_t *list) 
{
	list_node_t *cur , *next ;
	
	if(list == NULL || list->head == NULL)
	    return NULL ;
	    
	cur = list->head ;
	
	next = cur->next ;
	
	if(next != NULL)
	    next->prev = NULL ;
	
	list->head = next ;
	
	list->count-- ;
	
	return cur ;	
}

list_node_t *list_find(list_t *list , void *value , size_t size) 
{
    return list_iterator(list , NULL , value , size) ;
}

list_node_t *list_iterator(list_t *list , list_node_t *node , void *value , size_t size)
{
	list_node_t *cur ;
	
	if(list == NULL || value == NULL || size <= 0)
	    return NULL
	    
	if(node == NULL)
	    cur = list->head ;
	else
	    cur = node ;
	    
	while(cur != NULL)
	{
	    if(list->compare_fun(cur->value , cur->size , value , size) == 0)	
	        return cur ;
	}
	
	return NULL ;		
}

list_node_t *list_get(list_t *list , int index)
{
    list_node_t *node ;
    int _idx = 0 ;
    
    if(list == NULL || index < 0)
        return NULL ;
        
    node = list->head ;
    while(node != NULL)
    {
        if(_idx == index)
            return node ;
            
        _idx++ ;
        node = node->next ;
    }
    
    return NULL ;
}

