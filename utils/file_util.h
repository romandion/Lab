/***********************************************************************************
    作者：陈铨
    日期：2007-11-23
    功能：
    版本历史：
    0.1.0  2007-11-23
    
***********************************************************************************/

#ifndef __FILE_UTIL_H
#define __FILE_UTIL_H 1

#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif

int get_pid_path(char *path , size_t size , pid_t pid = 0) ;

char *extract_file_name(const char *full , char *name , size_t size) ;

char *extract_file_ext(const char *full , char *ext , size_t size) ;

char *extract_file_path(const char *full , char *path , size_t size) ;

#ifdef __cplusplus
}
#endif


#endif    /* __FILE_UTIL_H */