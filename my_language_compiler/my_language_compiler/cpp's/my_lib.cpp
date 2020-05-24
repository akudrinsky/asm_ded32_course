//
//  my_lib.cpp
//  macho-compile
//
//  Created by Alexey Kudrinsky on 15.05.2020.
//  Copyright © 2020 Alexey Kudrinsky. All rights reserved.
//

#include <stdio.h>
#include "../headers/my_lyb.h"
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Converts integer to string
//!
//! @param [in] a - integer value
//!
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
void itoa_ (int a, char* s)
{
    if (s[0] != '\0')                                //s[] must be empty
        printf ("String must be empty!\n");
    
    int i = 0;
    bool ispositive = true;
    
    if (a < 0){
        a = -a;
        ispositive = false;
    }
    int ndig = (a==0? 1: int (log10 (a) + 1));           //number of digits in integer value
    
    s[0] = '-';
    int digit_place = 1;                                     //digit_place - what digit are we now converting
    for (int j=1; j<ndig; j++)
        digit_place = digit_place * 10;
    
    bool beginning = true;                                  //start to read number
    for (i=ndig; i>0; --i) {
        if (ispositive && beginning) {                        //then there is no minus
            ++i;
            beginning = false;
        }
        if (digit_place == 0)                                   //then because the number was positive, there is one "false" digit left
            break;
        s[ndig-i+1] = (a / digit_place) + '0';
        a = a % digit_place;                                   //switch to next digit
        digit_place = digit_place / 10;
    }
    
    
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Converts string to integer
//!
//! @param [in] s - array of char
//!
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
int atoi_ (char* s, bool* is_good)
{
    assert (s != nullptr);
    
    *is_good = true;
    
    char c = '0';                                               //code of symbol in s minus code of zero (then all numerals have them in code, all non-numerals don't)
    int a = 0, i = 0;
    bool isnumber = true, ispositive = true;                   //assume it is a positive number
    if (s[0] == '-')
        ispositive = false;
    
    
    for (i=0; i<strlen(s); ++i)                            //start from non-minus item
    {
        if (!ispositive && i == 0)
            ++i;
        
        c = s[i] - '0';
        
        if (c < 10 && c > -1)                //then it is a number
            a = a * 10 + c;
        else                                //then it is not a number, so isnumber = false
        {
            isnumber = false;               //it is not a number
            break;
        }
    }
    
    if (!isnumber)                              //it is not a number
    {
        *is_good = false;
        return 0;
    }
    
    if (ispositive)
        return a;
    else
        return (-a);
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Makes machine code (aka binary number system) from signed int (aka usual number system)
//!
//! @param [in] number - int number to convert
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
char* int_to_code (int number) {
    
    char* machine = (char*)calloc (sizeof (int), sizeof (char));
    
    machine[0] = number & 255;
    machine[1] = (number & 65280) >> 8;
    machine[2] = (number & 16711680) >> 16;
    machine[3] = (number & 4278190080) >> 24;
    
    return machine;
}

int find_bool_length (int path, const int max_depth = 10) {
    int num = 0;
    
    for (num = max_depth; num > 0; num--) {
        if ((path >> num) == 1) {
            break;
        }
    }
    
    if (num == max_depth)
        err_info ("maybe you had overwritten max_depth while finding digits number\n");
    
    return num + 1;
}

void err_info (const char* str, const char* file) {
    ASSERT (file != nullptr)
    ASSERT (str != nullptr)
    FILE* pFile = fopen (file, "a");
    ASSERT (pFile != nullptr)
    
    fprintf (pFile, "%s", str);
    
    fclose (pFile);
}

void say_it (const char* message, bool need_print, const char* voice) {
    if (need_print) {
        printf ("%s", message);
    }
    
    int str_len = (int)strnlen (message, 1000) + 20;
    char* command = (char* )calloc (str_len, sizeof (char));
    
    strncat (command, "say -v ", str_len);
    strncat (command, voice, str_len);
    strncat (command, " '", str_len);
    strncat (command, message, str_len);
    strncat (command, "'", str_len);
    
    //execute command with the help of terminal
    system (command);
    free (command);
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Prints text console
//!
//! @param [in] format - pointer to line with a format of output
//! @param [in] pFile - pointer to file where the text exists
//!
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
#include <stdarg.h>
char for_print[100] = {};
void print (char* format, ...) {
    va_list argptr = {};
    va_start (argptr, format);
    
    char* format_c = nullptr;                                         //pointer to character in format line
    
    for (format_c = format; *format_c; ++format_c) {
        if (*format_c != '%') {
            sprintf (for_print, "%c", *format_c);
            continue;                                               //go to next character, this was printed
        }
        switch (*(++format_c)) {
            case 'd': {
                int ival = va_arg (argptr, int);
                sprintf (for_print, "%d", ival);
            }   break;
                
            case 'b': {
                for (int i = sizeof(int) * 8; i >= 0; --i) {
                    //problem           sprintf (for_print, "%d", bit_return (ival, i));
                }
            } break;
                
            case 'f': {
                double dval = va_arg (argptr, double);
                sprintf (for_print, "%f", dval);
            } break;
                
            case 's': {
                for (char* sval = va_arg (argptr, char *); *sval; ++sval)
                    sprintf (for_print, "%c", *sval);
            }   break;
                
            case 'c': {
                char sval = va_arg (argptr, int);
                sprintf (for_print, "%c", sval);
            }   break;
                
            case '%': {
                sprintf (for_print, "%%");
                sprintf (for_print, "%%");
            }   break;
                
            default: {
                sprintf (for_print, "%c", *format_c);
            }
                break;
        }
    }
    va_end (argptr); /* очистка, когда все сделано */
    
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Reads text to line
//!
//! @param [in] pFile - pointer to file where the text exists
//!
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
char* read_text (FILE* pFile) {
    ASSERT (pFile != nullptr);
    rewind (pFile);
    
    int n = count_symbols (pFile);
    
    char* line = (char* )calloc (n, sizeof (char));
    fread (line, sizeof(char), n, pFile);
    
    return line;
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Tries to make the order of lines as it was in the beginning
//!
//! @param [in] line - pointer to line, where to store file
//! @param [in] pFile - pointer to file where the text exists
//!
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
str* read_txt (char* *line, FILE* pFile, int* nlines, bool with_empty) {
    ASSERT (line != nullptr);
    ASSERT (pFile != nullptr);
    rewind (pFile);
    
    int n = count_symbols (pFile);
    
    *line = (char* )calloc (n+1, sizeof (char));
    fread (*line, sizeof(char), n, pFile);
    (*line)[n] = '\n';
    
    replace_c (*line, n+1, '\n', '\0');                   //change all \n to \0
    
    *nlines = count_lines (*line, n+1);
    str* text = (str* )calloc (*nlines, sizeof (str));
    ASSERT (text != nullptr);
    
    char c_prev = '\0';
    char* buf = *line;
    str *ptr = text;
    
    while ((buf - *line) < n + 1) {
        if (c_prev == '\0') {                       //big flow of conditions to better understand the code
            if (with_empty) {
                (*ptr).letters = buf;
                (*ptr).len = (int) strlen (buf);
                ++ptr;
                ASSERT (ptr != nullptr);
            }
            else {
                if (*(buf) != '\0') {
                    (*ptr).letters = buf;
                    (*ptr).len = (int) strlen (buf);
                    ++ptr;
                }
            }
        }
        c_prev = *buf;
        ++buf;
        ASSERT (buf != nullptr);
    }
    
    *nlines = (int)(ptr - text);
    
    return text;
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Makes array of pointers to strings (str) of lines of a text
//!
//! @param [in] line where text is stosred
//! @param [in] nlines - number of lines in text
//! @return - array of pointers to lines of a text (as a str)
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
str* remake_text (char* line, int nlines) {
    str* text = (str* )calloc (nlines, sizeof (str));
    ASSERT (text != nullptr);
    
    char c_prev = '\0';
    char* buf = line;
    str *ptr = text;
    
    while ((ptr - text) < nlines + 1) {
        if (c_prev == '\0') {                       //big flow of conditions to better understand the code
            (*ptr).letters = buf;
            (*ptr).len = (int) strlen(buf);
            ++ptr;
            ASSERT (ptr != nullptr);
        }
        c_prev = *buf;
        ++buf;
        ASSERT (buf != nullptr);
    }
    
    return text;
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Tries to make the order of lines as it was in the beginning
//!
//! @param [in] first_line - pointer to first line in the text
//!
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
void make_orig_txt (str* first_line, str* last_line) {
    ASSERT (first_line != nullptr);
    ASSERT (last_line != nullptr);
    
    str pivot = *first_line;
    str* l_hold = first_line;
    str* r_hold = last_line;
    
    while (first_line <= last_line) {
        while ((*first_line).letters < pivot.letters && first_line < r_hold)
            ++first_line;
        while ((*last_line).letters > pivot.letters && last_line > l_hold)
            --last_line;
        
        if (first_line <= last_line) {
            std::swap (*first_line, *last_line);
            ++first_line;
            --last_line;
        }
    }
    
    // recursive if there is something to sort
    if (last_line > l_hold)
        make_orig_txt (l_hold, last_line);
    if (r_hold > first_line)
        make_orig_txt (first_line, r_hold);
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Compares two lines
//!
//! @param [in] s1 - first one
//! @param [in] s2 - second one
//!
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
int Strcmp (const str s1, const str s2) {
    ASSERT (s1.letters != nullptr);
    ASSERT (s2.letters != nullptr);
    
    int min_len_s = 0;               //min length of s1 and s2
    
    for (int i = 0; i < min_len_s; ++i) {
        if (s1.letters[i] > s2.letters[i])
            return 1;
        if (s1.letters[i] < s2.letters[i])
            return -1;
    }
    
    if (s1.len < s2.len)
        return -1;
    if (s1.len > s2.len)
        return 1;
    
    return 0;
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Compares two lines from the end (ignoring all non-letter characters)
//!
//! @param [in] s1 - first one
//! @param [in] s2 - second one
//!
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
int back_cmp (const str s1, const str s2) {
    ASSERT (s1.letters != nullptr);
    ASSERT (s2.letters != nullptr);
    
    int i = s1.len , j = s2.len;
    
    while (i >= 0 && j >= 0) {
        while (!is_letter (s1.letters[i]) && i >= 0)
            --i;
        while (!is_letter (s2.letters[j]) && j >= 0)
            --j;
        
        
        if (s1.letters[i] > s2.letters[j])
            return 1;
        if (s1.letters[i] < s2.letters[j])
            return -1;
        --i;
        --j;
    }
    
    if (i == -1 && j == -1)
        return 0;
    if (i == -1)
        return -1;
    else
        return 1;
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Writes if the character is a letter or not
//!
//! @param [in] ch - character
//!
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
bool is_letter (char ch) {
    if (ch >= 'a' && ch <= 'z')
        return true;
    return (ch >= 'A' && ch <= 'Z');
}

bool is_digit (char ch) {
    return (ch >= '0' && ch <= '9');
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Writes if the character is a number or not
//!
//! @param [in] str - character
//!
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
bool is_num (const char* str) {
    if ((str[0] < '0' || str[0] > '9') && str[0] != '-') {
        //printf ("first\n");
        return false;
    }
    if (strncmp (str, "-", 3) == 0) {
        return false;
    }
    bool was_dot = false;
    for (int i = 1; i < strlen (str); ++i) {
        if ((str[i] < '0' || str[i] > '9') && str[i] != '.') {
            //printf ("not first\n");
            return false;
        }
        if (str[i] == '.' && was_dot) {
            return false;
        }
        if (str[i] == '.') {
            was_dot = true;
        }
    }
    
    return true;
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Sorts index array so as to get the alphabetic order of coresponding array of strings and writes them to output.txt
//!
//! @param [in] first_line - pointer to first line in text to sort
//! @param [in] last_line - pointer to last line in text to sort
//! @param [in] cmp - way to sort
//!
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
void qsort_index (str* first_line, str* last_line, int (*cmp)(const str, const str)) {
    ASSERT (first_line != nullptr);
    ASSERT (last_line != nullptr);
    
    str pivot = *first_line;
    str* l_hold = first_line;
    str* r_hold = last_line;
    
    while (first_line <= last_line) {
        while (cmp (*first_line, pivot) == -1 && first_line < r_hold)
            ++first_line;
        ASSERT (first_line != nullptr);
        
        while (cmp (*last_line, pivot) == 1 && last_line > l_hold)
            --last_line;
        ASSERT (last_line != nullptr);
        
        if (first_line <= last_line) {
            Swap (first_line, last_line);
            ++first_line;
            --last_line;
            ASSERT (first_line != nullptr);
            ASSERT (last_line != nullptr);
        }
    }
    
    // recursive if there is something to sort
    if (last_line > l_hold)
        qsort_index (l_hold, last_line, cmp);
    if (r_hold > first_line)
        qsort_index (first_line, r_hold, cmp);
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Swaps two pointers on strings
//!
//! @param [in] ptr1 - first pointer
//! @param [in] ptr2 - second pointer
//!
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
void Swap (str* ptr1, str* ptr2) {
    ASSERT (ptr1 != nullptr);
    ASSERT (ptr2 != nullptr);
    
    str tmp = *ptr1;
    *ptr1 = *ptr2;
    *ptr2 = tmp;
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Writes text to output.txt
//!
//! @param [in] text - array of strings
//!
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
void output_text (str *text) {
    FILE* pFile = fopen ("output.txt", "ab");
    ASSERT (pFile != nullptr);
    ASSERT (text != nullptr);
    
    while ((*text).letters != nullptr) {
        fputs ((*text).letters, pFile);
        fputs ("\n", pFile);
        ++text;
    }
    fputs ("\n___________________________End____________________________\n\n", pFile);
    
    fclose (pFile);
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Deletes line from array of strings
//!
//! @param [in] text - array of strings
//! @param [in] num_line - line to drop out (starts with 0)
//! @param [in] nlines - number of lines in array
//!
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
void drop_line (str *text, int num_line, int* nlines) {
    ASSERT (text != nullptr);
    ASSERT (nlines != nullptr);
    
    for (int i = num_line; i < *nlines-1; ++i) {
        text[i] = text[i+1];
    }
    *nlines -= 1;
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Deletes empty ("") lines from array of strings
//!
//! @param [in] text - array of strings
//! @param [in] nlines - number of lines in array
//!
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
void drop_empty_lines (str *text, int *nlines) {
    ASSERT (text != nullptr);
    ASSERT (nlines != nullptr);
    
    int i = 0;
    while (i < *nlines){
        if ((text[i]).letters[0] == '\0') {
            drop_line (text, i, nlines);
            --i;
        }
        ++i;
    }
    text[*nlines].letters = nullptr;
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Counts all symbols in text file
//!
//! @param [in] file - pointer to text file
//!
//!
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
int count_symbols (FILE* file) {
    ASSERT (file != nullptr);
    
    fseek (file, 0, SEEK_END);
    int n_symbols = (int) ftell (file);
    rewind (file);
    return n_symbols;
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Reads file to array of lines with no free space (non-rectangular form)
//!
//! @param [in] line - text file with in one line (where '\n' -> '\0' by read_txt())
//! @param [in] n_symbols - number of symbols in line (corresponding text file)
//!
//!
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
int count_lines (const char* line, int n_symbols) {
    ASSERT (line != nullptr);
    
    int number = 1;
    for (int i = 0; i < n_symbols; ++i) {
        if (line[i] == '\0' || line[i] == '\n') {
            ++number;
        }
    }
    return number;
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Change all prev symbols to now
//! @param [in] line - text file wittn in one line
//! @param [in] prev - symbol to change
//! @param [in] now - symbol to write instead of prev
//!
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
void replace_c (char* line, int n_symbols, char prev, char now) {
    ASSERT (line != nullptr);
    
    for (int i = 0; i < n_symbols; ++i) {
        if (line[i] == prev) {
            line[i] = now;
        }
    }
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Puts line to the screen
//!
//! @param [in] string - pointer to line to put out
//!
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
void puts_ (char* string) {
    ASSERT (string != nullptr);
    printf ("%s", string);
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! deletes empty lines from text file
//!
//! @param [in] file - pointer to text file
//!
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
void delete_empty_strings (FILE* file) {
    ASSERT (file != nullptr);
    FILE* can;
    can = fopen ("can.txt", "w");
    ASSERT (can != nullptr);
    
    char c = '0', cprev = '0';
    while ((c = fgetc (file)) != EOF) {
        if (!(cprev == '\n' && c == '\n')) {
            fputc (c, can);
            cprev = c;
        }
    }
    fclose (file);
    fclose (can);
    
    file = fopen ("input.txt", "w");
    can = fopen ("can.txt", "r");
    while ((c = fgetc (can)) != EOF) {
        fputc (c, file);
    }
    fclose (file);
    fclose (can);
    
    file = fopen ("input.txt", "r");
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! counts lines in file
//!
//! @param [in] file - pointer to text file
//!
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
int count_lines_txt (FILE* file){
    ASSERT (file != nullptr);
    
    rewind (file);
    int number = 1;
    char c = '0';
    while ((c = fgetc(file)) != EOF){
        if (c == '\n')
            ++number;
    }
    rewind (file);
    return number;
}


//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Gets line from file; returns the way line was absorbed
//!
//! @param [in] max_size - max size of line s[]
//! @param [in] file - pointer to text file to get line from
//!
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
int fgets_ (char* str, int max_size, FILE* file)
{
    ASSERT (str != nullptr);
    ASSERT (file != nullptr);
    
    char c = '0';
    bool is_space = true;           //is there enough space?
    bool not_end = true;            //is it the end of line?
    int i = 0;
    
    for (i = 0; i < max_size; ++i) {     //fill every element with \0
        str[i] = '\0';
    }
    i = 0;
    
    while (is_space && not_end){
        c = fgetc(file);
        if (c == EOF){
            str[i] = '\n';
            break;
        }
        if (i<max_size){
            str[i] = c;
            ++i;
        }
        else {
            is_space = false;
        }
        if (c=='\n') {
            not_end = false;
        }
    }
    if (c == EOF)
        return 0;
    if (c == '\n')
        return 1;
    if (!is_space)
        return 2;
    
    return 0;
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Gets minimum if two integer elements
//!
//! @param [in] a - integer value
//! @param [in] b - integer value
//!
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
int min_ (int a, int b) {
    return (b > a) ? a : b;
}
