/***********************************************************************************
    作者：陈铨
    日期：2007-10-31
    功能：
    版本历史：
    0.1.0  2007-10-31
      exec [-cmd] [-cmd para] [--para=value] para
      
      expr [-v] [-h] [-f filename] [--filename=filename] filename
***********************************************************************************/

#ifndef __EXPR_H
#define __EXPR_H 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OPTION_L '['
#define OPTION_R ']'
#define OPTION_T ' '
#define PARAM_EQ '='
#define CMD_FLAG '-'


#define ARGV_TYPE_UNKNOWN     0
#define ARGV_TYPE_INTEGRANT   1  //必须的
#define AEGV_TYPE_OPTIONAL    2  //可选的

#ifdef __cplusplus
extern "C"{
#endif

typedef struct argv_st argv_t ;

typedef struct expr_st expr_t ;

extern expr_t *expr_init(expr_t *expr , char *str) ;

extern argv_t *expr_find_cmd(expr_t *expr , const char *cmd) ;

extern argv_t *expr_iter(expr_t *expr) ;

extern int expr_cmd_count(expr_t *expr) ;

extern void expr_final(expr_t *expr) ;

extern char *expr_argv_cmd(expr_t *expr , argv_t *argv) ;

extern char *expr_argv_name(expr_t *expr , argv_t *argv) ;

extern int expr_argv_type(expr_t *expr , argv_t *argv) ;

extern int expr_argv_index(expr_t *expr , argv_t *argv) ;


#ifdef __cplusplus
}
#endif


#endif  /** __EXPR_H */

