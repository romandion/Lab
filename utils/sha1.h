/***********************************************************************************
    作者：陈铨
    日期：2007-11-1
    功能：
    版本历史：
    0.1.0  2007-11-1
    
***********************************************************************************/

#ifndef __SHA1_H
#define __SHA1_H 1

typedef struct sha1_satte_st sha1_state_t ;
struct sha1_state_st{
    unsigned int state[5] ;
    unsigned int buf[80] ;
        
    int len ;
    
    unsigned int size_hi ;
    unsigned int size_lo ;
} ;

#ifdef __cplusplus
extern "C" {
#endif


extern void sha1_init(sha1_state_t *sha1) ;

extern void sha1_append(sha1_state_t *sha1 , const unsigned char *in , int len) ;

extern void sha1_final(sha1_state_t *sha1 , unsigned char out[20]) ;

extern void sha1_hash(const unsigned char *in , int len , unsigned char out[20]) ;

#ifdef __cplusplus
}
#endif


#endif  /* __SHA1_H */

