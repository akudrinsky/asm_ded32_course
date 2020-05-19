//
//  x86_commands.hpp
//  my_language_compiler
//
//  Created by Alexey Kudrinsky on 17.05.2020.
//  Copyright © 2020 Alexey Kudrinsky. All rights reserved.
//

#ifndef x86_commands_hpp
#define x86_commands_hpp

/* Now I operate only with rax and rbx when needed (preference to rax).
 The rest are to add. */

/* For more info read Intel® 64 and IA-32 Architectures Software Developer’s Manual and try to disassemle some code on your machine. */

/* Every jump is considered to be rel 32 */

#include <vector>

namespace commands {
    struct label {
        unsigned char id    = -1;
        int offset          = -1;
    };
    
    std::vector<label> labels;                      // here will be data about all found labels
    std::vector<label> to_fill;                     // put when need to fill it later
    
    static constexpr unsigned char  ret[]               = {0xC3};
    static constexpr int            ret_size            = 1;
    
    static constexpr unsigned char  xor_rax_rax[]       = {0x48, 0x31, 0xc0};
    static constexpr int            xor_rax_rax_size    = 3;
    
    static constexpr unsigned char  xor_rbx_rbx[]       = {0x48, 0x31, 0xdb};
    static constexpr int            xor_rbx_rbx_size    = 3;
    
    static constexpr unsigned char  xor_rdx_rdx[]       = {0x48, 0x31, 0xd2};
    static constexpr int            xor_rdx_rdx_size    = 3;
    
    static constexpr unsigned char  add_rax_rbx[]       = {0x48, 0x01, 0xd8};
    static constexpr int            add_rax_rbx_size    = 3;
    
    static constexpr unsigned char  sub_rax_rbx[]       = {0x48, 0x01, 0xd8};
    static constexpr int            sub_rax_rbx_size    = 3;
    
    static constexpr unsigned char  idiv_rbx[]          = {0x48, 0x01, 0xd8}; /* Signed divide RDX:RAX by rbx, with result stored in RAX ← Quotient, RDX ← Remainder. */
    static constexpr int            idiv_rbx_size       = 3;
    
    static constexpr unsigned char  inc_rax[]           = {0x48, 0xff, 0xc0};
    static constexpr int            inc_rax_size        = 3;
    
    static constexpr unsigned char  dec_rax[]           = {0x48, 0xff, 0xc8};
    static constexpr int            dec_rax_size        = 3;
    
    static constexpr unsigned char  imul_rax_rbx[]      = {0x48, 0x0f, 0xaf, 0xc3};
    static constexpr int            imul_rax_rbx_size   = 4;
    
    static constexpr unsigned char  push_rax[]          = {0x50};
    static constexpr int            push_rax_size       = 1;
    
    static constexpr unsigned char  push_rbx[]          = {0x53};
    static constexpr int            push_rbx_size       = 1;
    
    static constexpr unsigned char  pop_rax[]           = {0x58};
    static constexpr int            pop_rax_size        = 1;
    
    static constexpr unsigned char  pop_rbx[]           = {0x5b};
    static constexpr int            pop_rbx_size        = 1;
    
    static constexpr unsigned char  cmp_rax_rbx[]       = {0x48, 0x39, 0xd8};
    static constexpr int            cmp_rax_rbx_size    = 3;
    
    static constexpr unsigned char  end_programm[]      = {0xb8, 0x01, 0x00, 0x00, 0x02, 0xbf, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x05}; /* mov rax, 0x2000001 (5) mov rdi, 0 (5) syscall (2) */
    static constexpr int            end_programm_size   = 12;
    
    static constexpr int            ja_size             = 6;
    static constexpr int            jae_size            = 6;
    static constexpr int            jb_size             = 6;
    static constexpr int            jbe_size            = 6;
    static constexpr int            je_size             = 6;
    static constexpr int            jne_size            = 6;
    static constexpr int            jmp_size            = 5;
    static constexpr int            call_size           = 5;
    
    unsigned const char* ja (unsigned char id, int offset) {
        static unsigned char data[] = {0x0f, 0x87, 0x00, 0x00, 0x00, 0x00};
        commands::to_fill.push_back ({id, offset + 2});
        return data;
    };
    
    unsigned const char* jae (unsigned char id, int offset) {
        static unsigned char data[] = {0x0f, 0x83, 0x00, 0x00, 0x00, 0x00};
        commands::to_fill.push_back ({id, offset + 2});
        return data;
    };
    
    unsigned const char* jb (unsigned char id, int offset) {
        static unsigned char data[] = {0x0f, 0x82, 0x00, 0x00, 0x00, 0x00};
        commands::to_fill.push_back ({id, offset + 2});
        return data;
    };
    
    unsigned const char* jbe (unsigned char id, int offset) {
        static unsigned char data[] = {0x0f, 0x86, 0x00, 0x00, 0x00, 0x00};
        commands::to_fill.push_back ({id, offset + 2});
        return data;
    };
    
    unsigned const char* je (unsigned char id, int offset) {
        static unsigned char data[] = {0x0f, 0x84, 0x00, 0x00, 0x00, 0x00};
        commands::to_fill.push_back ({id, offset + 2});
        return data;
    };
    
    unsigned const char* jne (unsigned char id, int offset) {
        static unsigned char data[] = {0x0f, 0x85, 0x00, 0x00, 0x00, 0x00};
        commands::to_fill.push_back ({id, offset + 2});
        return data;
    };
    
    unsigned const char* jmp (unsigned char id, int offset) {
        static unsigned char data[] = {0x0f, 0x85, 0x00, 0x00, 0x00, 0x00};
        commands::to_fill.push_back ({id, offset + 2});
        return data;
    };
    
    unsigned const char* call (unsigned char id, int offset) {
        static unsigned char data[] = {0xe8, 0x0c, 0x00, 0x00, 0x00};
        commands::to_fill.push_back ({id, offset + 2});
        return data;
    };
}

#endif /* x86_commands_hpp */
