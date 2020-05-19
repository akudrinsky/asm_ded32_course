#ifndef BACKEND
#define BACKEND

#include "tree.h"

struct b_end {
    FILE* dist;
    node* main_nd;
    char cond_number;
    for_names* local_vars;

    explicit b_end (FILE* req_dist, node* root);

    void write (const char* info1, const char* info2 = "", const char* info3 = "");
    int new_cond ();
    bool reg_name (node* nd);

    bool all ();
    bool block (node* nd);
    bool func_def (node* nd);
    bool func_return (node* nd);
    bool func_call (node* nd);
    bool variable (node* nd);

    bool instructions (node* nd);

    bool assign (node* nd);

    bool condition (node* nd);
    bool cycle (node* nd);

    bool push (node* nd);

    bool increm (node* nd);
    bool decrem (node* nd);

    bool out (node* nd);

    bool equation (node* nd);
};

bool backend (node* nd, const char* program_name = "program.txt");

#endif
