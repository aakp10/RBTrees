#include "lb_rbtree2.h"
#include <stdio.h>
#include <stdlib.h>
#include <linux/kernel.h>
#include <linux/stddef.h>

int is_red_node(struct lb_rbnode *r)
{
    return r && r->color == RED?1:0;
}

void
LB_RBNODE(struct lb_rbnode *r, struct lb_rbnode *parent)
{   r->color = RED;
    r->children[0] = NULL;
    r->children[1] = NULL;
    r->parent = parent;
}

struct lb_rbnode *
lb_rotate_single(struct lb_rbnode *root, int dir)
{
    struct lb_rbnode *new_root = root->children[!dir];
    struct lb_rbnode *new_child = new_root->children[dir];
    new_root->children[dir] = root;
    root->children[!dir] = new_child;

    new_root->parent = root->parent;
    root->parent = new_root;
    if(new_child)
        new_child->parent = root;

    new_root->color = BLACK;
    root->color = RED;
    return new_root;
}

struct lb_rbnode *
lb_rotate_double(struct lb_rbnode *root, int dir)
{
    root->children[!dir] = lb_rotate_single(root->children[!dir], !dir);
    return lb_rotate_single(root, dir);
}

/*
int
lb_rbtree_assert(struct lb_rbnode *root, int min, int max)
{
    if(root == NULL)
    {
        return 1;
    }

    struct lb_rbnode *right_child = RB_RIGHT(root);
    struct lb_rbnode *left_child = RB_LEFT(root);
    if(is_red_node(root))
    {
        if(is_red_node(right_child) || is_red_node(left_child))
        {
            printf("Red violation %d ",root->data);
            return 0;
        }
    }

    /*if( root->data <min || root->data >max)
        return 0;*/
 /*   int lh,rh;
    lh = lb_rbtree_assert(left_child, min, root->data);
    rh = lb_rbtree_assert(right_child, root->data, max);
    if(lh !=0 && rh!=0 && lh!=rh)
    {
        printf("BLACK violation");
        return 0;
    }

    if(lh!=0 && rh!=0)
    {
        return is_red_node(root)? lh: lh+1;
    }
    return 0;
}
*/

struct lb_rbnode*
lb_rb_ibalance(struct lb_rbnode *parent, struct lb_rbnode *new_node)
{
    //int dir = root->data > data;
    //root->children[dir] = lb_rb_insert_util(root->children[dir], data);
    while(parent)
    {
        int dir = (parent->children[1] == new_node);
        if( is_red_node(parent->children[dir]))
        {
            if( is_red_node(parent->children[!dir]))
            {   printf("this color switch");
                parent->color = RED;
                parent->children[0]->color = BLACK;
                parent->children[1]->color = BLACK;
            }
            else
            {
                if(is_red_node(parent->children[dir]->children[dir]))
                {   printf("did brr");
                    parent = lb_rotate_single(parent, !dir);
                }
                else if(is_red_node(parent->children[dir]->children[!dir]))
                {
                    printf("did brr2");
                    parent = lb_rotate_double(parent, !dir);
                }
            }
        }
        new_node = parent;
        parent = parent->parent;
    }
    return new_node;
}

struct lb_rbnode*
lb_rb_insert_util( struct lb_rbnode **child_dir_link, struct lb_rbnode* child, struct lb_rbnode *parent)
{
    *child_dir_link = child;
    LB_RBNODE(child,parent);
    struct my_tree* temp_data = container_of(*child_dir_link, struct my_tree, node);
    printf("%d ",temp_data->data);
    return lb_rb_ibalance(parent, child);
}

struct lb_rbtree*
lb_rbnode_insert(struct lb_rbtree *tree, struct my_tree* my_data)
{
    //tree->root = lb_rb_insert_util(tree->root, data);
    //tree->root->color = BLACK;
    struct lb_rbnode **link = &(tree->root);
    struct lb_rbnode *parent = NULL;
    int data = my_data->data;
    //printf("%d ",data);
    while(*link)
    {
        struct my_tree* temp_data = container_of(*link, struct my_tree, node);
        int dir = temp_data->data > data;
        parent = *link;
        link = &((*link)->children[dir]);
    }
    tree->root = lb_rb_insert_util(link, my_data->node, parent);
    tree->root->color = BLACK;
    return tree;
}


void
lb_rbnode_inorder(struct lb_rbnode *root)
{
    if(root == NULL)
        return;

    struct my_tree* temp_data = container_of(root, struct my_tree, node);
    printf("%d ,%p   ", temp_data->data, temp_data);
    lb_rbnode_inorder(root->children[0]);
    lb_rbnode_inorder(root->children[1]);
}

struct my_tree*
new_my_tree(int data)
{
    struct my_tree *new_tree = (struct my_tree*)malloc(sizeof(struct my_tree));
    new_tree->data = data;
    printf("%d  ",data);
    new_tree->node = (struct lb_rbnode*)malloc(sizeof(struct lb_rbnode));
    return new_tree;
    //LB_RBNODE(new_tree->node, NULL);
}

int main()
{
    struct lb_rbtree *process_tree = LB_NEW_TREE;
    process_tree->root = NULL;
    struct my_tree *to_be_inserted1 = new_my_tree(1);
    process_tree = lb_rbnode_insert(process_tree,to_be_inserted1);
    //lb_rbnode_inorder(process_tree->root);
    struct my_tree *to_be_inserted2 = new_my_tree(2);
    process_tree = lb_rbnode_insert(process_tree,to_be_inserted2);
    struct my_tree *to_be_inserted3 = new_my_tree(3);
    process_tree = lb_rbnode_insert(process_tree,to_be_inserted3);
    struct my_tree *to_be_inserted4 = new_my_tree(4);
    process_tree = lb_rbnode_insert(process_tree,to_be_inserted4);

    return 0;
}
/**
* FIX ME FIX ME FIX ME FIX ME FIX ME
*/
/*void
inorder_traversal(struct lb_rbnode *root)
{
    if(root == NULL)
        return;
    printf("%d color%d ",root->data, root->color);
    inorder_traversal(RB_LEFT(root));
    inorder_traversal(RB_RIGHT(root));
}*/

/*struct lb_rbnode*
find_predecessor(struct lb_rbnode *root)
{
    while(root->children[1] != NULL)
    {
        root = root->children[1];
    }
    return root;
}*/

/**
 * If node deleted is red = no violations, no red violation, black height still remains the same 
 * black node deletion = both red and black violations can occur
 *
 * if the node deleted was black and had 1 red child = recolor to black.
 */
/*struct lb_rbnode*
lb_rb_remove_balance(struct lb_rbnode* root, int dir, int *done)
{
    /**
     * if you delete a black node:
     *   if its sibling is BLACK and its children are also BLACK , recolor the sibling to RED.
     *      This doesn't create any black violation if the parent of the deleted node is root
     *      Otherwise it can cause a Black violation.
     *      To avoid this, in case the parent was red -> recolor to BLACK to counter the reduced black node in the sub tree and set the flag
     *      If the parent was black we need to keep checking this upwards.
     */

    /**
     * BLACK sibling and wither 1or both children are red.
     * 
     * 
     */
/*
struct lb_rbnode *lb_rb_remove_balance(struct lb_rbnode *root, int dir, int *done)
{
    struct lb_rbnode *p = root;
    struct lb_rbnode *s = root->children[!dir];
*/
    /* Case reduction, remove red sibling */
/*    if (is_red_node(s))
    {
        root = lb_rotate_single(root, dir);
        s = p->children[!dir];
    }

    if (s != NULL)
    {
        if (!is_red_node(s->children[0]) && !is_red_node(s->children[1]))
        {
            if (is_red_node(p))
            {
                *done = 1;
            }

            p->color = BLACK;
            s->color = RED;
        }
        else
        {
            int save = p->color;
            int new_root = (root == p);

            if (is_red_node(s->children[!dir]))
            {
                p = lb_rotate_single(p, dir);
            }
            else
            {
                p = lb_rotate_double(p, dir);
            }

            p->color = save;
            p->children[0]->color = BLACK;
            p->children[1]->color = BLACK;

            if (new_root)
            {
                root = p;
            }
            else
            {
                root->children[dir] = p;
            }

            *done = 1;
        }
    }
    return root;
}

struct lb_rbnode*
lb_rb_delete(struct lb_rbnode* root, int data, int *status)
{
    if(root == NULL)
    {
        *status = 1;
        return NULL;
    }

    if(data == root->data)
    {
        if(root->children[0] == NULL || root->children[1] == NULL)
        {
            struct lb_rbnode *child = root->children[root->children[0] == NULL];
            if(is_red_node(root))
            {
                *status = 1;
            }
            else if(is_red_node(child))
            {
                child->color = BLACK;
                *status = 1;
            }
            free(root);
            return child;
        }
        else{
            struct lb_rbnode *inorder_predeccessor = find_predecessor(root->children[0]);
            root->data = inorder_predeccessor->data;
            //root->children[0] = lb_rb_delete(root->children[0], root->data);
        }

    }

    int dir = data > root->data;
    root->children[dir] = lb_rb_delete(root->children[dir], data, status);

    if(!status)
    {
        root = lb_rb_remove_balance(root, dir, status);
    }
    return root;
}

int
lb_rb_delete_util(struct lb_rbtree *tree, int data)
{
    int done = 0;
    tree->root = lb_rb_delete(tree->root, data, &done);
    if(tree->root != NULL)
    {
        tree->root->color = BLACK;
    }
    return 1;
}
*/