//
//  murmur.cpp
//  hashtable
//
//  Created by Alexey Kudrinsky on 14.04.2020.
//  Copyright © 2020 Alexey Kudrinsky. All rights reserved.
//
#include <stdio.h>
#include <assert.h>

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Makes hash of a structure
//!
//! @param [in] key - pointer to first element
//! @param [in] seed - to make different results
//! @return hash of structure
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐

int murmur_hash (const char* key, int len, int seed) {
    const unsigned int m = 0x5bd1e995;
    const int r = 24;
    
    unsigned int h = seed ^ len;
    
    const unsigned char * data = (const unsigned char *)key;
    unsigned int k;
    
    while (len >= 4) {
        k  = data[0];                       // по сути берем 4 подряд идущих из data
        k |= data[1] << 8;
        k |= data[2] << 16;
        k |= data[3] << 24;                 // .. взяли
        
        k *= m;
        k ^= k >> r;
        k *= m;
        
        h *= m;
        h ^= k;
        
        data += 4;
        len -= 4;
    }
    
    switch (len) {
        case 3:
            h ^= data[2] << 16;
        case 2:
            h ^= data[1] << 8;
        case 1:
            h ^= data[0];
            h *= m;
    };
    
    h ^= h >> 13;
    h *= m;
    h ^= h >> 15;
    
    return h;
}

int bad_strlen (const char* str) {
    assert (str != nullptr);
    
    char* cur = (char*) str;
    while (*cur != '\0') {
        ++cur;
    }
    return cur - str;
}
