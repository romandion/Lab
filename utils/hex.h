/***********************************************************************************
    ���ߣ�����
    ���ڣ�2007-10-31
    ���ܣ�
    �汾��ʷ��
    0.1.0  2007-10-31
    
***********************************************************************************/

#ifndef __HEX_H
#define __HEX_H 1


#ifdef __cplusplus
extern "C" {
#endif

// 2007-11-1 , by chenquan ��ԭʼ��������ת��Ϊʮ�������ַ���
int raw_to_hex(const void *in, int len, char *out);

// 2007-11-1 , by chenquan ��ʮ�������ַ���ת��Ϊԭʼ��������
int hex_to_raw(const void *in, int len, char *out);

// 2007-11-1 , by chenquan ������ת��Ϊʮ�������ַ���
int int_to_hex(int in , char *out) ;

// 2007-11-1 , by chenquan ��ʮ�������ַ���ת��Ϊ����
int hex_to_int(const void *in , int len) ;

#ifdef __cplusplus
}
#endif


#endif  /** __HEX_H */