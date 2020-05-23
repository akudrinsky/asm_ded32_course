//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//!
//! It is my implementation of list
//!
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐

#ifndef LIST_WAS_DEFINED
#define LIST_WAS_DEFINED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define DEBUG
#ifdef DEBUG
    #define ASSERT(cond)\
    if (!(cond)) {\
        printf ("Assert failure: < %s > in file %s, line %d, function %s\n", #cond, __FILE__, __LINE__, __PRETTY_FUNCTION__);\
        abort();\
    }
#else
    #define ASSERT(cond) ;
#endif

#define Lt_construct(lst) List_Construct(&lst, #lst);

//#define LIST_NEED_CHECKS
#ifdef LIST_NEED_CHECKS
#define List_OK(lst)                                   \
List_okey (&lst);                                  \
Dump_f (&lst, __FILE__, __LINE__, __PRETTY_FUNCTION__);
#else
#define List_OK(lst) ;
#endif

typedef int Elem_t;

enum lt_errors_c {
    ok              = 0,
    nulldata        = 1,
    size_out_of_max = 2,
    no_memory       = 3,
    calling_nowhere = 4,
    destructed      = 5,
    strange_value   = 6,
};

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! List structure
//!
//! @param [in] name - name of list
//! @param [in] data - array of list values
//! @param [in] next - array, where you can understand which physical number is after a given one
//! @param [in] prev - array, where you can understand which physical number is before a given one
//! @param [in] head - physical number of first (logically) element
//! @param [in] tail - physical number of last (logically) element
//! @param [in] free - first free element in list
//! @param [in] size - number of elements in list
//! @param [in] is_sorted - if list physical numbers correspond to logical ones
//! @param [in] status - if it is OK
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐

#pragma pack(push, 1)
struct List_t {
    const char* name;
    Elem_t* data;
    Elem_t* next;
    Elem_t* prev;
    int head;
    int tail;
    int free;
    int size;
    int max_size;
    bool is_sorted;
    lt_errors_c status;
};
#pragma pack(pop)
void err_info (const char* str, const char* file = "errors_info.txt");

int List_Construct (List_t* lst, const char* lst_name = "lst", const int starting_size = 10000);
int Lt_destruct (List_t* lst);

void Dump_f (const List_t* lst, const char* file, int line,
             const char* func_name, const char* info = "assert",
             bool is_silent = false, bool no_corr = true,
             const char* file_name = "list_dump.txt");
void list_graph (List_t* lst, const char* pict_name = "list.png", const char* pict_type = "png");
bool List_okey (List_t* lst);

int insert_after (List_t* lst, const int pos, const Elem_t elem);
int insert_before (List_t* lst, const int pos, const Elem_t elem);

int delete_el (List_t* lst, const int pos);
int delete_after (List_t* lst, const int pos);
int delete_before (List_t* lst, const int pos);

bool phys_match_log (List_t* lst);
int val_in_list (List_t* lst, const int value, const bool need_log = false);

void list_test ();

#endif
