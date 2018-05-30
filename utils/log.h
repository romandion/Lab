/***********************************************************************************
    作者：陈铨
    日期：2007-10-31
    功能：
    版本历史：
    0.1.0  2007-10-31
    
***********************************************************************************/

#ifndef __LOG_H
#define __LOG_H 1

#include <stdio.h>
#include <stdlib.h>


#ifdef __cplusplus
extern "C" {
#endif


/*
 *  日志输出类型
 */
typedef enum{
    LOG_TYPE_CONSOLE ,
    LOG_TYPE_SYSLOG ,
    LOG_TYPE_FILE ,    
} log_type_t ;

/*
 *  日志级别
 */
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

typedef struct _log_st log_t ;

struct _log_st{
    log_type_t   type ;    //日志类型
    log_level_t  level ;   //默认的日志级别
    FILE*        file ;    //日志输出目的地
} ;


extern log_t *default_log ;

extern log_t *log_init(const char *log_file_name , log_type_t type = LOG_TYPE_CONSOLE , log_level_t level = LOG_LEVEL_WARNING) ;

extern void log_final(log_t *log = NULL) ;

extern void log_output(log_t *log , log_level_t level , int msgno , const char *file_name , int lineno , const char *format , ...) ;

#define OUTPUT(level , msgno , ...) log_output(default_log , level , msgno , __FILE__ , __LINE__ , __VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif    /* __LOG_H */

