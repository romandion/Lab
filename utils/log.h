/***********************************************************************************
    ���ߣ�����
    ���ڣ�2007-10-31
    ���ܣ�
    �汾��ʷ��
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
 *  ��־�������
 */
typedef enum{
    LOG_TYPE_CONSOLE ,
    LOG_TYPE_SYSLOG ,
    LOG_TYPE_FILE ,    
} log_type_t ;

/*
 *  ��־����
 */
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

typedef struct _log_st log_t ;

struct _log_st{
    log_type_t   type ;    //��־����
    log_level_t  level ;   //Ĭ�ϵ���־����
    FILE*        file ;    //��־���Ŀ�ĵ�
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

