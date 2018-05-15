
#ifndef __TEST_H
#define __TEST_H 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include "rbtree.h"

typedef struct __st_test_node{
    rb_node_t link ;
    int key ;
} test_node_t ;

static const int kMaxSize = 5000000 ;

test_node_t * build_test_nodes(int max_size) ;

int test_insert(rb_root_t * rbtree , test_node_t * nodes , int size) ;

int test_erase(rb_root_t * rbtree , int key) ;

int test_find(rb_root_t * rbtree , int key) ;

//ÖÐÐò±éÀú
int test_validate(rb_root_t * rbtree) ;


#include <set>

int test_set_insert() ;
int test_set_erase() ;
int test_set_find() ;


#endif /** __TEST_H */
