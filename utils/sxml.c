/***********************************************************************************
    作者：陈铨
    日期：2007-11-15
    功能：类XML协议
        
    版本历史：
    0.1.0  2007-11-15    
    
***********************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sxml.h"


size_t decode_7bits((unsigned char *buf , size_t buf_len , size_t *used_len) 
{
	size_t i , len = 0 ;
	unsigned char value ;
	
	
	for(i = 0 ; i < buf_len ; i++)
	{
		value = buf[i] & 0x7F ;
		value <<= i * 7 ;
		
		len += value ;
		
		if(buf[i] & 0x80)
		    break ;
	}
	
	if(used_len != NULL)
	    *used_len = i ;
	    
	return len ;
}

size_t encode_7bits(unsigned char *buf , size_t buf_len , size_t size) 
{
    size_t i , count ;
    
    count = ((sizeof(size) * 8) + 1) / 7 ;            
        
    for(i = 0 ; i < count && size != 0 ; i++)	
    {
    	if(i != 0)
    	    size >>= 7 ;
    	    
    	if(buf != NULL && i < buf_len)
    	{
    	    buf[i] = size & 0x7F ;
    	    
    	    if(size == 0)
    	        buf[i] |= 0x80 ;
    	}
    }    
    
    return i ;
}

sxml_t *sxml_init(sxml_t *sxml , int copy)
{	
    if(sxml != NULL && tree_init(&sxml->tree , 1) != NULL)
    {
    	sxml->copy = copy ;
        return sxml;
    }
    else
        return NULL ;
}

int sxml_final(sxml_t *sxml)
{
    if(sxml == NULL)	
        return -1 ;
    
    tree_final(&sxml->tree) ;
    return 0 ;
}

sxml_tag_t *sxml_malloc_tag(sxml_t *sxml ,unsigned int type , void *buf , size_t size)
{
    sxml_tag_t *cur;
    size_t tag_size ;
	
    if(sxml == NULL)	
        return NULL ;
         
    if(sxml->copy && size >= sizeof(unsigned char *))
    {
        tag_size = sizeof(sxml_tag_t) - sizeof(unsigned char *) + size ;
    }
    else
        tag_size = sizeof(sxml_tag_t) ;
        
    cur = (sxml_tag_t *)sxml->tree.malloc(tag_size)	 ;        
    if(cur == NULL)
    {
        sxml->tree.free(cur) ;	                
        return NULL ;
    }
    
    memset(cur , 0 , sizeof(sxml_tag_t)) ;    
    if(sxml->copy)    
    {
        memcpy(cur->data.cont , buf , size) ;        
    }
    else
        cur->data.buf = buf ;
        
    cur->size = size ;
    cur->type = type ;

    return cur ;
}

int sxml_free_tag(sxml_t *sxml , sxml_tag_t *tag)
{
    if(sxml == NULL || tag == NULL)
        return -1 ;
        
    sxml->free(tag) ;
    return 0 ;
}

sxml_node_t *sxml_add(sxml_t *sxml , sxml_node_t *node , unsigned int type , void *buf , size_t size)
{
    sxml_tag_t *tag ;   
    sxml_node_t *new_node ;
    size_t tag_size ;
    
    if((tag = sxml_malloc_tag(sxml , type , buf , size)) == NULL)
        return NULL ;
    
    
    if(sxml->copy && size >= sizeof(unsigned char *))
    {
        tag_size = sizeof(sxml_tag_t) - sizeof(unsigned char *) + size ;
    }
    else
        tag_size = sizeof(sxml_tag_t) ;
    
    new_node = tree_add(&sxml->tree , node , tag , tag_size) ;
    if(new_node == NULL)
    {
        sxml_free_tag(sxml , tag) ;	
        return NULL ;
    }
    else
        return new_node ;    
}

sxml_node_t *sxml_addchild(sxml_t *sxml , sxml_node_t *node , unsigned int type , void *buf , size_t size)
{
    sxml_tag_t *tag ;
    sxml_node_t *new_node ;
    size_t tag_size ;
    
    if((tag = sxml_malloc_tag(sxml , type , buf , size)) == NULL)
        return NULL ;
        
    if(sxml->copy && size >= sizeof(unsigned char *))
    {
        tag_size = sizeof(sxml_tag_t) - sizeof(unsigned char *) + size ;
    }
    else
        tag_size = sizeof(sxml_tag_t) ;        
        
    new_node = tree_addchild(&sxml->tree , node , tag , tag_size) ;
    if(new_node == NULL)
    {
        sxml_free_tag(sxml , tag) ;	
        return NULL ;
    }
    else
        return new_node ;
}

sxml_node_t *sxml_addprop(sxml_t *sxml , sxml_node_t *node , unsigned int type , void *buf , size_t size)
{
	sxml_node_t *prop_node , *cont_node;

    prop_node = sxml_addchild(sxml , node , SXML_TAG_PROP , NULL , 0) ;	
    if(prop_node == NULL)
        return NULL ;
	
	if((cont_node = sxml_addchild(sxml , prop_node , type , buf , size)) == NULL)	
	{
	    sxml_del(sxml , prop_node) ;
	    return NULL ;	
	}
	
	return prop_node ;
}

int sxml_del(sxml_t *sxml , sxml_node_t *node) 
{
    if(sxml == NULL || node == NULL)	
        return -1 ;
        
    tree_del(&sxml->tree , node) ;
    return 0 ;
}

sxml_node_t *sxml_find(sxml_t *sxml , sxml_node_t *node , unsigned int type)
{
	if(sxml == NULL)
	    return NULL ;
	    
	return tree_find(&sxml->tree , node , &type , sizeof(type) , 1) ;
}

size_t _encode_head(unsigned char *buf ,size_t buf_len , unsigned int type , size_t size)
{
	size_t used ;
		    
	if((used = encode_7bits(buf , size , type)) <= 0)
	    return 0 ;
	    
	if(buf != NULL && size >0)
	{
	    buf +=	used ;
	    size -= used ;
	}
	
	used += encode_7bits(buf , size , size) ;
	
	return used ;
}

int _sxml_size_adjust(tree_t *tree , tree_node_t *node , void *arg)
{
    sxml_tag_t *tag ;
    size_t *psize , size;
    
    if(tree == NULL || node == NULL || arg == NULL) ;
        return 0 ;
        
    tag = (sxml_tag_t *)node->buf ;    

    size = tag->size ;
    //size += encode_7bits(NULL , 0 , tag->size) + 1 ; //SXML_TAG_DATA
    size += _encode_head(NULL , 0 , (SXML_TAG_DATA & 0x7F) , tag->size) ;
    
    psize = (size_t *)arg ;
    tag->packed_size = *psize + size ;
    
    *psize = size + _encode_head(NULL , 0 , tag->type , tag->packed_size) ;
    
    return 0 ;
}

size_t sxml_adjust_size(sxml_t *sxml , sxml_node_t *node , size_t *size) 
{
	size_t whole_size = 0 ;
	
	if(sxml == NULL)
	    return 0 ;
	    
	if(tree_travel(&sxml->tree , node , _sxml_size_adjust , TREE_TRAVEL_LAST_ROOT , &whole_size) != 0)
	    return 0 ;
	    
	if(size != NULL)		
	    *size = whole_size ;
	
	return whole_size ;
}

struct _sxml_pack_info{
    sxml_t *sxml ;
    
    unsigned char *buf ;    
    size_t size ;
} ;

int _sxml_tag_pack(tree_t *tree , tree_node_t *node , void *arg)
{
    sxml_tag_t *tag ;
    struct _sxml_pack_info *info ; 
    size_t size ;
        
    if(tree == NULL || node == NULL || arg == NULL) ;
        return -1 ;
    
    info = (struct _sxml_pack_info *)arg ;
    if(info->sxml == NULL || info->buf == NULL || info->size <= 0)
        return -1 ;
    
    if((tag = (sxml_tag_t *)node->buf) == NULL)
        return 0 ;
    
    if(tag->packed_size > info->size)
        return -1 ;


    if((size = _encode_head(info->buf , info->size , tag->type , tag->packed_size)) <= 0)
        return -2 ;
    
    info->buf += size ;
    info->size -=size ;
    
    if((size = _encode_head(info->buf , info->size , (SXML_TAG_DATA & 0x7F) , tag->size)) <= 0)
        return -2 ;
    
    info->buf += size ;
    info->size -=size ;

    if(tag->size > 0)    
    {
        if(info->sxml->copy)
            if(memcpy(info->buf , tag->data.cont , tag->size) != info->buf)
                return -3 ;
        else
            if(memcpy(info->buf , tag->data.buf , tag->size) != info->buf)
                return -3 ;
    }
    
    info->buf += tag->size ;
    info->size -= tag->size ;
    
    return 0 ;
}

int _calc_endian()
{
    static int little_endian = -1 ;	
    
	int flag = 1 ;
	
	if(littl_endian == -1)
	    little_endian = (flag & 0xFFFFFFFEL) ? 0 : 1 ;
	    
	return little_endian ;
}

size_t sxml_pack(sxml_t *sxml , void *buf , size_t size) 
{
	size_t whole_size = 0;	
	struct _sxml_pack_info info ;	
	
	if(sxml == NULL || sxml->root == NULL)
	    return 0 ;
	
    whole_size = sxml_adjust_size(sxml) ;    
    if(whole_size > size || whole_size <= 0)
        return 0 ;
        
    if(_calc_endian())
    {
        buf[0] = SXML_BORDER1 ;
        buf[1] = SXML_BORDER2 ;
    }
    else
    {
        buf[0] = SXML_BORDER2 ;
        buf[1] = SXML_BORDER1 ;    	
    }    
    
    info->sxml = sxml ;
    info->buf = buf + 2;
    info->size = size - 2;
    info->used_size = 0 ;
    
    if(tree_travel(&sxml->tree , _sxml_tag_pack , TREE_TRAVEL_FIRST_ROOT , &info) != 0)	
        return 0 ;
        
    return (info->buf - (unsigned char *)buf);
}

size_t _tag_parse(unsigned char *buf , size_t size , sxml_tag_t *tag)
{
	size_t used ;
	unsgined char *head = buf ;
	
	if(buf == NULL || size <= 0 || tag == NULL)
	    return 0 ;
	    
    memset(tag , 0 , sizeof(sxml_tag_t)) ;
    
    tag->type = decode_7bits(buf , size , &used) ;
    if(used <= 0)
        return 0 ;
        
    buf += used ;
    size -= used ;
    
    tag->packed_size = decode_7bits(buf , size , &used) ;
    if(used <= 0)
        return 0 ;
        
    buf += used ;
    size -= used ;
    
    if(buf[0] != SXML_TAG_DATA)
        return 0 ;
        
    buf += 1 ;
    size-- ;
    
    tag->size = decode_7bits(buf , size , &used) ;
    if(used <= 0)
        return 0 ;
    
    tag->data.buf = buf ;
        
    buf += used ;
    size -= used ;
        
    return (buf - head) ;   
}

size_t sxml_parse(sxml_t *sxml , void *buf , size_t size) 
{
	sxml_tag_t tag , *cur ;
	sxml_node_t *root , *stack[TREE_TRAVEL_MAX_LEVEL] , *node;
	size_t tag_size , packed_size ;
	int top = 0;
	
    if(sxml_init(sxml) == NULL)
        return 0 ;

    if(_calc_endian())
    {
        if(buf[0] != SXML_BORDER1 || buf[1] != SXML_BORDER2)	
            return 0 ;
    }
    else
    {
        if(buf[0] != SXML_BORDER2 || buf[1] != SXML_BORDER1)	
            return 0 ;    	
    }
    
    buf +=2 ;
    size -=2 ;
    
    if((tag_size = _tag_parse(buf , size , &tag)) <= 0)
        return 0 ;    
    if((root = sxml_addchild(sxml , NULL , tag.type , tag.data.buf , tag.size)) == NULL)
        return 0 ;

    cur= (sxml_tag_t *)root->buf ;
    cur->packed_size = tag.packed_size ;
    
    packed_size = size = tag.packed_size ;
    memset(stack , 0 , sizeof(stack)) ;
    stack[top] = root ;
    
    while(size >= 0 && top >= 0)
    {
    	node = stack[top] ;
    	cur = (sxml_tag_t *)node->buf ;
    	
    	if(cur->data.buf + cur->size >= buf)
    	{
    		top-- ;
    		continue ;
    	}
    	
    	    	
    	if((tag_size = _tag_parse(buf , size , &tag)) <= 0)
    	    return 0 ;
    	    
    	buf += tag_size ;
    	size -= tag_size ;
    	
    	top++ ;
    	if(node->child == NULL)
    	    stack[top] = sxml_addchil(sxml , node , tag.type , tag.data.buf , tag.size) ;
    	else
    	{
    		node = node->child ;
    	    stack[top] = sxml_add(sxml , node , tag.type , tag.data.buf , tag.size) ;
    	}
    	
    	if(stack[top] == NULL)
    	    return -1 ;    	    
    	    
    	cur = stack[top]->buf ;
    	cur->packed_size = tag.packed_size ;
    }
    
    
    return packed_size ;
}


