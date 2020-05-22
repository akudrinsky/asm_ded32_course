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

#include <map>

#define cmd(text) write (commands::text, commands::text##_size);

namespace commands {
    struct label {
        unsigned char id    = -1;
        int offset          = -1;
    };
    
    std::map<unsigned char, int> labels;                      // here will be data about all found labels
    std::map<unsigned char, int> to_fill;                     // put when need to fill it later
    std::map<char*, unsigned char> funs_ids;
    
    static constexpr unsigned char  nop[]               = {0x90};
    static constexpr int            nop_size            = 1;
    
    static constexpr unsigned char  ret[]               = {0x48, 0x83, 0xc4, 0x40, 0xC3};
    static constexpr int            ret_size            = 5;
    
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
    
    static constexpr unsigned char  pop_rbp[]           = {0x5d};
    static constexpr int            pop_rbp_size        = 1;
    
    static constexpr unsigned char  cmp_rax_rbx[]       = {0x48, 0x39, 0xd8};
    static constexpr int            cmp_rax_rbx_size    = 3;
    
    static constexpr unsigned char  end_programm[]      = {0xb8, 0x01, 0x00, 0x00, 0x02, 0xbf, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x05}; /* mov rax, 0x2000001 (5) mov rdi, 0 (5) syscall (2) */
    static constexpr int            end_programm_size   = 12;
    
    static constexpr unsigned char  out[] = {0x58, 0x56, 0x48, 0x83, 0xc6, 0x09, 0xbb, 0x0a, 0x00, 0x00, 0x00, 0x31, 0xd2, 0xf7, 0xf3, 0x80, 0xc2, 0x30, 0x48, 0xff, 0xce, 0x88, 0x16, 0x85, 0xc0, 0x75, 0xf0, 0xb8, 0x04, 0x00, 0x00, 0x02, 0xbf, 0x01, 0x00, 0x00, 0x00, 0xba, 0x08, 0x00, 0x00, 0x00, 0x0f, 0x05, 0x5e};
    /*
     58                 pop    rax
     56                 push    rsi
     48 83 c6 09        add    rsi, 9
     bb 0a 00 00 00     mov    ebx, 10
     .next_digit:
     31 d2              xor    edx, edx
     f7 f3              div    ebx
     80 c2 30           add    dl, 48
     48 ff ce           dec    rsi
     88 16              mov    byte ptr [rsi], dl
     85 c0              test    eax, eax
     75 f0              jne    -16 <.next_digit>
     b8 04 00 00 02     mov    eax, 33554436
     bf 01 00 00 00     mov    edi, 1
     ba 08 00 00 00     mov    edx, 8
     0f 05              syscall
     5e                 pop    rsi
     */
    static constexpr int            out_size    = 45;
    
    static constexpr int            ja_size             = 6;
    static constexpr int            jae_size            = 6;
    static constexpr int            jb_size             = 6;
    static constexpr int            jbe_size            = 6;
    static constexpr int            je_size             = 6;
    static constexpr int            jne_size            = 6;
    static constexpr int            jmp_size            = 5;
    static constexpr int            call_size           = 5;
    static constexpr int            add_rsp_size        = 5;
    
    unsigned const char* ja (unsigned char id, int offset) {
        static unsigned char data[] = {0x0f, 0x87, 0x00, 0x00, 0x00, 0x00};
        commands::to_fill.insert ({id, offset + 2});
        return data;
    };
    
    unsigned const char* jae (unsigned char id, int offset) {
        static unsigned char data[] = {0x0f, 0x83, 0x00, 0x00, 0x00, 0x00};
        commands::to_fill.insert ({id, offset + 2});
        return data;
    };
    
    unsigned const char* jb (unsigned char id, int offset) {
        static unsigned char data[] = {0x0f, 0x82, 0x00, 0x00, 0x00, 0x00};
        commands::to_fill.insert ({id, offset + 2});
        return data;
    };
    
    unsigned const char* jbe (unsigned char id, int offset) {
        static unsigned char data[] = {0x0f, 0x86, 0x00, 0x00, 0x00, 0x00};
        commands::to_fill.insert ({id, offset + 2});
        return data;
    };
    
    unsigned const char* je (unsigned char id, int offset) {
        static unsigned char data[] = {0x0f, 0x84, 0x00, 0x00, 0x00, 0x00};
        commands::to_fill.insert ({id, offset + 2});
        return data;
    };
    
    unsigned const char* jne (unsigned char id, int offset) {
        static unsigned char data[] = {0x0f, 0x85, 0x00, 0x00, 0x00, 0x00};
        commands::to_fill.insert ({id, offset + 2});
        return data;
    };
    
    unsigned const char* jmp (unsigned char id, int offset) {
        static unsigned char data[] = {0x0f, 0x85, 0x00, 0x00, 0x00, 0x00};
        commands::to_fill.insert ({id, offset + 2});
        return data;
    };
    
    unsigned const char* call (unsigned char id, int offset) {
        static unsigned char data[] = {0xe8, 0x00, 0x00, 0x00, 0x00};
        commands::to_fill.insert ({id, offset + 1});
        return data;
    };
    
    unsigned const char* add_rsp (unsigned char number) {
        static unsigned char data[] = {0x48, 0x83, 0xc4, 0x00};
        data[3] = number;
        return data;
    };
    
    unsigned const char* mov_qword_rbp_rax (int32_t number) {
        static unsigned char data[] = {0x48, 0x89, 0x85, 0x00, 0x00, 0x00, 0x00};
        data[3] = number;
        return data;
    };
    static constexpr int mov_qword_rbp_rax_size = 7;
    
    unsigned const char* mov_rax_qword_rbp (int32_t number) {
        static unsigned char data[] = {0x48, 0x8b, 0x85, 0x00, 0x00, 0x00, 0x00};
        data[3] = number;
        return data;
    };
    static constexpr int mov_rax_qword_rbp_size = 7;
    
    unsigned const char* push_num (int32_t number) {
        static unsigned char data[] = {0x68, 0x00, 0x00, 0x00, 0x00};
        data[1] = number;
        return data;
    };
    static constexpr int push_num_size = 5;
    
    unsigned const char* init_frame (unsigned char number) {
        static unsigned char data[] = {0x55, 0x48, 0x89, 0xe5, 0x48, 0x83, 0xc5, 0x00, 0x48, 0x83, 0xec, 0x40};
        data[7] = 8 * (number + 2);
        return data;
    };
    
    static constexpr int init_frame_size = 12;
}

#endif /* x86_commands_hpp */
