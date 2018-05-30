/***********************************************************************************
    作者：陈铨
    日期：2007-11-13
    功能：
    版本历史：
    1.1.0  2007-11-13
        0.  expr    ::= (space)*((scheme) (space)+)*(scheme)?(space)*
        1.  scheme  ::= (option)| (integr)
        2.  option  ::= '[' (cmd1) | (cmd2) ']'
        3.  integr  ::= (cmd1) | (cmd2) | (name)
        4.  cmd1    ::= '-'((char) ((space)+ (name))?
        5.  cmd2    ::= '--'((char) | '_')+ (space)* ('=' (space)* (name))?
        7.  name    ::= ([a-z]|[A-Z]|'_')+ (char|'_')*
        8.  char    ::= [a-z]|[A-Z]|[0-9]
        9.  space   ::= #32
        
        
        " [-v] [--version] [ -t time ] [--file = file_name ] -h -s -o output --name = app integrant"
    
***********************************************************************************/

#ifndef __EXPR_H
#define __EXPR_H 1


#define OPTION_L '['
#define OPTION_R ']'
#define CMD_FLAG '-'
#define SPACE    ' '
#define CMD2_EQ  '='

#define EXPT_TYPE_MASK       0xFFFF0000L
#define EXPR_TYPE_UNKOWN     0x00000000L
#define EXPR_TYPE_EXPR       0x00010000L
#define EXPR_TYPE_SCHEME     0x00020000L
#define EXPR_TYPE_OPTION     0x00030000L
#define EXPR_TYPE_INTEGR     0x00040000L
#define EXPR_TYPE_CMD1       0x00050000L
#define EXPR_TYPE_CMD2       0x00060000L
#define EXPR_TYPE_NAME       0x00070000L
#define EXPR_TYPE_CHAR       0x00080000L
#define EXPR_TYPE_SPACE      0x00090000L
#define EXPR_TYPE(type) (type & EXPR_TYPE_MASK)

#define EXPR_STATE_MASK      0x0000FFFFL
#define EXPR_STATE_UNKNOWN   0x00000000L
#define EXPR_STATE_EXPECT    0x00000001L
#define EXPR_STATE_FINISH    0x00000002L
#define EXPR_STATE_ERROR     0x00000004L
#define EXPR_STATE(state) (state & EXPR_STATE_MASK)

#define MAX_STACK_SIZE 128

typedef int (*expr_action_t)(int *state_stack , int *top , char *str , int size) ;

extern char expr_maps[1 << sizeof(char)] ;

extern action_t expr_actions[32] ;

typedef void (*expr_handler_t)(int state , char *buf , int size) ;

extern int expr_parse(char *expr , int size , expr_handler_t handler) ;

#endif    /** __EXPR_H */
