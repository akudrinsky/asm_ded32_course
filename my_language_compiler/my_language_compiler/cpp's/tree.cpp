//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//!
//! It is my implementation of wolphram alpha derivatives
//!
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐

#ifndef TREE_WAS_DEFINED                           //flag that tree was defined only once in all project
#define TREE_WAS_DEFINED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <new>

#include "tree.hpp"
#include "my_lyb.h"
#include "operations.h"
#include "tokenizer.h"
#include "frontend.h"
#include "frontend-1.h"

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

#define el_array(name)\
sizeof (name) / sizeof (name[0])

#define get_rand_elem(name)\
name[rand() % (sizeof (name) / sizeof (name[0]))]

int default_node_size = 20;
int default_size = 20;

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Kind of analog of node construct, but it is more convinient for recursive diff
//!
//! @param [in] data - value
//! @param [in] type - type of node
//! @param [in] left - left leaf
//! @param [in] right - right leaf
//!
//! @return pointer to newly created node
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
node* create_node (const char* data, char type, node* left, node* right) {
    node* nd = new node;
    ASSERT (nd != nullptr)
    
    if (data == nullptr) {
        nd->data = nullptr;
    }
    else {
        nd->data = new char[default_size];
        strncpy (nd->data, data, default_size);
    }
    nd->type = type;
    nd->left = left;
    nd->right = right;
    
    if (nd->is_left())
        nd->left->parent = nd;
    if (nd->is_right())
        nd->right->parent = nd;
    
    ASSERT (nd != nullptr)
    
    return nd;
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Node construct
//!
//! @param [in] prnt - parent node
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
node::node(const char tp, char* str, node* left_l, node* right_l, node* prnt) {
    if (str == nullptr) {
        data = nullptr;
    }
    else {
        data = new char[default_size];
        strncpy (data, str, default_size);
    }
    
    left = left_l;
    right = right_l;
    parent = prnt;
    type = tp;                                         // unknown
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//!
//! Destructor of a tree
//!
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
node::~node() {                         //make it!!!
    
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//!
//! Safe clearing of a tree
//!
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
void node::clear() {
    if (is_left()) {
        left->clear();
        delete left;
    }
    if (is_right()) {
        right->clear();
        delete right;
    }
    
    left = nullptr;
    right = nullptr;
    parent = nullptr;
    type = UNKNOWN;
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Merging leaf to a given node from a given side
//!
//! @param [in] where - l for left, r for right
//! @param [in] leaf - leaf to merge
//! @return - if it was OK
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
bool node::merge(node* leaf, char where) {
    if (leaf == nullptr) {
        if (where == 'l') {
            left = nullptr;
        }
        else if (where == 'r') {
            right = nullptr;
        }
        else {
            err_info ("Wrong format of merge, where = ");
            err_info (&where);
            err_info ("\n");
            return false;
        }
    }
    else {
        //printf ("son in merge: \t%p\n", leaf);
        if (where == 'l') {
            left = leaf;
            leaf->parent = this;
        }
        else if (where == 'r') {
            right = leaf;
            leaf->parent = this;
        }
        else {
            err_info ("Wrong format of merge, where = ");
            err_info (&where);
            err_info ("\n");
            return false;
        }
    }
    
    return true;
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Makes picture of a tree
//!
//! @param [in] pict_name - name of photo
//! @param [in] pict_type - type (ex: png)
//! @param [in] iter - NOT FOR USERS
//! @param [in] pFile - NOT FOR USERS
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
void node::photo(const char* pict_name, const char* pict_type, int iter, FILE* pFile) {
    ASSERT (pict_name != nullptr)
    ASSERT (pict_type != nullptr)
    
    if (iter == 1) {
        pFile = fopen ("tree_graph.dot", "w");
        ASSERT (pFile != nullptr)
        fprintf (pFile, "digraph G{\n\tedge[color=\"chartreuse4\",fontcolor=\"blue\",fontsize=12];\n\tnode[shape=\"rectangle\",fontsize=15];\n");
    }
    ASSERT (pFile != nullptr)
    
    if (tokens[type].name[0] == '<' || tokens[type].name[0] == '>') {
        fprintf (pFile, "\t%d [shape=record,label=\"  <f0> %p| {\\%s | \\%s} | <f1> %p\" ];\n", iter, left, tokens[type].name, data, right);
    }
    else {
        fprintf (pFile, "\t%d [shape=record,label=\"  <f0> %p| {%s | %s} | <f1> %p\" ];\n", iter, left, tokens[type].name, data, right);
    }
    
    //edges
    if (left != nullptr) {
        (*left).photo (pict_name, pict_type, iter * 2, pFile);
        fprintf (pFile, "\t\t%d:<f0> -> %d\n", iter, iter * 2);
    }
    if (right != nullptr) {
        (*right).photo (pict_name, pict_type, iter * 2 + 1, pFile);
        fprintf (pFile, "\t\t%d:<f1> -> %d[color=\"orange\"]\n", iter, iter * 2 + 1);
    }
    
    if (iter == 1) {
        fprintf (pFile, "}");
        fclose (pFile);
        
        //executing command in terminal
        const int max_cmd_size = 50;
        char command[max_cmd_size] = "dot tree_graph.dot -T ";
        strcat (command, pict_type);
        strcat (command, " -o ");
        strcat (command, pict_name);
        
        system (command);
        
        system ("rm tree_graph.dot");
    }
}
//#include "rec.h"

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Saves tree to a txt file format
//!
//! @param [in] filename - name of file to save tree to
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
void node::save(const char *filename, bool parameter) {
    FILE* pFile = fopen (filename, "w");
    
    recursive_save (this, pFile, parameter);
    
    fclose (pFile);
}

bool recursive_save (node* nd, FILE* pFile, bool need_slash, bool is_first, bool need_closing) {
    bool status = true;
    
    if (need_slash) {
        if (nd->type == NUMBER) {
            fprintf (pFile, "{N:%s", nd->data);
        }
        else if (nd->type == ID) {
            fprintf (pFile, "{I:%s#", nd->data);
        }
        else {
            fprintf (pFile, "{O:%s#", tokens[nd->type].name);
        }
    }
    else {
        if (nd->type == NUMBER) {
            fprintf (pFile, "{N:%s", nd->data);
        }
        else if (nd->type == ID) {
            fprintf (pFile, "{I:%s", nd->data);
        }
        else {
            fprintf (pFile, "{O:%s", tokens[nd->type].name);
        }
    }
    
    if (nd->left == nullptr && nd->right != nullptr) {
        fprintf (pFile, "@");
        
        if (!recursive_save (nd->right, pFile, need_slash, false, false))
            status = false;
    }
    if (nd->left != nullptr && nd->right == nullptr) {
        if (!recursive_save (nd->left, pFile, need_slash, false, false))
            status = false;
        
        if (!need_slash) {
            fprintf (pFile, "@");
        }
    }
    if (nd->left != nullptr && nd->right != nullptr) {
        if (!recursive_save (nd->left, pFile, need_slash, false, false))
            status = false;
        if (!recursive_save (nd->right, pFile, need_slash, false, true))
            status = false;
    }
    fprintf (pFile, "}");
    
    return status;
}

bool skip_dividers (char** cur) {
    while (**cur == '\n' || **cur == ' ') {
        (*cur)++;
    }
    return true;
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Users function for making tree from txt file
//!
//! @param [in] filename - name of file from which we need to make tree
//!
//! @return if it was good
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
node* get_tree (const char* filename) {
    ASSERT (filename != nullptr)
    FILE* pFile = fopen (filename, "r");
    ASSERT (pFile != nullptr)
    
    char* file = read_text (pFile);
    
    node* root = get_subtree (nullptr, 'l', &file);
    fclose (pFile);
    
    return root;
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Recursive function for making tree from txt file
//!
//! @param [in] nd - pointer to node - parent
//! @param [in] where - l for left, r for right
//! @param [in] cur - pointer to text
//!
//! @return if it was good
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
node* get_subtree (node* nd, char where, char* *cur) {
    ASSERT (cur != nullptr)
    
    if (**cur != '{') {
        return nullptr;
    }
    ++(*cur);
    
    node* new_nd = new node;
    new_nd->parent = nd;
    
    char* leaf_data = new(std::nothrow) char[default_node_size]();
    if (leaf_data == nullptr) {
        err_info ("Unable to calloc leaf_data in get_subtree\n");
        return nullptr;
    }
    int got_c = 0;
    
    if (**cur == 'N') {
        *cur += 2;
        new_nd->type = NUMBER;
        sscanf (*cur, "%[0-9]%n", leaf_data, &got_c);
        new_nd->data = leaf_data;
        *cur += got_c;
    }
    else if (**cur == 'I') {
        *cur += 2;
        sscanf (*cur, "%[^#]%n", leaf_data, &got_c);
        *cur += got_c + 1;
        
        new_nd->type = ID;
        new_nd->data = leaf_data;
        
    }
    else {
        *cur += 2;
        sscanf (*cur, "%[^#]%n", leaf_data, &got_c);
        *cur += got_c + 1;
        
        new_nd->type = tokens->search_token (leaf_data);
        new_nd->data = leaf_data;
    }
    
    printf ("got %c (st) %s\tcur = %c\n", where, leaf_data, **cur);
    
    if (**cur == '{') {
        new_nd->left = get_subtree (new_nd, 'l', cur);
        if (new_nd->left == nullptr) {
            return nullptr;
        }
    }
    else if (**cur == '@') {
        *cur += 1;
        if (**cur != '{') {
            return nullptr;
        }
    }
    
    if (**cur == '{') {
        new_nd->right = get_subtree (new_nd, 'r', cur);
        if (new_nd->right == nullptr) {
            return nullptr;
        }
    }
    
    if (**cur != '}') {
        return nullptr;
    }
    
    *cur += 1;
    return new_nd;
}

node* rec_descent (const char *filename) {
    FILE* input = fopen (filename, "r");
    
    char* str = read_text (input);
    //printf ("%s", str);
    fclose (input);
    
    cell *nodes = tokenize (str);
    
    /*
    printf ("tokens:\n");
    int i = 0;
    while (nodes[i].type != UNKNOWN) {
        printf ("%9s\t\t%s\n", tokens[nodes[i].type].name, nodes[i].name);
        ++i;
    }
     */
    
    frontend* parser = new frontend (nodes);
    
    return parser->all ();
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//!
//! If a node has left child
//!
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
bool node::is_left() {
    return left != nullptr;
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//!
//! If a node has right child
//!
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
bool node::is_right() {
    return right != nullptr;
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! If a tree is valid
//!
//! @return if it was OK
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
bool node::is_valid () {
    //printf ("valiator launched\n");
    if (is_left()) {
        if (this != this->left->parent) {
            err_info ("Lost connection between node and his left child\n");
        }
        if (!this->left->is_valid()) {
            return false;
        }
    }
    
    if (is_right()) {
        if (this != this->right->parent) {
            err_info ("Lost connection between node and his right child\n");
            return false;
        }
        if (!this->right->is_valid()) {
            return false;
        }
    }
    
    return true;
}

void node::to_misha () {
    
    if (type == IF) {
        right = create_node ("misha", CONDITION, nullptr, right);
    }
    
    if (type == CONDITION || type == WHILE) {
        right = create_node ("misha", BAD_GUY, nullptr, right);
    }
    
    /*
     if (type == VARIABLE) {
     node* tmp = left;
     left = right;
     right = tmp;
     }
     */
    
    
    
    if (type == IS_BIGGER) {
        type = IS_LESS;
    }
    
    if (type == IS_LESS) {
        type = IS_BIGGER;
    }
    
    if (type == DEFINITION) {
        if (strncmp (right->data, "reign", 5) == 0) {
            strcpy (right->data, "main");
        }
    }
    
    if (type == RETURN) {
        left = create_node ("misha", VARLIST, nullptr, right);
        right = nullptr;
    }
    
    if (type == OUT) {
        left = create_node ("misha", VARLIST, nullptr, right);
        right = nullptr;
    }
    
    if (type == DEFINITION) {
        right->right = create_node ("misha", BAD_GUY, nullptr, right->right);
    }
    
    if (type == DEFINITION) {
        node* tmp = left;
        while (tmp != nullptr) {
            tmp->right = create_node ("misha", VARIABLE, tmp->right, nullptr);
            tmp = tmp->left;
        }
    }
    
    if (is_left()) {
        left->to_misha ();
    }
    if (is_right()) {
        right->to_misha ();
    }
    
}

void node::to_mine () {
    printf ("%s\n", tokens[type].name);
    
    if (type == IF) {
        right = right->right->right;
        printf ("1\n");
    }
    
    if (type == WHILE) {
        right = right->right;
        printf ("2\n");
    }
    
    if (type == RETURN) {
        right = left->right;
        left = nullptr;
        printf ("3\n");
    }
    
    if (type == OUT) {
        right = left->right;
        left = nullptr;
        printf ("4\n");
    }
    
    if (type == DEFINITION) {
        right->right = right->right->right;
        printf ("5\n");
    }
    
    if (type == DEFINITION) {
        node* tmp = left;
        while (tmp != nullptr) {
            tmp->right = tmp->right->left;
            tmp = tmp->left;
        }
    }
    
    if (type == IS_BIGGER) {
        type = IS_LESS;
    }
    
    if (type == IS_LESS) {
        type = IS_BIGGER;
    }
    
    if (left != nullptr) {
        left->to_mine ();
    }
    
    if (type == DEFINITION) {
        if (strncmp (right->data, "main", 5) == 0) {
            strcpy (right->data, "reign");
        }
    }
    
    if (right != nullptr) {
        right->to_mine ();
    }
}


#endif
