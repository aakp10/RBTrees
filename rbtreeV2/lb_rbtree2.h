#ifndef _LB_RBTREE_H
#define _LB_RBTREE_H

#include <stdio.h>
#include <stdlib.h>
#include <linux/kernel.h>
#include <linux/stddef.h>
enum mark{
    RED,
    BLACK
};

struct lb_rbnode{
    enum mark color;
    struct lb_rbnode *children[2];
    struct lb_rbnode *parent;
};

struct lb_rbtree{
    struct lb_rbnode *root;
};


struct my_tree{
    int data;
    struct lb_rbnode *node;
};

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

#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#define container_of(ptr, type, member) ({                      \
             const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
             (type *)( (char *)__mptr - offsetof(type,member) );})

#define get_node(nodeptr, type, fieldname) container_of(nodeptr, type, fieldname)

struct lb_rbnode *insert_balance(struct lb_rbnode *root);
//struct lb_rbnode *lb_rb_insert_util(struct lb_rbnode *root, int data);
int lb_rb_delete_util(struct lb_rbtree *root, int data);


#endif