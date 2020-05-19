#ifndef OPERATIONS                           //flag that tree was defined only once in all project
#define OPERATIONS

struct oper {
    const char* name;
    const int code;
    const char n_args;                //* has higher priority than +
    const char complexity;
};

#endif
