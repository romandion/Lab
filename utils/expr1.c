/***********************************************************************************
    作者：陈铨
    日期：2007-10-31
    功能：
    版本历史：
    0.1.0  2007-10-31
      exec [-cmd] [-cmd para] [--para=value] para
      
      expr [-v] [-h] [-f filename] [--filename=filename] filename
***********************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>


#include "expr.h"

#define MAX_BUF_SIZE 0xFFFF
#define MAX_ARGV_COUNT    128

#define DEBUG_MSGOUT(...) debug_msgout(__FILE__ , __LINE__ , __VA_ARGS__)

int debug_msg(char *file_name , int line_no , const char *format , ...)
{
    va_list _va ;
    char _msg[1024] ;
    int len ;
       
    sprintf(_msg , "FILE:[%s] LINE:[%d] " , file_name , line_no) ;
        
    len = strlen(_msg) ;
        
    va_start(_va ,format);
    vsprintf(_msg + len , format , _va);
    va_end(_va);
        
    return printf("%s \n" , _msg) ;
}

struct argv_st{
    int   type  ;
    int   index ;
    char *cmd   ;
    int  cmd_size ;
    char *name  ;
    int  name_size ;
} ;

struct expr_st{
    char buffer[MAX_BUF_SIZE] ;
    argv_t argvs[MAX_ARGV_COUNT] ;
    int argv_count ;
} ;

typedef enum{
    ARGV_STATE_ARGV      = 0x00 ,
    ARGV_STATE_OPTION    = 0x01 ,    
    ARGV_STATE_CMD1             ,    
    ARGV_STATE_CMD2             ,        
    ARGV_STATE_EQ               ,    
    ARGV_STATE_NAME             ,    
    ARGV_STATE_INTEGR     = 0xF0 ,    
} argv_state_t ;

typedef struct state_st state_t ;

struct state_st{
    int  begin         ;
    int  end           ;
    int completed     ;
    argv_state_t state ;
} ;

//#define state_init(begin , state) {begin , 0 , 0 , state}
void state_init(state_t *state , int begin , argv_state_t argv_state)
{
    if(state == NULL)
        return ;
        
    state->begin = begin ;
    state->end = 0 ;
    state->completed = 0 ;
    state->state = argv_state ;	
}

static char _valids[] = {
/*        00   01   02   03   04   05   06   07   08   09   0A   0B   0C   0D   0E   0F   */	
/* 00 */  -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 ,
/* 10 */  -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 ,
/* 20 */  ' ', '!', '"', '#', '$', '%', '&','\'', '(', ')', -1 , '+', ',', '-', '.', '/',
/* 30 */  '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', -1 , -1 , '<', '=', '>', '?',
/* 40 */  '@', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
/* 50 */  'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '[','\\', ']', '^', '_',
/* 60 */  '`', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 
/* 70 */  'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z' ,-1 , '|', -1 , '~', -1 ,
/* 80 */  -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 ,
/* 90 */  -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 ,
/* A0 */  -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 ,
/* B0 */  -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 ,
/* C0 */  -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 ,
/* D0 */  -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 ,
/* E0 */  -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 ,
/* F0 */  -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 	
} ;

int _parse_argv(char *str , int size , argv_t *argv)
{
	int _tmp_size ;
	state_t _stack[MAX_ARGV_COUNT] ;
	int _top = 0, i = 0 , end ;
	
	if(str == NULL || size <= 0 || argv == NULL)
	    return -1 ;
	    
	printf("str =%s, size = %d \n" , str , size) ;    
	    
	memset(_stack , 0 , sizeof(_stack)) ;
	memset(argv , 0 , sizeof(argv_t)) ;
	
	
	/*
	 *  我们将_top定位在当前栈顶，如果要push或者pop一个元素，必须自己改变_top的值。
	 *  i 是字符串中的当前位置。在switch结束之后，每个while循环，都自动加1。
	 */
	//_stack[_top] = state_init(0 , ARGV_STATE_ARGV) ;	
	state_init(&_stack[_top] , 0 , ARGV_STATE_ARGV) ;
	
	while(i >= 0 && i < size)
	{
		printf("即将处理这个字符【%c】 , i = %d , size = %d , _top = %d \n" , str[i] , i , size , _top) ;
	    switch(str[i])
	    {
	    	case OPTION_L :	    		
	    		_top++ ;
	    		//_stack[_top] = state_init(i , ARGV_STATE_OPTION);
	    		state_init(&_stack[_top] , i , ARGV_STATE_OPTION) ;
	    		break ;
	        case OPTION_R :
	        	end = i - 1;
	        	if(_top > 0 && _stack[_top].state == ARGV_STATE_NAME)
	        	{
	        		//Name如果没有完整，那么应该完整了
	        		if(!_stack[_top].completed)
	        		{
	        			_stack[_top].end = end ;
	        			_stack[_top].completed = 1 ;
	        			end = _stack[_top].begin - 1 ;
	        			
	        			argv->name = &str[_stack[_top].begin] ;
	        			argv->name_size = _stack[_top].end - _stack[_top].begin + 1 ;
	        		}
	        		//Name已经完成了，验证CMD1和CMD2
	        		if(--_top <= 0)
	        		    return -1 ;
	        		    
	        		if(_stack[_top].state == ARGV_STATE_EQ)
	        		{
	        		    if(--_top <= 0)	
	        		        return -1 ;
	        		        
	        		    if(_stack[_top].state != ARGV_STATE_CMD2 || !_stack[_top].completed)
	        		        return -1 ;
	        		        
	        		    if(--_top <= 0)
	        		        return -1 ;	        		        
	        		}
	        		else
	        		{        			
	        		    if(_stack[_top].state == ARGV_STATE_CMD1)
	        		    {
	        		        if(!_stack[_top].completed)	
	        		        {
	        		            return -1 ;
	        		        }
	        		        else
	        		        {
	        		            if(--_top <= 0)
	        		                return -1 ;
	        		        }
	        		    }
	        		}
	        	}
	        	
	        	if((_top > 0 && _stack[_top].state == ARGV_STATE_EQ) || (_top <= 0))
	        	    return -1 ;
	        	    
	        	if(_top > 0 && _stack[_top].state == ARGV_STATE_CMD2)
	        	{	        	        
	        	    _stack[_top].completed = 1 ;
	        	    _stack[_top].end = end ;
	        	    
	        	    argv->cmd = &str[_stack[_top].begin] ;
	        	    argv->cmd_size = _stack[_top].end - _stack[_top].begin + 1 ;
	        	    
	        	    if(--_top <= 0)
	        	        return -1 ;
	        	}
	        	else
	        	{
	        	    if(_top > 0 && _stack[_top].state == ARGV_STATE_CMD1)
	        	    {
	        	    	_stack[_top].completed = 1 ;
	        	    	_stack[_top].end = end ;
	        	    	
	        	    	argv->cmd = str + _stack[_top].begin ;
	        	    	argv->cmd_size = _stack[_top].end - _stack[_top].begin + 1 ;
	        	    	
	        	    	if(--_top <= 0)
	        	    	    return -1 ;
 	        	    }
	            }
	            
	            if(_top > 0 && _stack[_top].state == ARGV_STATE_OPTION)
	            {
	                _stack[_top].completed = 1 ;
	                _stack[_top].end = i ;	
	                
	                argv->type = AEGV_TYPE_OPTIONAL ;
	                
	                printf("return from argv_state_option . i = %d\n" ,i) ;
	                
	                return i + 1;
	            }
	                        
	        	return -1 ;
	        	
	        case CMD_FLAG :
	        	
	        	_top++ ;	        	
	        	
	       	    if(i < size - 2 && (str[i + 1] == CMD_FLAG))
	       	    {
	       	        i++ ;
	       	        //_stack[_top] = state_init(i + 1, ARGV_STATE_CMD2) ;
	       	        state_init(&_stack[_top] , i+1 , ARGV_STATE_CMD2) ;
	       	    }
	       	    else
	       	        //_stack[_top] = state_init(i + 1 , ARGV_STATE_CMD1) ;
	       	        state_init(&_stack[_top] , i+1 , ARGV_STATE_CMD1) ;
	       	        
	       	    if(i >= size - 1 || _valids[str[i + 1]] < 0)
	       	        return -1 ;
	       	        
	       	    break ;	       	    
	       	case PARAM_EQ :
	       		if(_top <= 0 || _stack[_top].state != ARGV_STATE_CMD2)
	       		    return -1 ;
	       		
	       		//让CMD2结束    
	       		_stack[_top].end = i - 1 ;
	       		_stack[_top].completed = 1 ;
	       		
	       		argv->cmd = str + _stack[_top].begin ;
	       		argv->cmd_size = _stack[_top].end - _stack[_top].begin + 1 ;
	       		
	       		//记录EQ状态
	       		_top++ ;
	       		//_stack[_top] = state_init(i , ARGV_STATE_EQ) ;
	       		state_init(&_stack[_top] , i , ARGV_STATE_EQ) ;
	       		_stack[_top].end = i ;
	       		_stack[_top].completed = 1 ;	       		
	        	
	        	/*
	        	//添加NAME等待状态
	        	if(i < size - 1)
	        	{
	        	    _top++ ;
	        	    //_stack[_top] = state_init(i + 1 , ARGV_STATE_NAME) ;
	        	    state_init(&_stack[_top] , i + 1 , ARGV_STATE_NAME) ;
	        	}
	        	else
	        	    return -1 ;
	        	*/
	        	
	        	break ;	        
	        case OPTION_T :
	        	//如果状态已经结束，那么忽略
	        	if(_stack[_top].completed || _stack[_top].state == ARGV_STATE_ARGV)
	        	    break ;
	        	    
	        	//否则将结束当前状态，结束算法和OPTION_R很象，并期待继续输入
	        	end = i - 1 ;
	        	if(_top > 0 && _stack[_top].state == ARGV_STATE_NAME)
	        	{
	        		//Name如果没有完整，那么应该完整了
	        		if(!_stack[_top].completed)
	        		{
	        			_stack[_top].end = end ;
	        			_stack[_top].completed = 1 ;
	        			end = _stack[_top].begin - 1 ;
	        			
	        			argv->name = str + _stack[_top].begin ;
	        			argv->name_size = _stack[_top].end - _stack[_top].begin + 1 ;
	        		}
	        		//Name已经完成了，验证CMD1和CMD2
	        		if(--_top < 0)
	        		    return -1 ;
	        		    
	        		if(_stack[_top].state == ARGV_STATE_EQ)
	        		{
	        		    if(--_top <= 0)	
	        		        return -1 ;
	        		        
	        		    if(_stack[_top].state != ARGV_STATE_CMD2 || !_stack[_top].completed)
	        		        return -1 ;
	        		        
	        		    if(--_top < 0)
	        		        return -1 ;	        		        
	        		}
	        		else
	        		{        			
	        		    if(_stack[_top].state == ARGV_STATE_CMD1)
	        		    {
	        		        if(!_stack[_top].completed)	
	        		        {
	        		            return -1 ;
	        		        }
	        		        else
	        		        {
	        		            if(--_top < 0)
	        		                return -1 ;
	        		        }
	        		    }
	        		}
	        	}
	        	
	        	if((_top > 0 && _stack[_top].state == ARGV_STATE_EQ) || (_top < 0))
	        	    return -1 ;
	        	    
	        	if(_top >= 0 && _stack[_top].state == ARGV_STATE_CMD2)
	        	{	        	        
	        	    _stack[_top].completed = 1 ;
	        	    _stack[_top].end = end ;
	        	    
	        	    argv->cmd = str + _stack[_top].begin ;
	        	    argv->cmd_size = _stack[_top].end - _stack[_top].begin + 1 ;
	        	    
	        	    break ;
	        	}
	        	else
	        	{
	        	    if(_top > 0 && _stack[_top].state == ARGV_STATE_CMD1)
	        	    {
	        	    	_stack[_top].completed = 1 ;
	        	    	_stack[_top].end = end ;
	        	    	
	        	    	argv->cmd = str + _stack[_top].begin ;
	        	    	argv->cmd_size = _stack[_top].end - _stack[_top].begin + 1 ;
	        	    	
	        	    	break ;
 	        	    }
	            }
	            
	            if(_top < 0)
	                return -1 ;	            	    
	            	            
	            if(_top > 0 && _stack[_top].state == ARGV_STATE_OPTION)
	            {
	            	break ;
	            }
	            else
	            {
	                if(_stack[_top].state == ARGV_STATE_ARGV)	
	                {
	                    _stack[_top].completed = 1 ;
	                    _stack[_top].end = i ;
	                    
	                    argv->type = ARGV_TYPE_INTEGRANT ;
	                    
	                    return i + 1;	
	                }
	            }
	                        
	        	return -1 ;	        	
	        default :
	        	//非法字符
	        	if(_valids[str[i]] < 0 || _top < 0)	
	        	    return -1 ;
	        	
	        	//啥都没有，那肯定是必须参数类型了   
	            if(_stack[_top].state == ARGV_STATE_ARGV)
	            {
	            	_top++ ;
	            	state_init(&_stack[_top] , i , ARGV_STATE_NAME) ;
	            	break ;	            	
	            }
	        	    
	        	//当前的状态还没有结束，继续输入
	        	if(!_stack[_top].completed)
	        	    break ;
	        	    
	        	if(_top > 0 && (_stack[_top].state == ARGV_STATE_EQ || _stack[_top].state == ARGV_STATE_CMD1 || _stack[_top].state == ARGV_STATE_ARGV))
	        	{
	        		//期待NAME
	        	    _top++ ;
	        	    //_stack[_top] = state_init(i , ARGV_STATE_NAME) ;
	        	    state_init(&_stack[_top] , i , ARGV_STATE_NAME) ;
	        	    break ;
	        	}
	        	
	        	return -1 ;	        	
	    }
	    
	    i++ ;
	} //end	while(i < size)
	
	printf("最后到这里了,i = %d \n" , i) ;
	
	if(i != size || _top < 0)
	    return -1 ;
	    
	//现在字符全部循环结束了，必须将整个状态栈清空
	end = i - 1 ;
	if(_stack[_top].state == ARGV_STATE_NAME)
	{
	    if(!_stack[_top].completed)	
	    {
	    	_stack[_top].end = end ;
	    	_stack[_top].completed = 1 ;
	    	
	    	argv->name = str + _stack[_top].begin ;
	    	argv->name_size = _stack[_top].end - _stack[_top].begin + 1 ;	    	
	    }
	    
	    if(--_top < 0)
	        return -1 ;
	        
	    if(_stack[_top].state == ARGV_STATE_EQ)
	    {
	        if(--_top < 0)	
	            return -1 ;
	            
	        if(_stack[_top].state != ARGV_STATE_CMD2 || !_stack[_top].completed)
	            return -1 ;
	            
	        if(--_top < 0)
	            return -1 ;
	            
	        if(_stack[_top].state != ARGV_STATE_ARGV)
	            return -1 ;
	            
	        argv->type = ARGV_TYPE_INTEGRANT ;
	        
	        return i + 1;
	    }
	    else
	    {
	        if(_stack[_top].state == ARGV_STATE_CMD1)	
	        {
	            if(!_stack[_top].completed)	
	                return -1 ;
	                
	            if(--_top < 0)
	                return -1 ;
	                
	            if(_stack[_top].state != ARGV_STATE_ARGV)
	                return -1 ;
	                
	            argv->type = ARGV_TYPE_INTEGRANT ;
	            
	            return i + 1;
	        }
	        
	        if(_stack[_top].state == ARGV_STATE_ARGV)
	        {
	            argv->type = ARGV_TYPE_INTEGRANT ;
	            return i + 1;	
	        }
	        
	        return -1 ;
	    }
	}
	
	if(_stack[_top].state == ARGV_STATE_CMD2 || _stack[_top].state == ARGV_STATE_CMD1)
	{
	    if(!_stack[_top].completed)	
	    {
	        _stack[_top].end = end ;
	        _stack[_top].completed = 1 ;
	        
	        argv->cmd = str + _stack[_top].begin ;
	        argv->cmd_size = _stack[_top].end - _stack[_top].begin + 1 ;	        
	    }
	    
	    if(--_top < 0)
	        return -1 ;
	        
	    if(_stack[_top].state != ARGV_STATE_ARGV)
	    {
	        return -1 ;
	    }
	    
	    argv->type = ARGV_TYPE_INTEGRANT ;
	    return i + 1;
	}
	
	
	if(_stack[_top].state == ARGV_STATE_ARGV)
	{
		printf("return from argv line[%d] i[%d]\n" , __LINE__ , i+1) ;
	    return i + 1;
	}
	
	    
	return -1 ;	
}

expr_t *default_expr = NULL ;
expr_t _default_expr ;

expr_t *expr_init(expr_t *expr , char *str)
{
	int size , len , i , pos = 0;
	argv_t argv ;
	char *p = str;
	
	
	if(default_expr == NULL)
	    default_expr = &_default_expr ;
	    
    if(expr == NULL)	
        expr = default_expr ;
        
    if(expr == NULL || str == NULL)
        return NULL ;
        
    memset(expr , 0 , sizeof(expr_t)) ;
    
    len = strlen(str) ;
    i = 0 ;
    expr->buffer[pos++] = '\0' ;
    while(((size = _parse_argv(p , len , &argv)) >= 0) && (i < MAX_ARGV_COUNT) && (pos < MAX_BUF_SIZE))
    {
    	if(pos + argv.cmd_size + argv.name_size + 2 >= MAX_BUF_SIZE)
    	    break ;
    	
        expr->argvs[i].type = argv.type ;
        expr->argvs[i].index = i ;
        
        expr->argvs[i].cmd = expr->buffer + pos ;
        expr->argvs[i].cmd_size = argv.cmd_size ;
        if(argv.cmd_size > 0)
        {        	
            memcpy(expr->buffer + pos , argv.cmd , argv.cmd_size) ;
            pos += argv.cmd_size ;
        }
        
        expr->buffer[pos++] = '\0' ;
        
        expr->argvs[i].name = expr->buffer + pos ;
        expr->argvs[i].name_size = argv.name_size ;
        if(argv.name_size > 0)
        {
            memcpy(expr->buffer + pos , argv.name , argv.name_size) ;
            pos += argv.name_size ;
        }

        expr->buffer[pos++] = '\0' ;
        
        
        
        i++ ;
        p += size ;
        len -=size ;
        
        printf("p =%s, i = %d , len = %d , size = %d \n" , p , i , len , size) ;
    }
    
    expr->argv_count = i ;
    
    return expr ;    
}

argv_t *expr_find_cmd(expr_t *expr , const char *cmd) 
{
	int i ;
	
    if(cmd == NULL)	
        return NULL ;
        
    if(expr == NULL)
        expr = default_expr ;
        
    if(expr == NULL)
        return NULL ;
        
    for(i = 0 ; i < expr->argv_count ; i++)
    {
        if(strcmp(expr->argvs[i].cmd , cmd) == 0)	
            return &expr->argvs[i] ;
    }
    
    return NULL ;
}


argv_t *expr_iter(expr_t *expr) 
{
    static i = 0 ;
    argv_t *argv ;
    
    if(expr == NULL)
        expr = default_expr ;
        
    if(expr == NULL)
        return NULL ;
        
    if(i >= expr->argv_count)
    {
        i = 0 ;
        return NULL ;
    }
        
    argv = &expr->argvs[i] ;
    i++ ;
    
    return argv ;        
}

int expr_cmd_count(expr_t *expr) 
{
    if(expr == NULL)	
        expr = default_expr ;
    if(expr == NULL)
        return 0 ;
        
    return expr->argv_count ;
}

void expr_final(expr_t *expr)
{
    if(expr == NULL)	
        expr = default_expr ;
        
    if(expr == NULL)
        return ;
        
    memset(expr , 0 , sizeof(expr_t)) ;
    
    if(expr == default_expr)
        default_expr = NULL ;
}

char *expr_argv_cmd(expr_t *expr , argv_t *argv)
{
    if(expr == NULL)	
        expr = default_expr ;
        
    if(expr == NULL || argv == NULL)
        return NULL ;
        
    return argv->cmd ;
}

char *expr_argv_name(expr_t *expr , argv_t *argv) 
{
    if(expr == NULL)	
        expr = default_expr ;
        
    if(expr == NULL || argv == NULL)
        return NULL ;
        
    return argv->name ;	
}

int expr_argv_type(expr_t *expr , argv_t *argv)
{
    if(expr == NULL)	
        expr = default_expr ;
        
    if(expr == NULL || argv == NULL)
        return -1 ;
        
    return argv->type ;	
}

int expr_argv_index(expr_t *expr , argv_t *argv) 
{
    if(expr == NULL)	
        expr = default_expr ;
        
    if(expr == NULL || argv == NULL)
        return -1 ;
        
    return argv->index ;		
}
