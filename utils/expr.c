/***********************************************************************************
    作者：陈铨
    日期：2007-11-13
    功能：
    版本历史：
    1.1.0  2007-11-13
    
***********************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "expr.h"


/*
 *  1. ' ' 2. [a-z]|[A-Z] 3. [0-9] 4. '_' 5. '['|']' 6. '=' 7. '-'
 */

char expr_maps[256] = {
/*        00   01   02   03   04   05   06   07   08   09   0A   0B   0C   0D   0E   0F   */	
/* 00 */   0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,
/* 10 */   0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,
/* 20 */   1 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  7 ,  0 ,  0 ,
/* 30 */   3 ,  3 ,  3 ,  3 ,  3 ,  3 ,  3 ,  3 ,  3 ,  3 ,  0 ,  0 ,  0 ,  6 ,  0 ,  0 ,
/* 40 */   0 ,  2 ,  2 ,  2 ,  2 ,  2 ,  2 ,  2 ,  2 ,  2 ,  2 ,  2 ,  2 ,  2 ,  2 ,  2 ,
/* 50 */   2 ,  2 ,  2 ,  2 ,  2 ,  2 ,  2 ,  2 ,  2 ,  2 ,  2 ,  5 ,  0 ,  5 ,  0 ,  4 ,
/* 60 */   2 ,  2 ,  2 ,  2 ,  2 ,  2 ,  2 ,  2 ,  2 ,  2 ,  2 ,  2 ,  2 ,  2 ,  2 ,  2 , 
/* 70 */   2 ,  2 ,  2 ,  2 ,  2 ,  2 ,  2 ,  2 ,  2 ,  2 ,  2  , 0 ,  0 ,  0 ,  0 ,  0 ,
/* 80 */   0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,
/* 90 */   0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,
/* A0 */   0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,
/* B0 */   0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,
/* C0 */   0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,
/* D0 */   0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,
/* E0 */   0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,
/* F0 */   0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 
}


int _expr_space(int *state_stack , int *top , char *str , int size) ;

int _expr_char(int *state_stack , int *top , char *str , int size) ;

int _expr_number(int *state_stack , int *top , char *str , int size) ;

int _expr_underline(int *state_stack , int *top , char *str , int size) ;

int _expr_option(int *state_stack , int *top , char *str , int size) ;

int _expr_eq(int *state_stack , int *top , char *str , int size) ;

int _expr_cmd(int *state_stack , int *top , char *str , int size) ;



action_t expr_actions[32] {
/*            00                  01                  02                  03                */	
/*  00 */     NULL    ,      _expr_space  ,      _expr_char      ,    _expr_number    ,
/*  01 */ _expr_underline , _expr_option  ,      _expr_eq        ,    _expr_cmd       ,
/*  02 */     NULL    ,          NULL     ,          NULL        ,       NULL          ,
/*  03 */     NULL    ,          NULL     ,          NULL        ,       NULL          ,
/*  04 */     NULL    ,          NULL     ,          NULL        ,       NULL          ,
/*  05 */     NULL    ,          NULL     ,          NULL        ,       NULL          ,
/*  06 */     NULL    ,          NULL     ,          NULL        ,       NULL          ,
/*  07 */     NULL    ,          NULL     ,          NULL        ,       NULL    
}


int expr_parse(char *expr , int size , expr_handler_t handler) 
{
    int count , i , len ;
    expr_action_t action = NULL ;
    
    if(expr == NULL || size <= 0)
        return -1 ;
    
    for(i = 0 ; i < size ; i += len)
    {
    	if((action = expr_actions[expr_maps[expr[i]]]) == NULL) 
    	    return -2 ;
    	    
    	len = action(&type , &state , &expr[i] , size - i) ;
    	
    	if(len <= 0 || state == EXPR_STATE_ERROR)
    	    return -3 ;
    	
    	if(state == EXPR_STATE_FINISH && handler != NULL)
    	    handler(type , &expr[i] , len) ;
    }
    
    return i ;    	
}

int _valid(int *stack , int *top , char *str , int size) 
{
    if(stack== NULL || top == NULL || str == NULL || size <=0)	
        return -1 ;
    
    return 0 ;
}

int _expr_space(int *state_stack , int *top , char *str , int size) 
{
	int i  , len , type , state ;
	
    if(_valid(state_stack , top , str , size) < 0)
        return -1 ;
        
    i = *top ;
    type = EXPR_TYPE(state_stack[i]) ;
    state = EXPR_TYPE(state_stack[i]) ;
    
    switch(type)
    {
        case EXPR_TYPE_EXPR :
        	return 1 ;
        case EXPR_TYPE_SCHEME :
        	return 1 ;
        case EXPR_TYPE_OPTION :
    	
    }
}

int _expr_char(int *state_stack , int *top , char *str , int size) ;

int _expr_number(int *state_stack , int *top , char *str , int size) ;

int _expr_underline(int *state_stack , int *top , char *str , int size) ;

int _expr_option(int *state_stack , int *top , char *str , int size) ;

int _expr_eq(int *state_stack , int *top , char *str , int size) ;

int _expr_cmd(int *state_stack , int *top , char *str , int size) ;
