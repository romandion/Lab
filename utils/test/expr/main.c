#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "expr.h"


int main(int argc , char *argv[])
{
    int i , len ;
    char expr_buf[1024] = {0} , *str = expr_buf ;
    
    expr_t *expr ;
    argv_t *arg ;
    
    for(i = 1 ; i < argc ; i++)
    {
        len = sprintf(str , "%s " , argv[i]) ;
        if(len <= 0)
        {
        	printf("sprintf failed , len = %d , argv[%d] = %s \n" , len , i , argv[i]) ;
            return -1 ;
        }
        
        str += len ;
    }
    
    expr = expr_init(NULL , expr_buf) ;
    if(expr == NULL)
    {
        printf("failed init expr \n") ;	
        return -1 ;
    }
    
    printf("expr cmd count = %d \n" , expr_cmd_count(expr)) ;
    
    while((arg = expr_iter(expr)) != NULL)
    {
        printf("type[%d] , index[%d] , cmd[%s] , name[%s] \n" , expr_argv_type(expr , arg) , expr_argv_index(expr , arg) , 
            expr_argv_cmd(expr , arg) , expr_argv_name(expr , arg)) ;	
    }
    
    expr_final(expr) ;
    
    return 0 ;	
}


