/***********************************************************************************
    作者：陈铨
    日期：2007-10-31
    功能：
    版本历史：
    0.1.0  2007-10-31
    
***********************************************************************************/

#ifndef __HEX_H
#define __HEX_H 1


#ifdef __cplusplus
extern "C" {
#endif

// 2007-11-1 , by chenquan 将原始二进制流转化为十六进制字符串
int raw_to_hex(const void *in, int len, char *out);

// 2007-11-1 , by chenquan 将十六进制字符串转化为原始二进制流
int hex_to_raw(const void *in, int len, char *out);

// 2007-11-1 , by chenquan 将整型转化为十六进制字符串
int int_to_hex(int in , char *out) ;

// 2007-11-1 , by chenquan 将十六进制字符串转化为整型
int hex_to_int(const void *in , int len) ;

#ifdef __cplusplus
}
#endif


#endif  /** __HEX_H */