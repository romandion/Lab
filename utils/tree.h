/***********************************************************************************
    作者：陈铨
    日期：2007-11-16
    功能：类XML协议
        
    版本历史：
    0.1.0  2007-11-16
    
***********************************************************************************/

#ifndef __TREE_H
#define __TREE_H  1


typedef struct _tree_node_st tree_node_t ;
struct _tree_node_st{

    void    *buf ;
    size_t  size ;
    
    tree_node_t *child ;
    tree_node_t *parent ;
    tree_node_t *next ;
    tree_node_t *prev ;    
} ;


typedef void *(*malloc_t)(size_t size) ;
typedef void (*free_t)(void *buf) ;

typedef struct _tree_st tree_t ;
struct _tree_st{
    tree_node_t *root ;
    
    int  copy ;    
    int  count ;
    
    malloc_t malloc ;
    free_t   free ;
    
    int max_branch ;    // 0 不限制
} ;

typedef int (*travel_process_t)(tree_t *tree , tree_node_t *node , void *arg) ;

#define TREE_TRAVEL_FIRST_ROOT    0xF0
#define TREE_TRAVEL_MIDDLE_ROOT   0xF1
#define TREE_TRAVEL_LAST_ROOT     0xF2

#define TREE_TRAVEL_MAX_LEVEL     1024

#ifdef __cplusplus
extern "C" {
#endif

tree_t *tree_init(tree_t *tree , int copy = false, malloc_t malloc_fun = NULL , free_t free_fun = NULL , int max_branch = 0) ;

void tree_final(tree_t *tree) ;

tree_node_t *tree_insert(tree_t *tree , tree_node_t *node , void *buf , size_t size) ;

tree_node_t *tree_add(tee_t *tree , tree_node_t *node , void *buf , size_t size) ;

tree_node_t *tree_addchild(tee_t *tree , tree_node_t *node , void *buf , size_t size) ;

void tree_del(tee_t *tree , tree_node_t *node) ;

tree_node_t *tree_find(tree_t *tree , tree_node_t *node , void *buf , size_t size , int part = 0 , int offset = 0) ;

tree_node_t *tree_childs(tree_t *tree , tree_node_t *node , int index) ;

tree_node_t *tree_malloc_node(tree_t *tree) ;

void tree_free_node(tree_t *tree , tree_node_t *node);

int tree_node_copy(tree_t *tree , tree_node_t *dest , tree_node_t *src) ;

int tree_node_assign(tree_t *tree , tree_node_t *node , void *buf , size_t size) ;

int tree_travel(tree_t *tree , tree_node_t *node , travel_process_t process , int travel_type = TREE_TRAVEL_MIDDLE_ROOT , void *arg = NULL) ;

#ifdef __cplusplus
}
#endif


#endif  /* __TREE_H */

