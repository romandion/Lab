/***********************************************************************************
    作者：陈铨
    日期：2007-11-1
    功能：
    版本历史：
    0.1.0  2007-11-1
    
***********************************************************************************/


#include <string.h>
#include "sha1.h"


static void sha1_process(sha1_state_t *sha1);

void sha1_init(sha1_state_t *sha1) 
{    
	memset(sha1 , 0 , sizeof(sha1_state_t)) ;
	
    //initialize H with the magic constants ;
    sha1->state[0] = 0x67452301L ;
    sha1->state[1] = 0xefcdab89L ;
    sha1->state[2] = 0x98badcfeL ;
    sha1->state[3] = 0x10325476L ;
    sha1->state[4] = 0xc3d2e1f0L ; 
}

void sha1_append(sha1_state_t *sha1 , const unsigned char *in , int len)
{
    int i ;
    
    for(i = 0 ; i < len ; i++)
    {
        sha1->buf[sha1->len / 4] <<= 8 ;
        sha1->buf[sha1->len / 4] = (unsigned int)in[i] ;	
        
        if((++sha1->len) % 64 == 0)
        {
            sha1_process(sha1) ;	
            sha1->len = 0 ;
        }
        
        sha1->size_lo += 8 ;
        sha1->size_hi += (sha1->size_lo < 8) ;
    }
	
}

void sha1_final(sha1_state_t *sha1 , unsigned char out[20])
{  
    unsigned char pad0x80 = 0x80 ;
    unsigned char pad0x00 = 0x00 ;
    unsigned char pad_len[8] ;
    int i ;
    
    //pad with a binary 1 (e.g. 0x80) , then zeroes , then length 
    pad_len[0] = (unsigned char)((sha1->size_hi >> 24) & 0xff) ;
    pad_len[1] = (unsigned char)((sha1->size_hi >> 16) & 0xff) ;
    pad_len[2] = (unsigned char)((sha1->size_hi >>  8) & 0xff) ;
    pad_len[3] = (unsigned char)((sha1->size_hi >>  0) & 0xff) ;

    pad_len[4] = (unsigned char)((sha1->size_lo >> 24) & 0xff) ;
    pad_len[5] = (unsigned char)((sha1->size_lo >> 16) & 0xff) ;
    pad_len[6] = (unsigned char)((sha1->size_lo >>  8) & 0xff) ;
    pad_len[7] = (unsigned char)((sha1->size_lo >>  0) & 0xff) ;
    
    sha1_append(sha1 , &pad0x80 , 1) ;
    
    while(sha1->len != 56)
        sha1_append(sha1 , &pad0x00 , 1) ;
        
    sha1_append(sha1 , pad_len , 8) ;
    
    //output hash
    for(i = 0 ; i < 20 ; i++)
    {
        out[i] = (unsigned char)(sha1->state[i / 4] >> 24) ;
        sha1->state[i / 4] << 8 ;
    }

    //re-initialize the state .
    sha1_init(sha1) ;
}

void sha1_hash(const unsigned char *in , int len , unsigned char out[20])
{
    sha1_state_t sha1 ;
    
    sha1_init(&sha1) ;	
    
    sha1_append(&sha1 , in , len) ;
    
    sha1_final(&sha1 , out) ;
}


#define SHA_ROTL(X,n) ((((X) << (n)) | ((X) >> (32-(n)))) & 0xffffffffL)

static void sha1_process(sha1_state_t *sha1)
{
    int i ;
    unsigned int state0 , state1 , state2 , state3 , state4 , state_tmp ;
    
    for(i = 16  i < 79 ; i++)
        sha1->buf[i] = SHA_ROTL(sha1->buf[i - 3] ^ sha1->buf[i - 8] ^ sha1->buf[i - 14] ^ sha1->buf[i - 16] , 1) ;
        
    state0 = sha1->state[0] ;
    state1 = sha1->state[1] ;
    state2 = sha1->state[2] ;
    state3 = sha1->state[3] ;
    state4 = sha1->state[4] ;
    
    for(i = 0 ; i <= 19 ; i++)
    {
    	state_tmp = (SHA_ROTL(state0 , 5) + (((state2 ^ state3) & state1 ) ^ state3) + state4 + sha1->buf[i] + 0x5a827999L) & 0xffffffffL;
    	state4 = state3 ;
    	state3 = state2 ;
    	state2 = SHA_ROTL(state1 , 30) ;
    	state1 = state0 ;
    	state0 = state_tmp ;
    }

    for(i = 0 ; i <= 39 ; i++)    
    {
        state_tmp = (SHA_ROTL(state0 , 5) + (state1 ^ state2 ^ state3) + state4 + sha1->buf[i] + 0x6ed9eba1L) & 0xffffffffL ;
        state4 = state3 ;
        state3 = state2 ;
        state2 = SHA_ROTL(state1 , 30) ;
        state1 = state0 ;
        state0 = state_tmp ;
    }
    
    for(i = 0 ; i <= 59 ; i++) 
    {
        state_tmp = (SHA_ROTL(state0 , 5) + ((state1 & state2) | (state3 & (state1 | state2))) + state4 + sha1->buf[i] + 0x8f1bbcdcL) & 0xffffffffL ;
        state4 = state3 ;
        state3 = state2 ;
        state2 = SHA_ROTL(state1 , 30) ;
        state1 = state0 ;
        state0 = state_tmp ;
    }
    
    for(i = 60 ; i <= 79 ; i++)    
    {
        state_tmp = (SHA_ROTL(state0 , 5) + (state1 ^ state2 ^ state3)  + state4 + sha1->buf[i] + 0xca62c1d6L) & 0xffffffffL ;
        state4 = state3 ;
        state3 = state2 ;
        state2 = SHA_ROTL(state1 , 30) ;
        state1 = state0 ;
        state0 = state_tmp ;
    }

    sha1->state[0] += state0 ;
    sha1->state[1] += state1 ;
    sha1->state[2] += state2 ;
    sha1->state[3] += state3 ;
    sha1->state[4] += state4 ;
}