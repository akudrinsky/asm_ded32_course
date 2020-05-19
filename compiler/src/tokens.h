#ifndef TOKENS                           //flag that tree was defined only once in all project
#define TOKENS

#include <cstdio>
#include <string.h>

struct for_names {
    char* *names;
    int amount;
    int initial_ammount;

    explicit for_names (int size = 30);
    void append (char* element);

    void print ();

    int search_name (char* name);
    ~for_names ();
};

struct token {
    char* name;
    const int code;
    const char n_args;
    const char length;

    int search_token (char* name);
};

enum token_codes {
    UNKNOWN,
    PROGRAMM,
    BLOCK,
    BAD_GUY,
    INSTRUCTION,
    VARLIST,
    ID,
    VARIABLE,

    DEFINITION,
    CALL,
    RETURN,

    NUMBER,
    ASSIGN,
    OUT,

    IF,
    CONDITION,
    WHILE,
    IS_EQUAL,
    IS_BIGGER,
    IS_LESS,
    NOT_EQUAL,
    PLUS,
    MINUS,                      //......
    MULT,
    DIVIDE,

    INCR,
    DECR,

    OPEN_BR,
    CLOS_BR,
    COND_DIVIDER,

    OPEN_EQ,
    CLOS_EQ,

    ARGUMENTS,
    LAST_ARG,
    ASSIGN_BY_LEN,

    PRINT
};

enum person_states {
    DEAD,                       //states
    DESTROYED,
    DISLOYAL,
    AFRAID,
    FURIOUS,
    ANGRY,
    SAD,
    DISHONEST,
    CONFUSED,
    FORGIVEN,
    LOYAl,
    HAPPY
};

extern token tokens[];

extern const token states[];

#endif
