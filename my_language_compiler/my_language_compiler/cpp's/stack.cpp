//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//!
//! It is my implementation of stack
//!
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
#ifndef STACK_WAS_DEFINED                           //flag that stack was defined only once in all project
#define STACK_WAS_DEFINED

//if you don't want to have hash and canaries - comment it
#define DEFENCE

#define St_construct(stk) Stack_Construct(stk, #stk);

#define Stack_OK(stk)                                   \
    Stack_okey (&stk);                                  \
    Dump_f(&stk, __FILE__, __LINE__, __PRETTY_FUNCTION__);

#define ST_HASH(stk)\
    (hash_it (stk, ST_NUMBER) + hash_it (stk->data, stk->max_size))

#ifdef DEFENCE
#define ON_DEBUG(code) code
#else
#define ON_DEBUG(code) ;
#endif

#include <stdio.h>
#include <stdlib.h>
//#include <abort.h>

typedef int Elem_t;

enum st_errors_c {
    ok,
    nulldata,
    pop_last,
    size_out_of_max,
    canary_t,
    canary_b,
    bad_hash,
    destructed
};

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Stack structure
//!
//! @param [in] name - name of stack
//! @param [in] data - array of stack values
//! @param [in] size - number of elements in stack
//! @param [in] is_valid - status of stack: 0 - ok, 1 - null data, 2 - pop last element, 3 - size > MAX_ELEM, 4 - problems with canary
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
#pragma pack(push, 1)
struct Stack_t {
    ON_DEBUG (int canary_t);
    const char* name;
    Elem_t* data;
    int size;
    int max_size;
    st_errors_c is_valid;             //seek for st_errors_c, st_errors_n
    ON_DEBUG (int hash;)
    ON_DEBUG (int canary_b;)
};
#pragma pack(pop)

const unsigned int ST_NUMBER = sizeof (Stack_t) - sizeof (int) ON_DEBUG(- sizeof(int));

void Stack_Construct (Stack_t* stk, const char* stk_name = "stk");
void Stack_Destruct (Stack_t* stk);
void Dump_f (Stack_t* str, const char* file, int line,
        const char* func_name, const char* info = "assert",
        bool is_silent = false, bool no_corr = false,
        const char* file_name = "stack_dump.txt");
bool St_push (Stack_t* stk, Elem_t value);
bool St_widen (Stack_t* stk, double factor = 1.4);
Elem_t St_pop (Stack_t* stk);
void St_narrow (Stack_t* stk, double factor = 1.5);
bool Stack_okey (Stack_t* stk);
int hash_it (void* stk, int number, int seed = 44);
int data_hash (Stack_t* stk, int seed = 55);
void print_stack (Stack_t* stk);
void unit_Stack1 (void);
void unit_Stack2 (void);
void unit_Stack3 (void);

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Stack construct
//!
//! @param [in] stk - pointer to stack
//! @param [in] stk_name - name of stack (if you don't need it, it will be "stk" by default)
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
void Stack_Construct (Stack_t* stk, const char* stk_name) {

    ON_DEBUG (stk->canary_t = 0);
    stk->data = (Elem_t *)calloc (10, sizeof(Elem_t));
    stk->size = 0;
    stk->is_valid = ok;
    stk->max_size = 10;
    stk->name = stk_name;
    ON_DEBUG (stk->canary_b = 0);
    ON_DEBUG (stk->hash = ST_HASH (stk);)

    Stack_OK (*stk);
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Stack desctruct
//!
//! @param [in] stk - pointer to stack
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
void Stack_Destruct (Stack_t* stk) {

    ON_DEBUG (stk->canary_t = 0);
    free (stk->data);
    stk->size = 0;
    stk->is_valid = destructed;
    stk->max_size = 0;
    stk->name = nullptr;
    ON_DEBUG (stk->canary_b = 0);
    ON_DEBUG (stk->hash = 0;)
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Pushes element to stack
//!
//! @param [in] stk - pointer to stack
//! @param [in] value - value to push
//! @return if everything is OK
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
bool St_push (Stack_t* stk, Elem_t value) {
    Stack_OK (*stk);

    if (stk->size >= stk->max_size) {
        St_widen (stk);
        St_push (stk, value);
        return false;
    }
    else {
        stk->data[stk->size] = value;
        ++stk->size;
    }
    ON_DEBUG (stk->hash = ST_HASH (stk);)

    Stack_OK (*stk);

    return true;
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Makes max size of stack bigger if it is needed
//!
//! @param [in] stk - pointer to stack
//! @param [in] factor - how many times new number of elements needs to be bigger
//! @return if everything is OK
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
bool St_widen (Stack_t* stk, double factor) {
    Stack_OK (*stk);
    stk->data = (Elem_t *)realloc (stk->data, sizeof(Elem_t) * stk->size * factor);
    stk->max_size = stk->max_size * factor;
    ON_DEBUG (stk->hash = ST_HASH (stk);)
    Stack_OK (*stk);
    return true;
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Returns last element of stack and makes max size of stack less if there is much free space
//!
//! @param [in] stk - pointer to stack
//! @return last element of stack
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
Elem_t St_pop (Stack_t* stk) {
    Stack_OK (*stk);
    St_narrow (stk);

    if (stk->size < 1) {
        stk->is_valid = pop_last;
        ON_DEBUG(stk->hash = ST_HASH (stk);)

        Stack_OK (*stk);

        return {};
    }
    else {
        --stk->size;
        Elem_t buf = stk->data[stk->size];
        stk->data[stk->size] = {};
        ON_DEBUG (stk->hash = ST_HASH (stk);)
        Stack_OK (*stk);

        return buf;
    }
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Makes size of stack less if there is much free space
//!
//! @param [in] stk - pointer to stack
//! @param [in] factor - how many times new number of elements needs to be less
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
void St_narrow (Stack_t* stk, double factor) {
    Stack_OK (*stk);
    if (stk->max_size > 14 && stk->size * (factor+0.1) < stk->max_size) {
        stk->data = (Elem_t *)realloc (stk->data, sizeof(Elem_t) * stk->size / factor);
        stk->max_size = stk->max_size / factor;
        ON_DEBUG(stk->hash = ST_HASH (stk);)
        Stack_OK (*stk);
    }
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//!
//! @param [in] str - pointer to stack
//! @param [in] file - file where this Dump_f was called
//! @param [in] line - line in file where this Dump_f was called
//! @param [in] func_name - name of function in file where this Dump_f was called
//! @param [in] is_silent - if you need to stop the program if Stack is not ok
//! @param [in] no_corr - if you need to print info about valid stacks or not
//! @param [in] file_name - where to print info about stacks
//!
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
void Dump_f (Stack_t* str, const char* file,
        int line, const char* func_name, const char* info,
        bool is_silent, bool no_corr, const char* file_name) {
    FILE* pFile = fopen (file_name, "ab");

    const char* status = "ok";
    if (str->is_valid != ok)                                                             //int or Elem_t????
        status = "NOT OK";

    if (!no_corr || str->is_valid != 0) {
        fprintf (pFile, "%s from %s in line %d (file: %s)\n", info, func_name, line, file);
        fprintf (pFile, "Stack_t <%s> [%p] (%s)\n", str->name, str, status);
        fprintf (pFile, "{\n\tsize = %d\n\tdata[%d] = %p\n", str->size, str->max_size, str->data);
        fprintf (pFile, "\t{\n");

        for (int i = 0; i < str->size; ++i) {
            fprintf (pFile, "\t* [%d] = %d\n", i, str->data[i]);
        }

        for (int i = str->size; i < str->max_size; ++i) {
            fprintf (pFile, "\t  [%d] = %d\n", i, str->data[i]);
        }
        fprintf(pFile, "\t}\n");

        switch (str->is_valid) {
            case ok:
                break;
            case nulldata:
                fprintf (pFile, "Pointer to data is nullptr\n");
                break;
            case pop_last:
                fprintf (pFile, "Trying to pop non-existing element\n");
                break;
            case size_out_of_max:
                fprintf (pFile, "Number of elements is bigger than max value\n");
                break;
            case destructed:
                fprintf (pFile, "Stack was destructed\n");
                break;
            ON_DEBUG (case canary_t:
                          fprintf (pFile, "Top canary has been changed\n");
                              break;
                              case canary_b:
                                  fprintf (pFile, "Bottom canary has been changed\n");
                              break;
                              case bad_hash:
                                  fprintf (pFile, "Hash is not good\n");
                              break;
            )
        }

        ON_DEBUG (fprintf (pFile, "hash is now: %d\n", str->hash);)
        fprintf (pFile, "}\n\n\n");
    }

    if (!is_silent && str->is_valid != 0) {
        printf ("Abort was made. For info look in stack_dump.txt\n");
        abort ();
    }
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Answers if stack is valid or not
//!
//! @param [in] stk - pointer to stack
//! @return if everything is OK
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
bool Stack_okey (Stack_t* stk) {
    if (stk == nullptr) {
        return false;
    }

    if (stk->data == nullptr) {
        stk->is_valid = nulldata;
        return false;
    }

    ON_DEBUG (if (ST_HASH (stk) != stk->hash) {
        stk->is_valid = bad_hash;
        return false;
    }
    )

    if (stk->size > stk->max_size) {
        stk->is_valid = size_out_of_max;
        return false;
    }

    if (stk->max_size < 10) {
        return false;
    }

    if (stk->size < 0) {
        return false;
    }

    if (stk->is_valid != ok) {
        return false;
    }

    ON_DEBUG ( if (stk->canary_t != 0) {
        stk->is_valid = canary_t;
        return false;
    }
    )

    ON_DEBUG ( if (stk->canary_t != 0) {
        stk->is_valid = canary_b;
        return false;
    }
    )
    return true;
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Makes hash of a structure
//!
//! @param [in] stk - pointer to first element
//! @param [in] seed - to make different results
//! @return hash of stack
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
int hash_it (void* stk, int number, int seed) {
    void* ptr = stk;
    int hash = seed;

    for (int i = 0; i < number; i++) {
        hash =  (hash * 0x239b961b) ^ (int)*((char* )ptr + i);
    }

    return hash;
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Prints all bytes of stack
//!
//! @param [in] stk - pointer to stack
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
void print_stack (Stack_t* stk) {
    void* ptr = stk;
    for (int i = 0; i < sizeof(Stack_t); i++) {
        printf("stk[%d] = %d\n", i,  *((char *) ptr + i));
        if (i == 3 || i == 11 || i == 19 || i == 23 || i == 27 || i == 31 || i == 35) {
            printf ("\n");
        }
    }
    printf ("<<------------------------------------------->>\n\n");
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Sasha's fake to my stack
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
void unit_Stack1 (void) {
    Stack_t stk = {};
    St_construct (&stk);

    St_push(&stk, 6);
    St_push(&stk, 5);
    St_push(&stk, 6);
    St_push(&stk, 3);

    //go to place, where max_size is situated
    char* Evil = (char*)&stk + sizeof(int) + sizeof(char*) + sizeof(Elem_t*) ON_DEBUG(+ sizeof(int));
    *((int*) Evil) += 13; // max_size += 13
    Dump_f (&stk, __FILE__, __LINE__, __PRETTY_FUNCTION__, "unit_Stack1 bad");

    if (Stack_okey(&stk))
        printf ("\nunit_Stack1 error\nstk->max_size = %d, expexted %d\nNo errors registered\n", stk.max_size, 10);
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Sasha's fake to my stack
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
void unit_Stack2 (void) {
    Stack_t stk = {};
    St_construct (&stk);

    St_push(&stk, 4);
    St_push(&stk, 3);
    St_push(&stk, 7);
    St_push(&stk, 1);

    char* Evil = (char*)&stk + sizeof(int) + sizeof(char*);
    Elem_t* fake = (Elem_t*)Evil;
    stk.data = fake;

    Dump_f (&stk, __FILE__, __LINE__, __PRETTY_FUNCTION__, "unit_Stack2 bad");
    if (Stack_okey(&stk))
        printf ("\nunit_Stack2 error\nstk.data = {%d, %d, %d, %d}, expected {%d, %d, %d, %d}\nNo errors registered\n\n",
                stk.data[0],
                stk.data[1],
                stk.data[2],
                stk.data[3],
                4,
                3,
                7,
                1);
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Sasha's fake to my stack
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
void unit_Stack3 (void) {
    Stack_t stk = {};
    St_construct (&stk);

    for (int i = 0; i < 4; i++) {
        St_push(&stk, i);
    }

    stk.data[2]= 5;
    Dump_f (&stk, __FILE__, __LINE__, __PRETTY_FUNCTION__, "unit_Stack4 bad");
    if (Stack_okey(&stk))
        printf ("unit_Stack3 error\nBad guy was able to change element in data manually\n");
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Sasha's fake to my stack
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
void unit_Stack4 (void) {
    Stack_t stk = {};
    St_construct (&stk);

    for (int i = 0; i < 30; i++) {
        St_push(&stk, i);
    }

    if (stk.size != 30)
        printf ("unit_Stack4 error 1\n");

    if (stk.max_size != 36)
        printf ("unit_Stack4 error 2\n");

    for (int i = 0; i < 30; i++) {
        int a = St_pop (&stk);
        if (a != 29-i)
            printf ("unit_Stack4 error 3, %d != %d\n", 29-i, a);
    }
    
}

#endif
