/***********************************************************************************
    作者：陈铨
    日期：2007-10-31
    功能：
    版本历史：
    0.1.0  2007-10-31
    
***********************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "option.h"

option_t *default_option = NULL ;

option_t static_default_option ;    //default_option最好不用动态分配，可能没有释放，所以预定义一个，不存在内存泄漏问题。

int _default_option_handler(const char *item , const char *value) 
{
    return 0 ;	
}

#define  ITEM_TYPE_ERROR       -1    /* 错误的类型 */
#define  ITEM_TYPE_PENDING      0    /*  没有任何前缀开始，可能是值，也可能是一个参数名 */
#define  ITEM_TYPE_1            1    /*  以'-'开始 */
#define  ITEM_TYPE_2            2    /*  以'--'开始 */


int _item_type(char *argv)
{
	int len , i;
	
    if(argv == NULL || ((len = strlen(argv)) <= 0))
        return ITEM_TYPE_ERROR ;
        
    if(argv[0] != '-')
        return ITEM_TYPE_UNKNOWN ;
        
    if(len >= 2 && argv[1] == '-')
        return ITEM_TYPE_2 ;
    else
        return ITEM_TYPE_1 ;    
}

option_t *_get_option(option_t *option)
{
    option_t *_tmp_option = option ;
    
	/*
	 *  2007-11-05 , by chenquan
	 *  初始化选项管理器，保证他已经存在，否则使用默认的变量
	 */
	if(default_option == NULL)
	    default_option = &static_default_option ;
	    
    if(_tmp_option == NULL)	
        _tmp_option = default_option ;
    else
        _tmp_option = option ;
        
    memset(_tmp_option , 0 , sizeof(option_t)) ;
    
    return _tmp_option ;
}

option_t *option_init(option_t *option , int argc , char *argv[]) 
{
	option_t *_tmp_option = _get_option(option) ;
	int i , len , cur_type , prev_type , index ;
	char *argvs , *buffer = NULL;
	
	
	if(argc < 1 || argv == NULL || _tmp_option == NULL)
	    return NULL ;
	            
    /*
     *  2007-11-05
     *  保存文件名在buffer缓冲区中，包括'\0' ;
     *  保存输入的选项，只保留指针。
     */
    buffer = _tmp_option->buffer ;
    sprintf(buffer , "%s" , argv[0]) ;
    len = strlen(buffer) + ;
    buffer = buffer + len ;
    
    for(i = 0 ; i < argc -1 ; i++)
    	_tmp_option->argvs[i] = argv[i + 1] ;
    	
    _tmp_option->argc = i + 1 ;   
    
    /*
     *  2007-11-05
     *  解析输入的选项
     */
    index = 0 ;
    argvs = _tmp_option->argvs ;
    prev_type = ITEM_TYPE_ERROR ;
    
    while(i < _tmp_option->argc) 	
    {
    	cur_type = _item_type(argvs[index]) ;
    	if(cur_type <= ITEM_TYPE_ERROR)
    	{
    	    index++ ;
    	    continue ;	
    	}
    	
    	/*
    	 *  2007-11-05 , by chenquan
    	 *  PENDING类型，说明这个项还不知道是一个选项名还是值，如果他前面的是一个ITEM_TYPE_1，
    	 *  那么这个就是和前面匹配的。 
    	 */
    	if(cur_type == ITEM_TYPE_PENDING)
    	{
    		
    		//和前面的类型是匹配的，PENDING只和ITEM_TYPE_1配合
    		if(prev_type == ITEM_TYPE_1)
    		{
    			sprintf(buffer , "%s" , argvs[index]) ;
    			len = strlen(buffer) + 1;
    			
    			_tmp_option->values[_tmp_option->item_count - 1] = buffer;
    			buffer += len ;
    		}
    		else
    		{
    		    sprintf(buffer , "%s" , argvs[index]) ;
    		    len = strlen(buffer) + 1 ;
    		    
    		    _tmp_option->unhandles[_tmp_option->unhandle_count++] = buffer;	
    		    buffer += len ;
    		}
    		
    		prev_type = cur_type ;
    		index++ ;
    		
    		continue ;
    	}
    	
    	/*
    	 *  2007-11-05 , by chenquan
    	 *  ITEM_TYPE_1，先处理本身，作为选项名，同时期待后面的值的设置。
    	 */
    	if(cur_type == ITEM_TYPE_1)
    	{
    	    sprintf(buffer , "%s" , argvs[index] + cur_type) ;	
    	    len = strlen(buffer) + 1 ;
    	    
    	    _tmp_option->items[_tmp_option->item_count++] = buffer ;
    	    
    	    buffer += len ;
    	    
    	    prev_type = cur_type ;
    	    index++ ;
    	    
    	    continue ;
    	}    		
    	
    	if(cur_type == ITEM_TYPE_2)
    	{
    		for
    	}
    }
    
    _tmp_option->buf_size = buffer - _tmp_option->buffer ;
    
    return _tmp_option ;
}

int option_register_handler(option_t *option , const char *item , option_handler_t handler)
{
    option_t *_tmp_option = _get_option(option) ;
    int i , len;
    char *buffer ;
    
    if(_tmp_option == NULL || item == NULL)
        return -1 ;
        
    for(i = 0 ; i < _tmp_option->handler_count ; i++)
    {
        if(strcmp(item , _tmp_option->handler_key[i]) == 0)	
        {
            _tmp_option->handlers[i] = handler ;
            return i ;
        }
    }
    
    if(i >= MAX_OPTION_COUNT)
        return -1 ;
        
    buffer = _tmp_option->buffer + _tmp_option->buf_size ;
    sprintf(buffer , "%s" , item) ;
    len = strlen(buffer) + 1 ;
    _tmp_option->handler_key[i] = buffer ;
    _tmp_option->buf_size + len ;
    
    _tmp_option->handlers[i] = handler ;
    
    _tmp_option->handler_count++ ;   
    
    return i ;
}

int option_next_item(option_t *option , const char **item , const char **value)
{
    option_t *_tmp_option = _get_option(option) ;	
    int index ;
    
    if(_tmp_option == NULL)
        return -1 ;
        
    index = _tmp_option->item_index++ ;
    if(index < _tmp_option->item_count)
    {
    	if(item != NULL)
    	    *item = _tmp_option->items[index] ;
    	    
        if(value != NULL)
            *value = _tmp_option->values[index] ;
            
        return 0 ;
    }
    
    return -1 ;        
}

int option_next_unhandle(option_t *option , const char **unhandle)
{
    option_t *_tmp_option = _get_option(option) ;	
    int index ;
    
    if(_tmp_option == NULL)
        return -1 ;
        
    index = _tmp_option->unhandle_index++ ;
    if(index < _tmp_option->unhandle_count)
    {
    	if(unhandle != NULL)
    	    *unhandle = _tmp_option->unhandles[index] ;
    	    
        return 0 ;
    }
    
    return -1 ;        
}

int option_find_item(option_t *option , const char *item , const char **value)
{
	option_t *_tmp_option = _get_option(option) ;
	int i ;
	
	if(_tmp_option == NULL || item == NULL)
	    return -1 ;
	    
	for(i = 0 ; i < _tmp_option->item_count ; i++)
	{
	    if(strcmp(item , _tmp_option->items[i]) == 0)	
	    {
	        if(value == NULL)	
	            *value = _tmp_option->values[i] ;
	            
	        return i ;
	    }
	}
	
	return -1 ;
}

void option_final(option_t *option)
{    
    if(option == NULL)
        option = default_option ;
        
    memset(option , 0 , sizeof(option)) ;
    
    if(option == default_option)
        default_option = NULL ;
}

int get_option(int argc , char *argv[] , char **item , char **value) 
{
    static option_initialized = false ;
    option_t *_tmp_option = NULL ;
    int rtn ;
    
    if(!option_initialzed)
    {
        _tmp_option = option_init(NULL , argc , argv) ;
        if(_tmp_option == NULL)
            return -1 ;
            
        option_initialized = true ;
    }
    
    rtn = option_next_item(_tmp_option , item , value) ;
    if(rtn < 0)
    {
        option_final(_tmp_option) ;	        
        option_initialized = false ;
    }
    
    return rtn ;
}
