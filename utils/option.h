/***********************************************************************************
    ���ߣ�����
    ���ڣ�2007-10-31
    ���ܣ�֧������3�ֲ���ָ����ʽ���Կո����ֲ�ͬ����
        1. '-item value' ���� '-item'
        2. '--item = value'
        3. 'item'
        
    �汾��ʷ��
    0.1.0  2007-10-31
    
    
***********************************************************************************/

#ifndef __OPTION_H
#define __OPTION_H 1


#define MAX_OPTION_COUNT 128
#define MAX_BUFFER_SIZE  8192 

typedef int (*option_handler_t)(const char *item , const char *value) ;

typedef struct option_st option_t ;
struct option_st{

    char *argvs[MAX_OPTION_COUNT] ;
    int argc ;
    
    char *items[MAX_OPTION_COUT] ;
    char *values[MAX_OPTION_COUT] ;
    int item_count ;
    int item_index ;
    
    char *unhandles[MAX_OPTION_COUNT] ;
    int unhandle_count ;
    int unhandle_index ;
    
    char *handler_key[MAX_OPTION_COUNT];
    option_handler_t handlers[MAX_OPTION_COUNT] ;    
    int handler_count ;  
    
    char buffer[MAX_BUFFER_SIZE] ;
    int buf_size ;
} ;

#ifdef __cplusplus
extern "C" {
#endif

extern option_t *option_init(option_t *option , int argc , char *argv[]) ;

extern int option_register_handler(option_t *option , const char *item , option_handler_t handler) ;

extern int option_next_item(option_t *option , const char **item , const char **value) ;

extern int option_next_unhandle(option_t *option , const char **unhandle) ;

extern int option_find_item(option_t *option , const char *item , const char **value) ;

extern void option_final(option_t *option) ;

extern int get_option(int argc , char *argv[] , char **item , char **value) ;

#ifdef __cplusplus
}
#endif

#endif /** __OPTION_H */
