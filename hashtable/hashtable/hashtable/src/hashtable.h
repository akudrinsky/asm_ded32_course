//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//!
//! It is my implementation of hash tables designed ONLY for words with length no more than 32 letters. That assumption makes it extremely fast.
//!
//! DO NOT USE WITH WORDS WHICH LENGTH IS MORE THAN 16 LETTERS
//!
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐

#ifndef HASHTABLE_WAS_DEFINED
#define HASHTABLE_WAS_DEFINED

#include <fstream>
#include "list.h"
#include <nmmintrin.h>

#define LOUD_HASTABLE
#ifdef LOUD_HASTABLE
    #define hast_OK(hast)\
    if (hast_ok(&(hast)) != true) {\
        ASSERT ((hast).status == ok);\
    }
#else
    #define hast_OK(hast) ;
#endif

#define Lt_construct(lst) List_Construct(&lst, #lst);

extern "C" int murmur_asm (const char*, int, int);
int murmur_hash (const char*, int, int);

const int hash_seed_for_hast = 144;

#pragma pack(push, 1)
struct hashtable {
    List_t* rows;
    int size;
    int max_size;
    int status;
    
    int (*hashfunc) (const char*, int, int);
};
#pragma pack(pop)

const int def_maxsize = 2000;

int hast_construct (hashtable* hast, int (*hash_f) (const char*, const int, const int), const int hast_maxsize = def_maxsize, bool is_loud = true);
int hast_destruct (hashtable* hast, const int hast_maxsize = def_maxsize, bool is_loud = true);

int murmur_hash (const char* key, int len, int seed = 10);
int hash_it (const char* stk, int number, int seed);
void count_inter (hashtable* hast, FILE* pFile);

void add_element (hashtable* hast, const char* str, Elem_t value);
Elem_t* search_by_str (hashtable* hast, const char* str, int* size);
bool is_in (hashtable* hast, const char* str, int value);
void delete_element (hashtable* hast, const char* str);

int bad_strlen (const char* str);

bool hast_ok (hashtable* hast);

void hast_test_1 ();
void hast_funcs ();
void hast_test_effeciency ();

#endif
