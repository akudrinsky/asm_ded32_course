//
//  hashtable.cpp
//  hashtable
//
//  Created by Alexey Kudrinsky on 14.04.2020.
//  Copyright © 2020 Alexey Kudrinsky. All rights reserved.
//

#ifndef HASHTABLE_CPP_WAS_DEFINED
#define HASHTABLE_CPP_WAS_DEFINED

#include "hashtable.h"
//#include "murmur.cpp"

#define OPTIMIZE

#ifdef OPTIMIZE
    #define strlen_asm_be_sure_you_know_what_you_do(str) \
    __asm__ (   ".intel_syntax noprefix\n"          \
                "pxor xmm0, xmm0\n"                 \
                "pcmpistri xmm0, xmmword ptr[rsi], 1000b\n"\
                ".att_syntax prefix\n"              \
                :   "=c"(len)                       \
                :   "S"(str)                        \
                :   "cc", "xmm0"                    \
    );
#else
    #define strlen_asm_be_sure_you_know_what_you_do(str) len = bad_strlen (str);
#endif

const unsigned int crc32_table[256] = {
    0x00000000, 0x77073096, 0xEE0E612C, 0x990951BA,
    0x076DC419, 0x706AF48F, 0xE963A535, 0x9E6495A3,
    0x0EDB8832, 0x79DCB8A4, 0xE0D5E91E, 0x97D2D988,
    0x09B64C2B, 0x7EB17CBD, 0xE7B82D07, 0x90BF1D91,
    0x1DB71064, 0x6AB020F2, 0xF3B97148, 0x84BE41DE,
    0x1ADAD47D, 0x6DDDE4EB, 0xF4D4B551, 0x83D385C7,
    0x136C9856, 0x646BA8C0, 0xFD62F97A, 0x8A65C9EC,
    0x14015C4F, 0x63066CD9, 0xFA0F3D63, 0x8D080DF5,
    0x3B6E20C8, 0x4C69105E, 0xD56041E4, 0xA2677172,
    0x3C03E4D1, 0x4B04D447, 0xD20D85FD, 0xA50AB56B,
    0x35B5A8FA, 0x42B2986C, 0xDBBBC9D6, 0xACBCF940,
    0x32D86CE3, 0x45DF5C75, 0xDCD60DCF, 0xABD13D59,
    0x26D930AC, 0x51DE003A, 0xC8D75180, 0xBFD06116,
    0x21B4F4B5, 0x56B3C423, 0xCFBA9599, 0xB8BDA50F,
    0x2802B89E, 0x5F058808, 0xC60CD9B2, 0xB10BE924,
    0x2F6F7C87, 0x58684C11, 0xC1611DAB, 0xB6662D3D,
    0x76DC4190, 0x01DB7106, 0x98D220BC, 0xEFD5102A,
    0x71B18589, 0x06B6B51F, 0x9FBFE4A5, 0xE8B8D433,
    0x7807C9A2, 0x0F00F934, 0x9609A88E, 0xE10E9818,
    0x7F6A0DBB, 0x086D3D2D, 0x91646C97, 0xE6635C01,
    0x6B6B51F4, 0x1C6C6162, 0x856530D8, 0xF262004E,
    0x6C0695ED, 0x1B01A57B, 0x8208F4C1, 0xF50FC457,
    0x65B0D9C6, 0x12B7E950, 0x8BBEB8EA, 0xFCB9887C,
    0x62DD1DDF, 0x15DA2D49, 0x8CD37CF3, 0xFBD44C65,
    0x4DB26158, 0x3AB551CE, 0xA3BC0074, 0xD4BB30E2,
    0x4ADFA541, 0x3DD895D7, 0xA4D1C46D, 0xD3D6F4FB,
    0x4369E96A, 0x346ED9FC, 0xAD678846, 0xDA60B8D0,
    0x44042D73, 0x33031DE5, 0xAA0A4C5F, 0xDD0D7CC9,
    0x5005713C, 0x270241AA, 0xBE0B1010, 0xC90C2086,
    0x5768B525, 0x206F85B3, 0xB966D409, 0xCE61E49F,
    0x5EDEF90E, 0x29D9C998, 0xB0D09822, 0xC7D7A8B4,
    0x59B33D17, 0x2EB40D81, 0xB7BD5C3B, 0xC0BA6CAD,
    0xEDB88320, 0x9ABFB3B6, 0x03B6E20C, 0x74B1D29A,
    0xEAD54739, 0x9DD277AF, 0x04DB2615, 0x73DC1683,
    0xE3630B12, 0x94643B84, 0x0D6D6A3E, 0x7A6A5AA8,
    0xE40ECF0B, 0x9309FF9D, 0x0A00AE27, 0x7D079EB1,
    0xF00F9344, 0x8708A3D2, 0x1E01F268, 0x6906C2FE,
    0xF762575D, 0x806567CB, 0x196C3671, 0x6E6B06E7,
    0xFED41B76, 0x89D32BE0, 0x10DA7A5A, 0x67DD4ACC,
    0xF9B9DF6F, 0x8EBEEFF9, 0x17B7BE43, 0x60B08ED5,
    0xD6D6A3E8, 0xA1D1937E, 0x38D8C2C4, 0x4FDFF252,
    0xD1BB67F1, 0xA6BC5767, 0x3FB506DD, 0x48B2364B,
    0xD80D2BDA, 0xAF0A1B4C, 0x36034AF6, 0x41047A60,
    0xDF60EFC3, 0xA867DF55, 0x316E8EEF, 0x4669BE79,
    0xCB61B38C, 0xBC66831A, 0x256FD2A0, 0x5268E236,
    0xCC0C7795, 0xBB0B4703, 0x220216B9, 0x5505262F,
    0xC5BA3BBE, 0xB2BD0B28, 0x2BB45A92, 0x5CB36A04,
    0xC2D7FFA7, 0xB5D0CF31, 0x2CD99E8B, 0x5BDEAE1D,
    0x9B64C2B0, 0xEC63F226, 0x756AA39C, 0x026D930A,
    0x9C0906A9, 0xEB0E363F, 0x72076785, 0x05005713,
    0x95BF4A82, 0xE2B87A14, 0x7BB12BAE, 0x0CB61B38,
    0x92D28E9B, 0xE5D5BE0D, 0x7CDCEFB7, 0x0BDBDF21,
    0x86D3D2D4, 0xF1D4E242, 0x68DDB3F8, 0x1FDA836E,
    0x81BE16CD, 0xF6B9265B, 0x6FB077E1, 0x18B74777,
    0x88085AE6, 0xFF0F6A70, 0x66063BCA, 0x11010B5C,
    0x8F659EFF, 0xF862AE69, 0x616BFFD3, 0x166CCF45,
    0xA00AE278, 0xD70DD2EE, 0x4E048354, 0x3903B3C2,
    0xA7672661, 0xD06016F7, 0x4969474D, 0x3E6E77DB,
    0xAED16A4A, 0xD9D65ADC, 0x40DF0B66, 0x37D83BF0,
    0xA9BCAE53, 0xDEBB9EC5, 0x47B2CF7F, 0x30B5FFE9,
    0xBDBDF21C, 0xCABAC28A, 0x53B39330, 0x24B4A3A6,
    0xBAD03605, 0xCDD70693, 0x54DE5729, 0x23D967BF,
    0xB3667A2E, 0xC4614AB8, 0x5D681B02, 0x2A6F2B94,
    0xB40BBE37, 0xC30C8EA1, 0x5A05DF1B, 0x2D02EF8D
};

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Hash-table construct
//!
//! @param [in] hast - pointer to hash-table
//! @param [in] hast_maxsize - max elements which it can theoretically store without intersections
//! @return error code
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
int hast_construct (hashtable* hast, int (*hash_f) (const char*, const int, const int), const int hast_maxsize, bool is_loud) {
    ASSERT (hast != nullptr)
    
    hast->rows = (List_t*)calloc (hast_maxsize, sizeof (List_t));
    if (hast->rows == nullptr) {
        if (is_loud)
            err_info ("We have no memory left!\n");
        return no_memory;
    }
    for (int i = 0; i < hast_maxsize; ++i) {
        Lt_construct (hast->rows[i])
        if (hast->rows[i].status != ok) {
            if (is_loud)
                err_info ("One of lists in hash table was not allocated\n");
            return no_memory;
        }
    }
    hast->size = 0;
    hast->max_size = hast_maxsize;
    hast->status = ok;
    
    hast->hashfunc = hash_f;
    
    hast_OK (*hast)
    
    return 0;
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Hash-table destruct
//!
//! @param [in] hast - pointer to hash-table
//! @param [in] hast_maxsize - max elements which it can theoretically store without intersections
//! @return error code
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
int hast_destruct (hashtable* hast, const int hast_maxsize, bool is_loud) {
    ASSERT (hast != nullptr)
    
    if (hast->status == destructed) {
        if (is_loud)
            err_info ("Trying to destruct hash table which had been destructed\n");
        return destructed;
    }
    for (int i = 0; i < hast_maxsize; ++i) {
        Lt_destruct (&hast->rows[i]);
    }
    free (hast->rows);
    hast->size = 0;
    hast->max_size = 0;
    hast->status = destructed;
    
    hast->hashfunc = nullptr;
    
    return 0;
}

int hash_it (const char* stk, const int number, const int seed) {
    const char* ptr = stk;
    int hash = seed;
    
    for (int i = 0; i < number; i++) {
        hash =  (hash * 0x239b961b) ^ (int)*((char* )ptr + i);
    }
    
    return hash;
}

int hash_it_strlen (const char* stk, const int number, const int seed) {
    char* ptr = (char*) stk;
    
    return strlen (ptr);
}

int hash_it_sum (const char* stk, const int number, const int seed) {
    const char* ptr = stk;
    int hash = seed;
    
    for (int i = 0; i < number; i++) {
        hash += *((char* )ptr + i);
    }
    
    return hash;
}

int hash_it_first (const char* stk, const int number, const int seed) {
    return *((char*) stk);
}

int hash_it_rand (const char* stk, const int number, const int seed) {
    return rand() % 200 + 100;
}

void count_inter (hashtable* hast, FILE* pFile) {
    for (int i = 0; i < hast->max_size; ++i) {
        fprintf (pFile, "%d,", hast->rows[i].size);
    }
    fprintf (pFile, "\n");
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Adds new element to hash-table
//!
//! @param [in] hast - pointer to hash-table
//! @param [in] str - "index" of element to add
//! @param [in] value - value to add
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
void add_element (hashtable* hast, const char* str, Elem_t value) {
    ASSERT (hast != nullptr)
    ASSERT (str != nullptr)
    //printf ("a\n");
    int len = 0;
    strlen_asm_be_sure_you_know_what_you_do (str);
    
    int str_hash = hast->hashfunc (str, len, hash_seed_for_hast);
    //printf ("%d\n", str_hash);
    insert_after (&hast->rows[abs (str_hash) % hast->max_size], hast->rows[abs (str_hash) % hast->max_size].tail, value);
    hast->size++;
    
    hast_OK (*hast)
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Searches for value (or several) by str
//!
//! @param [in] hast - pointer to hash-table
//! @param [in] str - "index" of element to search by
//! @param [in] size - how many elements were by that exact string
//! @return pointer to first element with corresponding value
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
Elem_t* search_by_str (hashtable* hast, const char* str, int* size) {
    ASSERT (hast != nullptr)
    ASSERT (str != nullptr)
    ASSERT (size != nullptr)
    
    int len = 0;
    strlen_asm_be_sure_you_know_what_you_do (str);
    
    int str_hash = abs (hast->hashfunc (str, len, hash_seed_for_hast)) % hast->max_size;
    
    if (!hast->rows[str_hash].is_sorted) {
        phys_match_log(&hast->rows[str_hash]);
    }
    
    *size = hast->rows[str_hash].size;
    
    hast_OK (*hast)
    
    return hast->rows[str_hash].data + 1;                //because first row is for me, now for users
}

//! Says if value by str-index is inside hashtable
//!
//! @param [in] hast - pointer to hash-table
//! @param [in] str - "index" of element to search by
//! @param [in] value - value by that str-index
//! @return is / is not
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
bool is_in (hashtable* hast, const char* str, int value) {
    int size = 0;
    Elem_t* elements = search_by_str (hast, str, &size);
    
    for (int i = 0; i < size; ++i) {
        if (elements[i] == value) {
            return true;
        }
    }
    
    return false;
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Deletes element in hash-table
//!
//! @param [in] hast - pointer to hash-table
//! @param [in] str - "index" of element to delete
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
void delete_element (hashtable* hast, const char* str) {
    ASSERT (hast != nullptr)
    ASSERT (str != nullptr)
    
    int len = 0;
    strlen_asm_be_sure_you_know_what_you_do (str);
    
    int str_hash = abs (hast->hashfunc (str, len, hash_seed_for_hast)) % hast->max_size;
    
    List_t* lst = &hast->rows[str_hash];
    
    int starting_size = 10;
    for (int i = 1; i < starting_size; ++i) {
        lst->data[i] = 0;
        lst->next[i] = i + 1;
        lst->prev[i] = -1;
    }
    lst->next[starting_size - 1] = 0;
    
    lst->head = 0;
    lst->tail = 0;
    lst->free = 1;
    lst->size = 0;
    lst->max_size = starting_size;
    lst->is_sorted = true;
    lst->status = ok;
    
    hast_OK (*hast)
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Answers if hast is valid or not
//!
//! @param [in] hast - pointer to hash table
//! @return if everything is OK
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
bool hast_ok (hashtable* hast) {
    if (hast == nullptr) {
        return false;
    }
    
    if (hast->rows == nullptr) {
        hast->status = nulldata;
        return false;
    }
    
    if (hast->max_size < 10) {
        hast->status = size_out_of_max;
        return false;
    }
    
    if (hast->size < 0) {
        hast->status = size_out_of_max;
        return false;
    }
    
    if (hast->status != ok) {
        return false;
    }
    
    return true;
}

void hast_test_1 () {
    hashtable hast = {};
    hast_construct (&hast, &hash_it);
    
    add_element (&hast, "abcd", 133);
    add_element (&hast, "abcd", 233);
    add_element (&hast, "jhvh", 333);
    
    int size = 0;
    Elem_t* elements = search_by_str (&hast, "abcd", &size);
    
    if (elements[0] != 133) {
        printf ("hast_text_1 is not ok (1): %d\n", elements[0]);
    }
    if (elements[1] != 233) {
        printf ("hast_text_1 is not ok (2): %d\n", elements[1]);
    }
    
    delete_element (&hast, "abcd");
    
    elements = search_by_str (&hast, "abcd", &size);
    if (size != 0) {
        printf ("hast_text_1 is not ok (3): %d\n", size);
    }
    
    add_element (&hast, "abcd", 131);
    add_element (&hast, "abcd", 2331);
    add_element (&hast, "jhvh", 33312);
    
    elements = search_by_str (&hast, "abcd", &size);
    
    if (elements[0] != 131) {
        printf ("hast_text_1 is not ok (4): %d\n", elements[2]);
    }
    if (elements[1] != 2331) {
        printf ("hast_text_1 is not ok (5): %d\n", elements[3]);
    }
    
    hast_destruct (&hast);
}

int crc32_sse (const char* str, int len, const int seed) {
    int crc = seed;
    
    char* end       = (char*) str + ((len >> 2) << 2);
    char* final_end = end + (len & 0x3);                                      // last two digits
    
    while (str < end) {
        crc = _mm_crc32_u32 (crc, *(unsigned int*) str);
        str += 4;
    }
    
    while (str < final_end) {
        crc = _mm_crc32_u8 (crc, *(unsigned char*) str);
        ++str;
    }
    
    return crc ^ seed;
}

int crc32 (const char* str, int len, const int seed) {
    int crc = seed;
    while (--len)
        crc = (crc >> 8) ^ crc32_table[(crc ^ *str++) & 0xFF];
    
    return crc ^ seed;
}

void hast_funcs () {
    hashtable hast = {};
    hast_construct (&hast, &crc32_sse);
    FILE* input = fopen ("input.txt", "r");
    FILE* pFile = fopen ("comparing.txt", "w");
    char* str = (char*) calloc (20, sizeof (char));
    
    fprintf (pFile, "my hash,");
    for (int i = 0; i < 20000; ++i) {
        fscanf (input, "%s", str);
        add_element (&hast, str, i);
    }
    count_inter (&hast, pFile);
    hast_destruct (&hast);
    
    hast_construct (&hast, &hash_it_strlen);
    rewind (input);
    
    fprintf (pFile, "strlen,");
    for (int i = 0; i < 20000; ++i) {
        fscanf (input, "%s", str);
        add_element (&hast, str, i);
    }
    count_inter (&hast, pFile);
    hast_destruct (&hast);
    
    hast_construct (&hast, &hash_it_sum);
    rewind (input);
    
    fprintf (pFile, "sum,");
    for (int i = 0; i < 20000; ++i) {
        fscanf (input, "%s", str);
        add_element (&hast, str, i);
    }
    count_inter (&hast, pFile);
    hast_destruct (&hast);
    
    hast_construct (&hast, &hash_it_first);
    rewind (input);
    
    fprintf (pFile, "first,");
    for (int i = 0; i < 20000; ++i) {
        fscanf (input, "%s", str);
        add_element (&hast, str, i);
    }
    count_inter (&hast, pFile);
    hast_destruct (&hast);
    
    hast_construct (&hast, &hash_it_rand);
    rewind (input);
    
    fprintf (pFile, "rand,");
    for (int i = 0; i < 20000; ++i) {
        fscanf (input, "%s", str);
        add_element (&hast, str, i);
    }
    count_inter (&hast, pFile);
    hast_destruct (&hast);
    
    fclose (input);
    fclose (pFile);
}

void hast_test_effeciency () {
    hashtable hast = {};
    hast_construct (&hast, &crc32_sse);               //murmur_hash, murmur_asm

    std::ifstream input;
    input.open ("words.txt");
    
    const int n_str = 20000;     //0 .. 20000
    const int max_length = 20;
    char* *text = new char*[n_str];
    
    for (int i = 0; i < n_str; ++i)
        text[i] = new char[max_length];
    printf ("memory allocated...\n");
    
    for (int i = 0; i < n_str; ++i) {
        input >> text[i];
        add_element (&hast, text[i], i);
    }

    input.close ();
    printf ("ready for search stage...\n");
    
    unsigned long long end = ((unsigned long long) n_str) * 70000;
    
    for (unsigned long long i = 0; i < end; ++i) {
        int index = i % n_str;

        is_in (&hast, text[index], index);
    }
    printf ("search completed...\n");
    
    for (int i = 0; i < n_str; ++i)
        delete [] text[i];
    
    delete [] text;
}

#endif
