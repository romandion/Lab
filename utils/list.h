/***********************************************************************************
    作者：陈铨
    日期：2007-11-02
    功能：
    版本历史：
    0.1.0  2007-11-02
    
***********************************************************************************/

#ifndef __LIST_H
#define __LIST_H 1

typedef void * (* malloc_t)(size_t size) ;
typedef void (* free_t)(void *mem , size_t size) ;
typedef int (* compare_t)(void *src , size_t size_src , void *dest , size_t size_dest) ;

typedef struct list_node_st list_node_t ;

struct list_node_st{
    void *value ;    
    size_t size ;
    
    list_t *prev ;
    list_t *next ;
} ;

typedef struct list_st list_t ;
struct list_st {
	list_node_t *head ;
	
	list_node_t *tail ;
	
	int count ;
	
	bool copy ;
	
	malloc_t malloc_fun ;
	
	free_t free_fun ;	
	
	compare_t compare_fun ;
} ;

#ifdef __cplusplus
extern "C" {
#endif

extern list_t *list_init(list_t *list , malloc_t malloc_fun = NULL , free_t free_fun = NULL , compare_t compare_fun = NULL , bool copy = false) ;

extern void list_final(list_t *list) ;

extern int list_insert(list_t *list , list_node_t *node , void *value , size_t size) ;

extern int list_delete(list_t *list , list_node_t *node) ;

extern int list_push(list_t *list , void *value , size_t size) ;

extern list_node_t *list_pop(list_t *list) ;

extern list_node_t *list_find(list_t *list , void *value , size_t size) ;

extern list_node_t *list_iterator(list_t *list , list_node_t *node , void *value , size_t size) ;

extern list_node_t *list_get(list_t *list , int index) ;

#ifdef __cplusplus
}
#endif

#endif  /* __LIST_H */
