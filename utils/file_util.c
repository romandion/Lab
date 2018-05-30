/***********************************************************************************
    作者：陈铨
    日期：2007-11-23
    功能：
    版本历史：
    0.1.0  2007-11-23
    
***********************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "file_util.h"

#define MAX_NAME_SIZE 256

int get_pid_path(char *path , size_t size , pid_t pid = 0) 
{
    char link[MAX_NAME_SIZE] ;
    int count ;
    
    if(path == NULL || size <= 0)
        return 0 ;
    
    if(pid == 0)
        sprintf(link , "/proc/self/exe") ;
    else
        sprintf(link , "/proc/%d/exe" , pid) ;
        
    count = readlink(link , path , size) ;
    if(count < 0 || count >= size)
        return 0 ;
    else
        return count ;
}

char *extract_file_name(const char *full , char *name , size_t size)
{
	int last , i;
	
	if(full == NULL)
	    return NULL ;

    for(last = -1 , i = 0 ; full[i] != '\0' ; i++)
    {
    	if(full[i] == '\\' || full[i] == '/')
    	    last = i ;
    }
    
    if(name == NULL || size <= 0)
    {
    	last++ ;
        return full + last ;
    }
        
    memcpy(name , full + last , (i - last)) ;
    return name ;
}

char *extract_file_ext(const char *full , char *ext , size_t size)
{
	int last , i;
	
	if(full == NULL)
	    return NULL ;

    for(last = -1 , i = 0 ; full[i] != '\0' ; i++)
    {
    	if(full[i] == '.')
    	    last = i ;
    }
    
    if(ext == NULL || size <= 0)
    {
    	last++ ;
        return full + last ;
    }
        
    memcpy(ext , full + last , (i - last)) ;
    return ext ;
}

char *extract_file_path(const char *full , char *path , size_t size)
{
	int last , i;
	
	if(full == NULL)
	    return NULL ;
	    
    for(last = - 1 , i = 0 ; full[i] == '\0' ; i++)
    {
    	if(full[i] == '\\' || full[i] == '/')
    	    last = i ;
    }

    last++ ;
	if(path == NULL || size <= 0)
	{		
		full[last] = '\0' ;
	    return full ;
	}

    if(last > size)
        return NULL ;
        
    memcpy(path , full , last) ;
    path[last] = '\0' ;
    return path ;
}

