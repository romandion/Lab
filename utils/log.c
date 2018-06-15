/***********************************************************************************
    ���ߣ�����
    ���ڣ�2007-10-31
    ���ܣ�
    �汾��ʷ��
    0.1.0  2007-10-31
    
***********************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "log.h"

#define MAX_LOG_LINE (1024)

log_t *default_log = NULL ;

typedef enum{
    LOG_LEVEL_EMERGE ,     //Σ��״̬��ϵͳ���޷�ʹ��
    LOG_LEVEL_ALERT ,      //����״̬���������ϲ����ж�
    LOG_LEVEL_CRITICAL ,   //����״̬�����Ͼͻᷢ���������صĴ���
    LOG_LEVEL_ERROR ,      //����״̬���Ѿ���������
    LOG_LEVEL_WARNING ,    //�澯״̬���м�����ܻᷢ������
    LOG_LEVEL_NOTICE ,     //��ʾ״̬����Ȼ������ȷ��������ֵ��ע��ġ�
    LOG_LEVEL_INFO ,       //��Ϣ״̬��û�����⣬��������Ҫ����Щ��Ϣ
    LOG_LEVEL_DEBUG ,      //����״̬���Ժ���Щ��־����ɾ����ֻ�����������õġ�
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
    
    //û�б�Ҫ������ǲ���Ҫ����־����ֱ�ӹ��˵�
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

