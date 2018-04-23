
#ifndef __TEST_H
#define __TEST_H 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

typedef bool (*test_case)() ;

bool test_compare_exchange() ;
bool test_increment() ;
bool test_decrement() ;


static const int kTestCaseSize = 3 ;
static test_case __test_cases__[kTestCaseSize] = {
    test_compare_exchange ,
    test_increment ,
    test_decrement
} ;

#endif /**__TEST_H*/
