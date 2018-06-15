/***********************************************************************************
    作者：陈铨
    日期：2007-10-31
    功能：
    版本历史：
    0.1.0  2007-10-31
    
***********************************************************************************/

#ifndef __BASE64_H
#define __BASE64_H 1

#ifdef __cplusplus
extern "C" {
#endif


extern int base64_decode_len(const unsigned char *buf , int len) ;

extern int base64_decode(const unsigned char *coded_buf , int len , unsigned char *plain_buf) ;

extern int base64_encode_len(const unsigned char *plain_buf , int len) ;

extern int base64_encode(const unsigned char *plain_buf , int len , unsigned char *coded_buf) ;

#ifdef __cplusplus
}
#endif


#endif  /* __BASE64_H */

