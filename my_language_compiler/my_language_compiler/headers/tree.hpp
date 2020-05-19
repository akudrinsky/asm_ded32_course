#ifndef TREE_HEADER
#define TREE_HEADER

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//!
//! It is my implementation of tree
//!
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>

#include "my_lyb.h"
#include "operations.h"
#include "tokens.h"

extern int default_size;

/*
#define DEBUG
#ifdef DEBUG
#define ASSERT(cond)\
if (!(cond)) {\
printf ("Assert failure: < %s > in file %s, line %d, function %s\n", #cond, __FILE__, __LINE__, __PRETTY_FUNCTION__);\
abort();\
}
#else
#define ASSERT(cond) ;
#endif
 */

extern int default_node_size;
struct node {
    char *data;
    node *left;
    node *right;
    node *parent;
    char type;
    
    explicit node (const char tp = UNKNOWN, char* data = nullptr, node* left_l = nullptr, node* right_l = nullptr, node* prnt = nullptr);
    
    ~node ();
    
    //void init (int size = default_node_size);
    void clear ();
    
    bool merge (node *leaf, char where);              //r - right, l - left
    void photo (const char *pict_name = "tree_graph.png", const char *pict_type = "png", int iter = 1, FILE *pFile = nullptr);
    
    void save (const char* filename = "tree_saved.txt", bool parameter = true);
    
    bool is_left ();
    
    bool is_right ();
    
    bool is_valid ();
    
    void to_misha ();
    void to_mine ();
};

node* create_node   (const char* data, char type, node* left, node* right);
node* get_tree      (const char* filename = "tree_saved.txt");
node* rec_descent   (const char* filename = "program.txt");
bool recursive_save (node* nd, FILE* pFile, bool need_slash, bool is_first = true, bool need_closing = true);
node* get_subtree   (node* nd, char where, char* *cur);

#endif
