/***********************************************************************************
    作者：陈铨
    日期：2007-11-15
    功能：类XML协议
        
    版本历史：
    0.1.0  2007-11-15        
    FF FE TYPE LENGTH...DATA LENGTH...
***********************************************************************************/

#ifndef __SXML_H
#define __SXML_H 1

#include "tree.h"

#define SXML_BORDER1         0xFF
#define SXML_BORDER2         0xFE

#define SXML_TAG_DATA        0xFD
#define SXML_TAG_PROP        0xFC
#define SXML_TAG_NS          0xFB
#define SXML_TAG_COMMENT     0xFA
#define SXML_TAG_TYPEDES     0xF9

#define SXML_TAG_NAME        0xEF


typedef struct _sxml_tag_st sxml_tag_t ;
struct _sxml_tag_st{
    unsigned int    type ;
    
    size_t          packed_size ;    
    
    size_t          size ;        
    union{
        unsigned char  *buf  ;
        unsigned char  cont[4] ;
    } data ;
} ;

typedef tree_node_t sxml_node_t ;

typedef struct _sxml_st sxml_t ;
struct _sxml_st{
    tree_t tree ;
    
    int copy ;
    
    void *packed_buf ;
    size_t packed_size ;
} ;

#define INVALID_OFFSET 0xFFFFFFFFL

#ifdef __cplusplus
extern "C" {
#endif

size_t decode_7bits(unsigned char *buf , size_t buf_len , size_t *used_len) ;

size_t encode_7bits(unsigned char *buf , size_t buf_len , size_t size) ;


sxml_t *sxml_init(sxml_t *sxml , int copy) ;

int sxml_final(sxml_t *sxml) ;


sxml_tag_t *sxml_malloc_tag(sxml_t *sxml , unsigned int type , void *buf , size_t size) ;

int sxml_free_tag(sxml_t *sxml , sxml_tag_t *tag) ;


sxml_node_t *sxml_add(sxml_t *sxml , sxml_node_t *node , unsigned int type , void *buf , size_t size) ;

sxml_node_t *sxml_addchild(sxml_t *sxml , sxml_node_t *node , unsigned int type , void *buf , size_t size) ;

sxml_node_t *sxml_addprop(sxml_t *sxml , sxml_node_t *node , unsigned int type , void *buf , size_t size) ;

int sxml_del(sxml_t *sxml , sxml_node_t *node) ;

sxml_node_t *sxml_find(sxml_t *sxml , sxml_node_t *node , unsigned int type) ;


size_t sxml_adjust_size(sxml_t *sxml , sxml_node_t *node , size_t *size) ;

size_t sxml_pack(sxml_t *sxml , void *buf , size_t size) ;

size_t sxml_parse(sxml_t *sxml , void *buf , size_t size) ;

#ifdef __cplusplus
}
#endif


#endif  /* __SXML_H */