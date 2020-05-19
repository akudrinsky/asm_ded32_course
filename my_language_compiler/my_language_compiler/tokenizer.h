#ifndef TOKENiZIER
#define TOKENiZIER

#include "tree.hpp"

struct for_funcs {
    node* *names;
    int amount;

    explicit for_funcs (int size = 5);
    void append (node* element);
    void see ();

    int search_func (node* name);
    ~for_funcs ();
};

bool not_special (char symbol);
bool not_divider (char symbol);

#define now_parse_that_long_long_name   \
    nodes[n_nodes].type = ID;           \
    char* id = new char[default_size];  \
    char* cur = id;                     \
                                        \
    while (not_divider (*str)) {        \
        *cur = *str;                    \
        ++cur;                          \
        ++str;                          \
    }                                   \
    if (*(cur - 1) == ',') {            \
        *(cur - 1) = '\0';              \
    }                                   \
                                        \
    if (cur - id > default_size) {      \
        err_info ("Overwritten id - tokenize\n");\
    }                                   \
                                        \
    strncpy (data, id, cur - id);       \
    delete [] id;
    //ids->append (id);


struct cell {
    char* name;
    int type;

    explicit cell ();
    ~cell ();
};

cell* tokenize (char* str, const int max_nodes_number = 100);

#endif
