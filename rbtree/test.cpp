

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

int test_insert(rb_root_t * rbtree)
{ 
    test_node_t * nodes = build_test_nodes(kMaxSize) ;

    DWORD start_tick = ::GetTickCount() ;

    for(int idx = 0 ; idx < kMaxSize ; ++idx)
    {
        test_node_t * node = nodes + idx ;
        node->key = idx ;

        //::rb_insert_color(&node->link , rbtree) ;    
        rb_insert_testnode(rbtree ,node) ;
    }

    DWORD end_tick = ::GetTickCount() ;

    test_validate(rbtree) ;

    int elapse = (int)(end_tick - start_tick) ;
    if(elapse <= 0)
        return 0 ;

    return elapse ;
}

int test_erase(rb_root_t * rbtree)
{
    return 0 ;
}

int test_find(rb_root_t * rbtree)
{
    return 0 ;
}

int test_validate(rb_root_t * rbtree) 
{
    test_node_t * nodes[128] ;
    ::memset(nodes , 0 , sizeof(nodes)) ;
    int level = 0 ;

    rb_node_t * root = rbtree->root ;
    nodes[level] = (test_node_t *)root ;
    int index = 0 ;

    while(level >= 0)
    {
        rb_node_t * cur = (rb_node_t *)nodes[level] ;
        if(cur == NULL)
        {
            --level ;
            continue ;
        }

        rb_node_t * child = (rb_node_t *)nodes[level + 1] ;
        if(child == NULL)
        {
            if(cur->left != NULL)
            {
                ++level ;
                nodes[level] = (test_node_t *)cur->left ;
                continue ;
            }
            else if(cur->right != NULL)
            {
                ++level ;
                nodes[level] = (test_node_t *)cur->right ;
                continue ;
            }
            else
            {
                //不再有子节点了，本节点就是叶子节点
                ++index ;
                --level ;
                printf("[%08d] = [%08d] \n" , index , ((test_node_t *)cur)->key) ;
            }
        }
        else if(child == cur->left)
        {
            if(cur->right != NULL)
            {
                ++level ;
                nodes[level] = (test_node_t *)cur->right ;
            }
            else
                --level ;
            continue ;
        }
        else if(child == cur->right)
        {
            --level ;
            continue ;
        }
        else
        {
            //跳跃到其他分支去，则先深度遍历
            nodes[level + 1] = NULL ;
            printf("[%08d] = [%08d] \n" , index , ((test_node_t *)cur)->key) ;
        }
    }


    return index ;
}

