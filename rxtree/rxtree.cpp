
#include "rxtree.h"

bool rx_tree_init(rx_tree_t * tree , uint8_t * bits , uint32_t level , void (*value_free)(uintptr_t value))
{
    if(tree == NULL || bits == NULL || level > sizeof(uintptr_t))
        return false ;

    ::memset(tree , 0 , sizeof(rx_tree_t)) ;
    ::memcpy(tree->bits , bits , level) ;
    tree->level = level ;
    tree->free = value_free ;

    return true ;
}

void rx_tree_final(rx_tree_t * tree)
{
    if(tree == NULL)
        return ;

    size_t sizes[sizeof(uintptr_t)] ;
    ::memset(sizes , 0 , sizeof(sizes)) ;
    uint32_t level = tree->level ;

    for(uint32_t idx = 0 ; idx < level ; ++idx)
    {
        sizes[idx] = 1 << (tree->bits[idx]) ;
    }

    rx_node_t *nodes[256] ;
    ::memset(nodes , 0 , sizeof(nodes)) ;

    int lidx = 0 , max_level = level - 1;
    nodes[lidx] = tree->root.childs ;

    while(lidx >= 0)
    {
        rx_node_t * cur = nodes[lidx] ;
        if(cur == NULL)
        {
            --lidx ;
            continue ;
        }

        if(lidx >= max_level)
        {
            //最后一个，是value
            if(tree->free != NULL)
                tree->free(cur->value) ;
            --lidx ;
            continue ;
        }

        if(cur->childs != NULL)
        {
            ++lidx ;
            nodes[lidx] = cur->childs ;
            continue ;
        }

        rx_node_t * next = cur->left ;
        if(next != NULL)
        {
            nodes[lidx] = next ;
        }
        else
        {
            //向上一级
            --lidx ;
            if(lidx >= 0)
            {
                rx_node_t * parent = nodes[lidx] ;
                parent->childs = NULL ;
            }
        }

        if(cur->nodes != NULL)
        {
            cur->nodes = NULL ;
            ::free(cur->nodes) ;
        }
    }
}

bool rx_tree_insert(rx_tree_t * tree , uint32_t key , uintptr_t value)
{
    if(tree == NULL)
        return false ;

    rx_node_t * parent = &tree->root ;
    uint32_t level = tree->level ;
    for(uint32_t lidx = 0 ; lidx < level ; ++lidx)
    {
        uint8_t bits = tree->bits[lidx] ;
        uint32_t offset = key & ((1 << bits) - 1) ; 

        if(parent->nodes == NULL)
        {
            size_t size = size_page_align(sizeof(rx_node_t) << bits) ;
            parent->nodes = (rx_node_t *)::malloc(size) ;
            if(parent->nodes == NULL)
                return false ;

            ::memset(parent->nodes , 0 , size) ;
            if(tree->root.nodes == NULL)
            {
                tree->root.nodes = parent->nodes ;
            }
        }

        rx_node_t * child = parent->nodes + offset ;
        if(parent->childs == NULL)
        {
            child->right = child->left = child ;
            parent->childs = child ;
        }
        else
        {
            rx_node_t * childs = parent->childs ;
            child->left = childs->right ;
            child->right = childs ;
            childs->right = child ;
        }

        parent = child ;
        key >>= bits ;
    }

    parent->value = value ;
    return true ;
}

bool rx_tree_erase(rx_tree_t * tree , uint32_t key)
{
    if(tree == NULL || tree->root.nodes == NULL)
        return false ;

    uint32_t level = tree->level ;
    rx_node_t * nodes = tree->root.nodes ;
    for(uint32_t lidx = 0 ; lidx < level ; ++lidx)
    {
        uint8_t bits = tree->bits[lidx] ;
        uint32_t offset = key & ((1 <<  bits) -1) ;
        nodes = nodes + offset ;

        if(nodes == NULL)
            return false ;

        key >>= bits ;
    }

    if(nodes == NULL)
        return false ;

    if(tree->free != NULL)
        tree->free(nodes->value) ;
    return true ;
}

rx_node_t * rx_tree_find(rx_tree_t * tree , uint32_t key)
{
    if(tree == NULL || tree->root.nodes == NULL)
        return false ;

    uint32_t level = tree->level ;
    rx_node_t * cur = &tree->root;
    for(uint32_t lidx = 0 ; lidx < level ; ++lidx)
    {
        uint8_t bits = tree->bits[lidx] ;
        uint32_t offset = key & ((1 <<  bits) -1) ;

        if(cur->nodes == NULL)
            return false ;

        cur = cur->nodes + offset ;
        key >>= bits ;
    }


    return cur ;
}
