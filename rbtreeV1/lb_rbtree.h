#ifndef _LB_RBTREE_H
#define _LB_RBTREE_H

#include <stdio.h>
#include <stdlib.h>

enum mark{
    RED,
    BLACK
};

struct lb_rbnode{
    enum mark color;
    int data;
    struct lb_rbnode *children[2];
};

struct lb_rbtree{
    struct lb_rbnode *root;
};

//#define is_red_node(r) (r->color == RED? 1 : 0)
#define LB_TREE (struct lb_rbtree) {NULL}
//#define LB_RBNODE(r, data) ({ r->color = RED;\
                              r->data = data;\
                              r->children[0] = NULL;\
                              r->children[1] = NULL;\
                            })
#define LB_NEW_NODE (struct lb_rbnode *) (malloc(sizeof(struct lb_rbnode)))
#define LB_NEW_TREE (struct lb_rbtree *) (malloc(sizeof(struct lb_rbtree)))
#define RB_RIGHT(r) (r->children[1])
#define RB_LEFT(r)  (r->children[0])

struct lb_rbnode *lb_rb_insert_util(struct lb_rbnode *root, int data);
int lb_rb_delete_util(struct lb_rbtree *root, int data);


#endif