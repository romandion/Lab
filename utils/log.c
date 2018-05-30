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

#include "log.h"

#define MAX_LOG_LINE (1024)

log_t *default_log = NULL ;

typedef enum{
    LOG_LEVEL_EMERGE ,     //危急状态，系统将无法使用
    LOG_LEVEL_ALERT ,      //紧急状态，必须马上采用行动
    LOG_LEVEL_CRITICAL ,   //严重状态，马上就会发生更加严重的错误
    LOG_LEVEL_ERROR ,      //错误状态，已经发生错误。
    LOG_LEVEL_WARNING ,    //告警状态，有极大可能会发生错误
    LOG_LEVEL_NOTICE ,     //提示状态，虽然现在正确，但还是值得注意的。
    LOG_LEVEL_INFO ,       //消息状态，没有问题，但还是需要告诉些消息
    LOG_LEVEL_DEBUG ,      //调试状态，以后这些日志将被删除，只是用来调试用的。
} log_level_t ;

static const char *level_desc[] = {
	"EMERGE" ,
	"ALERT" ,
	"CRITICAL" ,
	"ERROR" ,
	"WARNING" ,
	"NOTICE" ,
	"INFO" ,
	"DEBUG"
} ;

log_t *log_init(const char *log_file_name , log_type_t type , log_level_t level) ;
{
	log_t *log = (log_t *)malloc(sizeof(log_t)) ;
	if(log == NULL)
	    return NULL ;	
	
	memset(log , 0 , sizeof(log_t)) ;
	
	if(log_file_name == NULL)
	{
	    if(type != LOG_TYPE_CONSOLE && type != LOG_TYPE_SYSLOG)	
	    {
	        free(log) ;	
	        return NULL ;
	    }
	}
	else
	{
	    if(type == LOG_TYPE_FILE)	
	    {
	        log->file = fopen(log_file_name , "ab") ;
	        if(log->file == NULL)
	        {
	            free(log) ;	
	            return NULL ;
	        }
	    }
	}
	
	log->type = type ;
	log->level = level ;
	
    if(default_log == NULL)
        default_log = log ;
        
    return log ;
}

void log_final(log_t *log)
{
    if(log == NULL)	
        log = default_log ;
        
    if(log->file != NULL)
        fclose(log->file) ;
    log->file = NULL ;
    
    if(log == default_log)
    {
        default_log = NULL ;
        free(log) ;	
    }
}

void log_output(log_t *log , log_level_t level , int msgno , const char *file_name , int lineno , const char *format , ...)
{
    va_list vl ;
    char msg[MAX_LOG_LINE + 1] , timebuf[20] , *pos;
    time_t t ;
    struct tm _tm ;
    int size ;
    	    
    if(level < LOG_LEVEL_EMERGE)
        level = LOG_LEVEL_EMERGE ;
        
    if(level > LOG_LEVEL_DEBUG)
        level = LOG_LEVEL_DEBUG ;
    
    //没有必要输出我们不需要的日志级别，直接过滤掉
    if(log != NULL && log->level > level)
        return ;
    
    /* timestamp */
    t = time(NULL);
    localtime_r(&t , &_tm) ;
    sprintf(timebuf , "%d-%d-%d %d:%d:%d", _tm.tm_year + 1900 , _tm.tm_mon + 1 , _tm.tm_mday , _tm.tm_hour , _tm.tm_min , _tm.tm_sec) ;
    
    if(file_name != NULL)
        sprintf(msg , "file[%s] line[%d] timestamp[%s] : level[%s] , msgno[%d] --- " , file_name , lineno , timebuf , level_desc[level] , msgno) ;
    else
        sprintf(msg , "file[----] line[%d] timestamp[%s] : level[%s] , msgno[%d] --- " , lineno , timebuf , level_desc[level] , msgno) ;
    
    size = strlen(msg) ;
    pos = msg + size ;
    
    va_start(vl , format) ;    
    vsnprintf(pos , MAX_LOG_LINE - size , format , vl);
    va_end(vl);

    
    if(log == NULL)
        log = default_log ;
        
    if(log == NULL || log->type == LOG_TYPE_CONSOLE || log->file == NULL)
        printf("%s \n" , msg) ;
    else
    {
    	if(log->type == LOG_TYPE_FILE && log->file != NULL)
    	    fprintf(log->file , "%s" , msg) ;
    	else
            syslog(level , "%s" , msg) ;
    }
}

