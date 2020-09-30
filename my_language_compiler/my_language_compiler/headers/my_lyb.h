//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Several function to work with text and files
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐

#ifndef ADD_BETTER_ONEGIN
#define ADD_BETTER_ONEGIN

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

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>

struct str {
    const char* letters;
    int len;
};

void itoa_ (int a, char* s);
int atoi_ (char* s, bool* is_good);

char* int_to_code (int number);

void err_info (const char* str, const char* file = "errors_info.txt");
void say_it (const char* message, bool need_print = true, const char* voice = "Yuri");

char* read_text (FILE* pFile);
void make_orig_txt (str* first_line, str* last_line);
void replace_c (char* line, int n_symbols, char prev, char now);
int Strcmp (const str s1, const str s2);
void qsort_index (str* first_line, str* last_line, int (*cmp)(const str, const str));
void output_text (str *text);
void drop_empty_lines (str *text, int *nlines);
void drop_line (str *text, int num_line, int* nlines);
int count_symbols (FILE* file);
int count_lines (const char* line, int n_symbols);
str* read_txt (char* *line, FILE* pFile, int* nlines, bool with_empty = true);
str* remake_text (char* line, int nlines);
void Swap (str* ptr1, str* ptr2);
bool is_letter (char ch);
bool is_num (const char* str);
int back_cmp (const str s1, const str s2);
void print (char* format, ...);

void test_back_cmp (void);
void test_Strcmp (void);
void test_is_letter (void);
void test_qsort_index (void);
void test_make_orig_txt (void);
void test_count_symbols (void);
void test_count_lines (void);
void test_replace_c (void);

bool is_digit (char ch);

#endif
