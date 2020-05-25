//
//  make_file.hpp
//  my_language_compiler
//
//  Created by Alexey Kudrinsky on 17.05.2020.
//  Copyright © 2020 Alexey Kudrinsky. All rights reserved.
//

#ifndef make_file_hpp
#define make_file_hpp

#include <string.h>
#include <stdlib.h>

#define DBG

#ifdef DBG
    #define ON_DEBUG(text) text;
#else
    #define ON_DEBUG(text) ;
#endif

struct unixthread {
    uint32_t    cmd;
    uint32_t    cmdsize;
    
    uint32_t    flavor;
    uint32_t    count;
    
    uint64_t    rax;
    uint64_t    rbx;
    uint64_t    rcx;
    uint64_t    rdx;
    uint64_t    rdi;
    uint64_t    rsi;
    uint64_t    rbp;
    uint64_t    rsp;
    uint64_t    r8;
    uint64_t    r9;
    uint64_t    r10;
    uint64_t    r11;
    uint64_t    r12;
    uint64_t    r13;
    uint64_t    r14;
    uint64_t    r15;
    uint64_t    rip;
    uint64_t    rflags;
    uint64_t    cs;
    uint64_t    fs;
    uint64_t    gs;
    
};

void make_file (const char* filename, const unsigned char* code, uint64_t len);

#endif /* make_file_hpp */
