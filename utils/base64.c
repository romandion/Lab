/***********************************************************************************
    作者：陈铨
    日期：2007-10-31
    功能：
    版本历史：
    0.1.0  2007-10-31
    
***********************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "base64.h"

/*
 *  这个数组是用来映射的，直接将映射手段对字符进行转化，虽然需要复杂的初始化，但是
 *  性能比较高。
 */
static const unsigned char map[256] =
{
    /* ASCII table 16 × 16 */
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 62, 64, 64, 64, 63,
    52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 64, 64, 64, 64, 64, 64,
    64,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 64, 64, 64, 64, 64,
    64, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
    41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64
};


int base64_decode_len(const unsigned char *buf , int len)
{
    int bytes ;
    register const unsigned char *in ;
    
    if(buf == NULL)
        return 0 ;
        
    in = (const unsigned char *)buf ;
    while((map[*in] <= 63) && (len >= 0))
    {
        in++ ;
        len-- ;	
    }
    
    bytes = in - (const unsigned char *)buf ;
    bytes = (bytes * 3) / 4 ;
    
    return bytes ;
}

int base64_decode(const unsigned char *coded_buf , int len , unsigned char *plain_buf)
{
    register int bytes ;
    register const unsigned char *in ;
    register unsigned char *out ;
    
    
    in = coded_buf ;
    while((map[*in] <= 63) && (len >= 0))
    {
        in++ ;
        len-- ;
    }
    
    bytes = in - coded_buf ;
    bytes = ((bytes + 3) / 4) * 3 ;
    
    out = plain_buf ;
    in = coded_buf ;

    while(bytes > 4)    
    {
        *(out++) = (unsigned char)(map[*in] << 2 | map[in[1]] >> 4) ;
        *(out++) = (unsigned char)(map[in[1] << 4] | map[in[2]] >> 2) ;
        *(out++) = (unsigned char)(map[in[2]] << 6 | map[in[3]]) ;
        
        in += 4 ;
        bytes -= 4 ;
    }
 
    //注意：如果bytes == 1 ，可能是个错误，只需简单的忽略就行了。
    if(bytes > 1)
        *(out++) = (unsigned char)(map[*in] << 2 | map[in[1]] >> 4) ;
        
    if(bytes > 2)    
        *(out++) = (unsigned char)(map[in[1]] << 4 | map[in[2]] >> 2) ;
        
    if(bytes > 3)
        *(out++) = (unsigned char)(map[in[2]] << 6 | map[in[3]]) ;
            
    bytes -= (4 - bytes) & 3 ;
    return bytes ;         
}

int base64_encode_len(const unsigned char *plain_buf , int len)
{
	return ((len + 2) / 3 * 4) + 1;	
}

static const unsigned char base64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/" ;

int base64_encode(const unsigned char *plain_buf , int len , unsigned char *coded_buf) 
{
    int i ;
    unsigned char *p ;	
    
    p = coded_buf ;
    for(i = 0 ; i < len - 2 ; i += 3 ) 
    {
        *p++ = base64[(plain_buf[i] >> 2) & 0x3F] ;
        *p++ = base64[(plain_buf[i] & 0x3) << 4 | ((int)(plain_buf[i + 1] & oxF0) >> 4)] ;
        *p++ = base64[((plain_buf[i + 1] & 0xF) << 2) | ((int)(plain_buf[i + 2] & 0xC0) >> 6)] ;
        *p++ = base64[plain_buf[i + 2] & 0x3F] ;
    }
    
    if(i < len)
    {
        *p++ = base64[(plain_buf[i] >> 2) & 0x3F] ;	
        if(i == (len - 1))
        {
            *p++ = base64[((plain_buf[i] & 0x3) << 4)] ;
            *p++ = '=' ;
        }
        else
        {
            *p++ = base64[((plain_buf[i] & 0x3) << 4) | ((int)(plain_buf[i + 1] & 0xF0) >> 4)] ;	
            *p++ = base64[((plain_buf[i + 1] & 0xF) << 2)] ;
        }
        
        *p++ = '=' ;
    }
    
    *p++ = '\0' ;
    
    return p - coded_buf ;
}




