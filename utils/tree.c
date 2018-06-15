/***********************************************************************************
    作者：陈铨
    日期：2007-11-16
    功能：类XML协议
        
    版本历史：
    0.1.0  2007-11-16
    
***********************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tree.h"

tree_t *tree_init(tree_t *tree , int copy = false, malloc_t malloc_fun = NULL , free_t free_fun = NULL , int max_branch = 0) 
{
    if(tree == NULL)	
        return NULL ;
        
    memset(tree , 0 , sizeof(tree_t)) ;
    
    tree->copy = copy ;
    
    if(malloc_fun == NULL)
        malloc_fun = malloc ;
    tree->malloc = malloc_fun ;
    
    if(free_fun == NULL)
        free_fun = free ;
    tree->free = free_fun ;
    
    tree->max_branch = (max_brach >=0 )? max_branch : 0 ;
    
    return tree ;
}

int _process_final(tree_t *tree , tree_node_t *node , void *arg)
{
    if(buf == NULL || size < 0 || tree == NULL)	
        return -1 ;
        
    if(tree->copy)
        tree->free(buf) ;
        
    tree->free(node) ;
    
    return 0 ;
}

void tree_final(tree_t *tree)
{
    if(tree == NULL)	
        return ;
        
    _travel(tree , tree->root , _process_final , TREE_TRAVEL_LAST_ROOT , NULL) ;
}

tree_node_t *tree_insert(tree_t *tree , tree_node_t *node , void *buf , size_t size)
{
    tree_node_t *cur , *parent , *prev , *next;
    int rtn ;
    
    if(tree == NULL || node == NULL || buf == NULL || size <= 0 || node == tree->root)
        return NULL ;
    
    if((cur = tree_malloc_node(tree)) == NULL)
        return NULL ;
        
    if(tree_node_assign(tree , cur , buf , size) != size)
    {
        tree_free_node(cur) ;	
        return NULL ;
    }
    
    parent = node->parent ;
    prev = node->prev ;
    
    cur->parent = parent ;
    cur->prev = prev ;
    
    if(prev == NULL)
    {
    	next = parent->child ;
        parent->child = cur ;
        
        cur->next = next ;
        next->prev = cur ;
    }
    else
    {
    	next = prev->next ;
        prev->next = cur ;
        next->prev = cur ;
        cur->next = next ;
    }
    
    return cur ;
}

tree_node_t *tree_add(tee_t *tree , tree_node_t *node , void *buf , size_t size) 
{
	tree_node_t *cur , *prev , *next ;
	
	if(tree == NULL || node == NULL || buf == NULL || size <= 0 || node == tree->root)
	    return NULL ;

    //找到目标位置	    
	cur = prev = node ;
	while(cur != NULL)
	{
	    prev = cur ;
	    cur = cur->next ;	
	}	    
	cur = prev ;
	 
	//创建新的节点   
	if((next = tree_malloc_node(tree)) == NULL)
	    return NULL ;

    //赋值
    if(tree_node_assign(tree , next , buf , size) != size)
    {
        tree_free_node(tree , next) ;
        return NULL ;	
    }
	
    //挂接
    next->prev = cur ;
    next->parent = cur->parent ;
    cur->next = next ;	
	
	return cur ;
}

tree_node_t *tree_addchild(tee_t *tree , tree_node_t *node , void *buf , size_t size) 
{
    tree_node_t *cur;
    
    if(tree == NULL || buf == NULL || size <= 0)
        return NULL ;
            
    if(node == NULL || node->child == NULL)        
    {            
        //分配新节点
        if((cur = tree_malloc_node(tree)) == NULL)
            return NULL ; 
        
        //赋值
        if(tree_node_assign(tree , cur , buf , size) != size)
        {
            tree_free_node(tree , node) ;	
            return NULL ;
        }
        
        if(node == NULL)
            tree->root = cur ;
        else
        {
            node->child = cur ;
            cur->parent = node ;	
        }
        
        return cur ;
    }    
    else
        return tree_add(tree , node->child , buf , size) ;
}

void tree_del(tee_t *tree , tree_node_t *node) 
{
    tree_node_t *parent , *prev , *next ;
    
    if(tree == NULL || node == NULL)
        return ;
       
    parent = node->parent ;
    
    prev = node->prev ;
    next = node->next ;
    
    if(prev != NULL)
        prev->next = next ;
        
    if(next != NULL)
        next->prev = prev ;
        
    if(parent != NULL)
    {
    	if(prev != NULL)
            parent->child = prev ;
        else
            parent->next = next ;
    }
    else
        tree->root = NULL ;
}

struct find_st{
    void *buf , 
    size_t size ;
    
    tree_node_t *node ;
    int part ;
    int offset ;
} ;

int find_process(tree_t *tree , tree_node_t *node , void *arg)
{
	find_st *f ;
	size_t size , f_size , node_size ;
	char *buf ;
	
	
	if(tree == NULL || node == NULL || arg == NULL)
	    return 0;
	    
	f = (struct find_st *)arg ;	
	
	f_size = f->size ;
	if(node->size <= f->offset) 
	    return 0 ;
	else
	    node_size = node->size - f->offset ;
	
	size = (f_size <= node_size) ? f_size : node_size;	
	buf = (char *)node->buf + f->offset ;
	
	if((f_size == node_size || (f->part && f_size < node_size)) && memcmp(f->buf , node->buf , size) == 0)
	{
		f->node = node ;		
		return 1 ;
	}
	else
	    return 0 ;
}

tree_node_t *tree_find(tree_t *tree , tree_node_t *node , void *buf , size_t size  , int part , int offset) 
{
    find_st f ;
    
    f.buf = buf ;
    f.size = size ;
    f.node = NULL ;
    f.part = part ;
    f.offset = offset ;
    
    tree_travel(tree , node , find_process , TREE_TRAVEL_FIRST_ROOT , &f) ;
    
    if(f.node != NULL)
        return f.node ;

    return NULL ;        
}

tree_node_t *tree_childs(tree_t *tree , tree_node_t *node , int index)
{
    tree_node_t *cur , *next ;
    int i ;
    
    if(tree == NULL || index < 0)
        return NULL ;
        
    if(node == NULL)
        cur = tree->root ;
    else
        cur = node ;
        
    if(cur == NULL || cur->child == NULL)
        return NULL ;
        
    for(i = 0 , cur = cur->child ; cur != NULL ; i++)
    {
        if(i == index)	
            return cur ;
            
        cur = cur->next ;
    }
}

tree_node_t *tree_malloc_node(tree_t *tree) 
{    
    tree_node_t *node ;
    
    if(tree == NULL)
        node = (tree_node_t *)malloc(sizeof(tree_node_t)) ;
    else
        node = (tree_node_t *)tree->malloc(sizeof(tree_node_t)) ;
        
    if(node == NULL)
        return NULL ;
        
    memset(node , 0 , sizeof(tree_node_t)) ;
    
    return node ;    	
}

void tree_free_node(tree_t *tree , tree_node_t *node)
{
    if(node == NULL)	
        return NULL ;
    
    if(tree == NULL)
        free(node) ;
    else    	
        tree->free(node) ;
}

int tree_node_copy(tree_t *tree , tree_node_t *dest , tree_node_t *src)
{
	void *tmp ;
	
    if(tree == NULL || src == NULL || dest == NULL)	
        return 0 ;
        
    if(tree->copy)
    {
        if(src->size > dest->size)
        {
            if((tmp = tree->malloc(src->size)) == NUL)
                return -1 ;
            
            tree->free(dest->buf) ;
            dest->buf = tmp ;            
        }
        
        dest->size = src->size ;                                    
        memcpy(tmp , src->buf , src->size) ;
    }    
    else
    {
        dest->buf = src->buf ;
        dest->size = src->size ;	
    }
    
    return src->size ;
}

int tree_node_assign(tree_t *tree , tree_node_t *node , void *buf , size_t size)
{
    if(tree == NULL || node == NULL || buf == NULL || size <= 0)
        return -1 ;
        
    if(tree->copy)
    {
        if(node->buf != NULL)	
            tree->free(node->buf) ;
            
        if((node->buf = tree->malloc(size)) == NULL)
            return -1 ;
        memcpy(node->buf , buf , size) ;
        node->size = size ;
    }
    else
    {
        node->buf = buf ;
        node->size = size ;	
    }
    
    return size ;
}

int tree_travel(tree_t *tree , tree_node_t *node , travel_process_t process , int travel_type , void *arg) 
{
	tree_node_t *root , *cur , *stack[TREE_TRAVEL_MAX_LEVEL] ;
	int result , top = 0 , count = 0;
	
	if(tree == NULL)
	    return -1 ;
	    
	if(node == NULL)
	    root = tree->root ;
	else
	    root = node ;
	if(root == NULL)
	    return -2 ;
    
    memset(stack , 0 , sizeof(stack)) ;
    stack[top] = root ;    
    
    while(top >= 0 && top < (TREE_TRAVEL_MAX_LEVEL - 1))
    {
        cur = stack[top];
        
        //先根遍历
        if(process!= NULL && travel_type == TREE_TRAVEL_FIRST_ROOT)
        {
        	count++ ;
            if((result = process(tree , node , arg)) != 0)	
                return result ;
        }        
        
        if(cur->child != NULL && (stack[top + 1] == NULL || stack[top + 1]->parent != cur))
        {
            top++ ;
            stack[top] = cur->child ;            	
        }
        else
        {
        	//中根遍历
            if(process!= NULL && travel_type == TREE_TRAVEL_MIDDLE_ROOT)	
            {
            	count++ ;
                if((result = process(tree , node , arg)) != 0)	
                    return result ;
            }
            
            if(cur->next != NULL)
                stack[top] = cur->next ;	
            else
            	top-- ;
            	
            //后根遍历
            if(process!= NULL && travel_type == TREE_TRAVEL_LAST_ROOT)	
            {
            	count++ ;
                if((result = process(tree , node , arg)) != 0)	
                    return result ;
            }            	
        }
    }
    
    return count ;	    
}

