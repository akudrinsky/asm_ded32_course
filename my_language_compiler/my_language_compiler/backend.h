#ifndef BACKEND
#define BACKEND

#include "tree.hpp"
#include "make_file.hpp"

struct b_end {
    //FILE*       dist;
    //void write (const char* cmd, const char* cmd1 = "", const char* cmd2 = "") {}
    
    unsigned char* code = nullptr;
    uint32_t len = 0;
    uint32_t max_len = 1000;
    
    node* main_nd;
    unsigned char cond_number;
    for_names* local_vars;

    b_end () = delete;
    
    explicit b_end (node* root);
    ~b_end ();
    
    void write (const unsigned char cmd[], const int cmd_len);    //
    
    int new_cond ();
    uint32_t reg_name (node* nd);

    bool all ();                                        //
    bool block (node* nd);
    bool func_def (node* nd);
    bool func_return (node* nd);
    bool func_call (node* nd);
    bool variable (node* nd);

    bool instructions (node* nd);

    bool assign (node* nd);
    
    void pushm (int32_t var_id);
    void popm (int32_t var_id);

    bool condition (node* nd);
    bool cycle (node* nd);

    bool push (node* nd);

    bool increm (node* nd);
    bool decrem (node* nd);

    bool out (node* nd);

    bool equation (node* nd);
    
    void fill_labels ();
};

bool backend (node* nd, const char* program_name = "backend_program.txt");

#endif
