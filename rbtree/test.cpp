

#include "test.h"

test_node_t * build_test_nodes(int max_size)
{
    size_t alloc_size = sizeof(test_node_t) * max_size;
    test_node_t * nodes = (test_node_t *)::malloc(alloc_size) ;
    if(nodes == NULL)
        return NULL ;
    ::memset(nodes , 0 , alloc_size) ;
    return nodes ;
}

int * __test_insert_testnode(rb_root_t * root , test_node_t * node)
{
    rb_node_t ** p = &root->root ;
    rb_node_t * parent = NULL ;
    int * pkey = NULL ;
    int skey = node->key ;
    while(*p)
    {
        parent = *p ;
        pkey = &((test_node_t *)parent)->key ;

        if(skey < *pkey)
            p = &(*p)->left ;
        else if(skey > *pkey)
            p = &(*p)->right ;
        else
            return pkey ;
    }

    rb_link_node(&node->link , parent , p) ;
    return NULL ;
}

int * rb_insert_testnode(rb_root_t * root , test_node_t * node)
{
    int * ret = NULL ;
    if((ret = __test_insert_testnode(root , node)) == NULL)
        rb_insert_color(&node->link , root) ;

    return ret ;
}

int test_insert(rb_root_t * rbtree , test_node_t * nodes , int size)
{ 
    DWORD start_tick = ::GetTickCount() ;

    for(int idx = 0 ; idx < size ; ++idx)
    {
        test_node_t * node = nodes + idx ;
        node->key = idx ;    
        rb_insert_testnode(rbtree ,node) ;
    }

    DWORD end_tick = ::GetTickCount() ;

    int elapse = (int)(end_tick - start_tick) ;
    if(elapse <= 0)
        return 0 ;

    return elapse ;
}

rb_node_t * test_search(rb_root_t * rbtree , int key)
{
    rb_node_t * cur = rbtree->root ;
    while(cur != NULL)
    {
        test_node_t * node = (test_node_t *)cur ;
        if(node->key == key)
            return cur ;
        else if(node->key > key)
            cur = cur->left ;
        else
            cur = cur->right ;
    }

    return NULL ;
}

int test_erase(rb_root_t * rbtree ,int key)
{
    rb_node_t * node = test_search(rbtree , key) ;
    if(node == NULL)
        return -1 ;

    ::rb_erase(node , rbtree) ;
    return 0 ;
}

int test_find(rb_root_t * rbtree , int key)
{
    rb_node_t * node = test_search(rbtree , key) ;
    if(node == NULL)
        return -1 ;

    if(((test_node_t *)node)->key != key)
        return -2 ;
    return 0 ;
}

bool validate_node(test_node_t * node , int& cur_val , int& prev_val)
{
    prev_val = cur_val ;
    cur_val = node->key ;

    if(cur_val > prev_val) 
        return true ;
    
    ::printf("PrevValue[%d] is greater than CurrentValue[%d] \n" , prev_val , cur_val) ;
    return false ;
}

int test_validate(rb_root_t * rbtree) 
{
    rb_node_t * cur = rbtree->root ;  
    int cur_val = -1 , prev_val = -1 ;
    
    while(cur != NULL)
    {
        if(cur->left != NULL)
        {
            cur = cur->left ;
            continue ;
        }        //不再有左儿子，打印本节点
        validate_node((test_node_t *)cur , cur_val , prev_val) ;

        if(cur->right != NULL)
        {
            cur = cur->right ;
            continue ;
        }

        //即没有左儿子，也没有右儿子。右儿子向上回溯
        rb_node_t * parent = cur->parent ;
        while(parent != NULL)
        {
            if(cur == parent->left)
            {
                validate_node((test_node_t *)parent , cur_val , prev_val) ;

                if(parent->right == NULL)
                {
                    cur = parent ;
                    parent = cur->parent ;
                }
                else
                {
                    cur = parent->right ;
                    break ;
                }
            }
            else if(cur == parent->right)
            {
                cur = parent ;
                parent = cur->parent ;
            }
        }

        if(parent == NULL)
            break ;
    }

    return 0 ;
}

