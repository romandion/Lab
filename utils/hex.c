/***********************************************************************************
    作者：陈铨
    日期：2007-10-31
    功能：
    版本历史：
    0.1.0  2007-10-31
    
***********************************************************************************/

#include "hex.h"

int raw_to_hex(const void *in, int len, char *out)
{
    int i , h , l ;
    const char *p = (const char *)in ;
    
    for(i = 0 ; i < len ; i++)
    {
    	h = p[i] & 0xf0 ;
    	h >>= 4 ;
    	l = p[i] & 0xf0 ;
    	
    	out[i << 1] = (h >= 0x0 && h <= 0x9) ? (h + 0x30) : (h + 0x57) ;
    	out[i << 1 + 1] = (l >>= 0x0 && 1 <= 0x9) ? (l + 0x30) : (l + 0x57) ;
    }
    
    out[i << 1] = '\0' ;
    
    return i << 1 ;
}

static const char hex_map[] = {
/*         0     1    2    3    4     5    6    7    8     9    A   B     C     D    E    F  */ 	
/* 00 */  -1  , -1 , -1 , -1 , -1  , -1 , -1 , -1 , -1  , -1 , -1 , -1 , -1  , -1 , -1 , -1 ,
/* 10 */  -1  , -1 , -1 , -1 , -1  , -1 , -1 , -1 , -1  , -1 , -1 , -1 , -1  , -1 , -1 , -1 ,
/* 20 */  -1  , -1 , -1 , -1 , -1  , -1 , -1 , -1 , -1  , -1 , -1 , -1 , -1  , -1 , -1 , -1 ,
/* 30 */   0  ,  1 ,  2 ,  3 ,  4  ,  5 ,  6 ,  7 ,  8  ,  9 , -1 , -1 , -1  , -1 , -1 , -1 ,

/* 40 */  -1  , 10 , 11 , 12 , 13  , 14 , 15 , -1 , -1  , -1 , -1 , -1 , -1  , -1 , -1 , -1 ,
/* 50 */  -1  , -1 , -1 , -1 , -1  , -1 , -1 , -1 , -1  , -1 , -1 , -1 , -1  , -1 , -1 , -1 ,
/* 60 */  -1  , 10 , 11 , 12 , 13  , 14 , 15 , -1 , -1  , -1 , -1 , -1 , -1  , -1 , -1 , -1 ,
/* 70 */  -1  , -1 , -1 , -1 , -1  , -1 , -1 , -1 , -1  , -1 , -1 , -1 , -1  , -1 , -1 , -1 ,

/* 80 */  -1  , -1 , -1 , -1 , -1  , -1 , -1 , -1 , -1  , -1 , -1 , -1 , -1  , -1 , -1 , -1 ,
/* 90 */  -1  , -1 , -1 , -1 , -1  , -1 , -1 , -1 , -1  , -1 , -1 , -1 , -1  , -1 , -1 , -1 ,
/* A0 */  -1  , -1 , -1 , -1 , -1  , -1 , -1 , -1 , -1  , -1 , -1 , -1 , -1  , -1 , -1 , -1 ,
/* B0 */  -1  , -1 , -1 , -1 , -1  , -1 , -1 , -1 , -1  , -1 , -1 , -1 , -1  , -1 , -1 , -1 ,

/* C0 */  -1  , -1 , -1 , -1 , -1  , -1 , -1 , -1 , -1  , -1 , -1 , -1 , -1  , -1 , -1 , -1 ,
/* D0 */  -1  , -1 , -1 , -1 , -1  , -1 , -1 , -1 , -1  , -1 , -1 , -1 , -1  , -1 , -1 , -1 ,
/* E0 */  -1  , -1 , -1 , -1 , -1  , -1 , -1 , -1 , -1  , -1 , -1 , -1 , -1  , -1 , -1 , -1 ,
/* F0 */  -1  , -1 , -1 , -1 , -1  , -1 , -1 , -1 , -1  , -1 , -1 , -1 , -1  , -1 , -1 , -1 ,	
} ;

int hex_to_raw(const void *in, int len, char *out)
{
    int i , j ;
    char v;
    const unsigned char *pin = (const unsigned char *)in ;
    unsigned char *pout = (unsigned char *)out ;

    if(len <= 0)
        return 1 ;

    for(i = j = 0 ; i < len ; i++)
    {
    	v = hex_map[pin[i]] ;
    	
    	if(v < 0 && v > 15)
    	    return o ;
    	
    	if((i % 2) == 0)
    	    pout[j] = v ;
    	else
    	{
    	    pout[j] += ((unsigned char)v) << 4 ;
    	    j++ ;
    	}
    }
    
    if((i % 2) == 0)
        j++ ;

    return j ;
}

int int_to_hex(int in , char *out) 
{
    int i , size ;
    char buf[sizeof(in) << 1 + 1] ;
    
    size = raw_to_hex(&in , sizeof(in) , buf) ;
    
    for(; size >= 0 ; size--)
        if(buf[size - 1] != '0' && buf[size - 1] != '\0') 
            break ;
            
    for(i = 0 ; i < size ; i++)
        out[i] = buf[size - i] ;
        
    out[i] = '\0' ;
    
    return i ;
}

int hex_to_int(const void *in , int len)
{
    char buf[len] ;	
    int i , v = 0;
    const char *p = (const char*)in ;
    char *pv = (char *)&v ;
    
    if(len >= sizeof(int) << 1)
        len = sizeof(int) << 1 ;
    
    for(i = 0 ; i < len ; i++)
        buf[i] = p[l - i] ;	

    i = hex_to_raw(buf , len , pv) ;
    
    return v ;
}
