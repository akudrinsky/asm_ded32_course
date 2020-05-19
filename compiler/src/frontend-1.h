#ifndef FRONTEND_REV
#define FRONTEND_REV

#include "tree.h"

//#define flag printf ("\033[32;1m flag \033[0m\n\n");

#define error(code) fprintf(for_errors,"front_rev::%s (%s)\n", __PRETTY_FUNCTION__, #code);\
                    return false;


struct front_rev {
    node* root;
    FILE* dist;
    FILE* for_errors;
    int tabs;

    explicit front_rev (node* nodes = nullptr, const char* filename = "program_reversed.txt");

    bool print_tabs (int how_many = -1);

    bool global (node* nd);
    bool def_func (node* nd);
    bool call_func (node* nd);
    bool arguments (node* nd);

    bool instruction (node* nd);
    bool command (node* nd);

    bool equation (node* nd);
};

bool front_reverse (node* nd);

#endif
