/***********************************************************************************
    作者：陈铨
    日期：2007-11-1
    功能：
    版本历史：
    0.1.0  2007-11-1
    
***********************************************************************************/

#ifndef __MD5_H
#define __MD5_H 1

#include <stdint.h>

typedef uint8_t  md5_byte_t ;
typedef uint32_t md5_word_t ;

typedef struct md5_state_st md5_state_t ;
struct md5_state_st{
    md5_word_t count[2] ;
    md5_word_t state[4] ;
    md5_byte_t buf[64] ;
} ;


#ifdef __cplusplus
extern "C" {
#endif


extern void md5_init(md5_state_t *md5) ;

extern void md5_append(md5_state_t *md5 , const md5_byte_t *buf , unsigned int len) ;

extern void md5_final(md5_state_t *md5 , md5_byte_t digest[16]) ;

#ifdef __cplusplus
}
#endif

#endif  /* __MD5_H */

