#include "lb_rbtree.h"
#include <stdio.h>
#include <stdlib.h>

int is_red_node(struct lb_rbnode *r)
{
    return r && r->color == RED?1:0;
}

void
LB_RBNODE(struct lb_rbnode *r, int data)
{   r->color = RED;
    r->data = data;
    r->children[0] = NULL;
    r->children[1] = NULL;
}

struct lb_rbnode *
lb_rotate_single(struct lb_rbnode *root, int dir)
{
    struct lb_rbnode *new_root = root->children[!dir];
    struct lb_rbnode *new_child = new_root->children[dir];
    new_root->children[dir] = root;
    root->children[!dir] = new_child;
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
    int lh,rh;
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

struct lb_rbnode *
lb_rb_insert_util( struct lb_rbnode *root, int data)
{
    if(root == NULL)
    {
        root = LB_NEW_NODE;
        LB_RBNODE(root, data);
        return root;
    }

    int dir = root->data > data;
    root->children[dir] = lb_rb_insert_util(root->children[dir], data);
    if( is_red_node(root->children[dir]))
    {
        if( is_red_node(root->children[!dir]))
        {   printf("this color switch");
            root->color = RED;
            root->children[0]->color = BLACK;
            root->children[1]->color = BLACK;
        }
        else
        {
            if(is_red_node(root->children[dir]->children[dir]))
            {   printf("did brr");
                root = lb_rotate_single(root, !dir);
            }
            else if(is_red_node(root->children[dir]->children[!dir]))
            {
                printf("did brr2");
                root = lb_rotate_double(root, !dir);
            }
        }
    }

    return root;
}

struct lb_rbtree*
lb_rbnode_insert(struct lb_rbtree *tree, int data)
{
    tree->root = lb_rb_insert_util(tree->root, data);
    tree->root->color = BLACK;
    return tree;
}

void
inorder_traversal(struct lb_rbnode *root)
{
    if(root == NULL)
        return;
    printf("%d color%d ",root->data, root->color);
    inorder_traversal(RB_LEFT(root));
    inorder_traversal(RB_RIGHT(root));
}

struct lb_rbnode*
find_predecessor(struct lb_rbnode *root)
{
    while(root->children[1] != NULL)
    {
        root = root->children[1];
    }
    return root;
}

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
     * BLACK sibling and with 1or both children are red.
     * 
     * 
     */

struct lb_rbnode *lb_rb_remove_balance(struct lb_rbnode *root, int dir, int *done)
{
    struct lb_rbnode *p = root;
    struct lb_rbnode *s = root->children[!dir];

    /* Case reduction, remove red sibling */
    if (is_red_node(s))
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


int main()
{
    struct lb_rbtree *process_tree = LB_NEW_TREE;
    process_tree->root;
    process_tree = lb_rbnode_insert(process_tree, 10);
     inorder_traversal(process_tree->root);

    printf("%d",lb_rbtree_assert(process_tree->root,-100,100));
    printf("\n");
    process_tree = lb_rbnode_insert(process_tree, 20);
     inorder_traversal(process_tree->root);
    printf("%d",lb_rbtree_assert(process_tree->root,-100,100));
    printf("\n");

    process_tree = lb_rbnode_insert(process_tree, 30);
     inorder_traversal(process_tree->root);

    printf("%d",lb_rbtree_assert(process_tree->root,-100,100));
    printf("\n");

    process_tree = lb_rbnode_insert(process_tree, 15);
    inorder_traversal(process_tree->root);
    printf("%d",lb_rbtree_assert(process_tree->root,-100,100));

    printf("\n");
    lb_rb_delete_util(process_tree, 20);
    inorder_traversal(process_tree->root);
    printf("%d",lb_rbtree_assert(process_tree->root,-100,100));
    return 0;
}