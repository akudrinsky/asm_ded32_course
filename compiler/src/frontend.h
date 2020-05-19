#ifndef FRONTEND
#define FRONTEND

#include "tree.h"
#include "tokenizer.h"

#define flag printf ("\033[32;1m flag \033[0m\n\n");

struct frontend {
    cell* nds;

    explicit frontend (cell* nodes = nullptr);

    node* all ();                   //1
    node* global ();                //2
    node* def_func ();              //3
    node* arguments ();
    node* variable ();              //3

    node* get_id ();                //3

    node* instruction ();           //2
    node* get_number ();                //4

    node* condition ();
    node* cycle ();

    node* print ();

    node* equation ();
    node* multiplication ();
    node* brackets ();

};

#endif
