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
        int offset              = -1;
        int next_instruction    = -1;
    };
    
    struct compare {
        bool operator() (const char* a, const char* b) const {
            int iter = 0;
            while (a[iter] == b[iter] and a[iter] != '\0' and a[iter] != '\0') {
                ++iter;
            }
            if ((a[iter] == '\0' and b[iter] != '\0')
                or (a[iter] != '\0' and b[iter] != '\0' and a[iter] < b[iter])) {
                return true;
            }
            
            return false;
        }
    };
    
    std::map<unsigned char, int>    labels;           // here will be data about all found labels
    std::multimap<unsigned char, label>  to_fill;          // put when need to fill it later
    std::map<char*, unsigned char, compare>  funs_ids;
    
    static constexpr unsigned char  nop[]               = {0x90};
    static constexpr int            nop_size            = 1;
    
    static constexpr unsigned char  add_rsp_40[]        = {0x48, 0x83, 0xc4, 0x40};
    static constexpr int            add_rsp_40_size     = 4;
    
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
    
    static constexpr unsigned char  sub_rax_rbx[]       = {0x48, 0x29, 0xd8};
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
    
    static constexpr unsigned char  end_programm[]      = {
        0xb8, 0x01, 0x00, 0x00, 0x02,   // mov rax, 0x2000001
        0xbf, 0x00, 0x00, 0x00, 0x00,   // mov rdi, 0
        0x0f, 0x05};                    // syscall
    static constexpr int            end_programm_size   = 12;
    
    static constexpr unsigned char  out[] = {
        0x58,                           // pop    rax
        0x56,                           // push    rsi
        0x48, 0x83, 0xc6, 0x09,         // add    rsi, 9
        0xc6, 0x06, 0x0a,               // mov    [rsi], '\n'
        0xbb, 0x0a, 0x00, 0x00, 0x00,   // mov    ebx, 10
        //.next_digit
        0x31, 0xd2,                     // xor    edx, edx
        0xf7, 0xf3,                     // div    ebx
        0x80, 0xc2, 0x30,               // add    dl, 48
        0x48, 0xff, 0xce,               // dec    rsi
        0x88, 0x16,                     // mov    byte ptr [rsi], dl
        0x85, 0xc0,                     // test    eax, eax
        0x75, 0xf0,                     // jne    -16 <.next_digit>
        0xb8, 0x04, 0x00, 0x00, 0x02,   // mov    eax, 0x2000004
        0xbf, 0x01, 0x00, 0x00, 0x00,   // mov    edi, 1
        0xba, 0x08, 0x00, 0x00, 0x00,   // mov    edx, 8
        0x0f, 0x05,                     // syscall
        0x5e};                          // pop    rsi
    static constexpr int            out_size    = 48;
    
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
        commands::to_fill.insert ({id, {offset + 2, offset + ja_size}});
        return data;
    };
    
    unsigned const char* jae (unsigned char id, int offset) {
        static unsigned char data[] = {0x0f, 0x83, 0x00, 0x00, 0x00, 0x00};
        commands::to_fill.insert ({id, {offset + 2, offset + jae_size}});
        return data;
    };
    
    unsigned const char* jb (unsigned char id, int offset) {
        static unsigned char data[] = {0x0f, 0x82, 0x00, 0x00, 0x00, 0x00};
        commands::to_fill.insert ({id, {offset + 2, offset + jb_size}});
        return data;
    };
    
    unsigned const char* jbe (unsigned char id, int offset) {
        static unsigned char data[] = {0x0f, 0x86, 0x00, 0x00, 0x00, 0x00};
        commands::to_fill.insert ({id, {offset + 2, offset + jbe_size}});
        return data;
    };
    
    unsigned const char* je (unsigned char id, int offset) {
        static unsigned char data[] = {0x0f, 0x84, 0x00, 0x00, 0x00, 0x00};
        commands::to_fill.insert ({id, {offset + 2, offset + je_size}});
        return data;
    };
    
    unsigned const char* jne (unsigned char id, int offset) {
        static unsigned char data[] = {0x0f, 0x85, 0x00, 0x00, 0x00, 0x00};
        commands::to_fill.insert ({id, {offset + 2, offset + jne_size}});
        return data;
    };
    
    unsigned const char* jmp (unsigned char id, int offset) {
        static unsigned char data[] = {0xe9, 0x00, 0x00, 0x00, 0x00};
        commands::to_fill.insert ({id, {offset + 1, offset + jmp_size}});
        return data;
    };
    
    unsigned const char* call (unsigned char id, int offset) {
        static unsigned char data[] = {0xe8, 0x00, 0x00, 0x00, 0x00};
        commands::to_fill.insert ({id, {offset + 1, offset + call_size}});
        return data;
    };
    
    unsigned const char* add_rsp (unsigned char number) {
        static unsigned char data[] = {0x48, 0x83, 0xc4, 0x00};
        data[3] = number;
        return data;
    };
    
    unsigned const char* mov_qword_rbp_rax (int32_t number) {
        static unsigned char data[] = {0x48, 0x89, 0x45, 0x00};
        ON_DEBUG (printf ("nmb %d, %d\n", number, 0x100 - 8 * ((unsigned char) number)))
        data[3] = (unsigned char) 0x100 - 8 * ((unsigned char) number);
        return data;
    };
    static constexpr int mov_qword_rbp_rax_size = 4;
    
    unsigned const char* mov_rax_qword_rbp (int32_t number) {
        static unsigned char data[] = {0x48, 0x8b, 0x45, 0x00};
        ON_DEBUG (printf ("nmb %d, %d\n", number, 0x100 - 8 * ((unsigned char) number)))
        data[3] = (unsigned char) 0x100 - 8 * ((unsigned char) number);
        return data;
    };
    static constexpr int mov_rax_qword_rbp_size = 4;
    
    unsigned const char* mov_rax (int32_t number) {
        static unsigned char data[] = {0xb8, 0x00, 0x00, 0x00, 0x00};
        data[1] = number;
        return data;
    };
    static constexpr int mov_rax_size = 5;
    
    unsigned const char* push_num (int32_t number) {
        static unsigned char data[] = {0x68, 0x00, 0x00, 0x00, 0x00};
        data[1] = number;
        return data;
    };
    static constexpr int push_num_size = 5;
    
    unsigned const char* sar_rax (unsigned char number) {
        static unsigned char data[] = {0x48, 0xc1, 0xf8, 0x00};
        data[3] = number;
        return data;
    };
    static constexpr int sar_rax_size = 4;
    
    unsigned const char* sar_rbx (unsigned char number) {
        static unsigned char data[] = {0x48, 0xc1, 0xfb, 0x00};
        data[3] = number;
        return data;
    };
    static constexpr int sar_rbx_size = 4;
    
    unsigned const char* imul_rax (uint32_t number) {
        static unsigned char data[] = {0x48, 0x69, 0xc0, 0x00, 0x00, 0x00, 0x00};
        data[3] = number;
        return data;
    };
    static constexpr int imul_rax_size = 7;
    
    unsigned const char* imul_rbx (uint32_t number) {
        static unsigned char data[] = {0x48, 0x69, 0xd0, 0x00, 0x00, 0x00, 0x00};
        data[3] = number;
        return data;
    };
    static constexpr int imul_rbx_size = 7;
    
    unsigned const char* init_frame (unsigned char number_of_func_params) {
        static unsigned char data[] = {
            0x55,                               // push rbp
            0x48, 0x89, 0xe5,                   // mov rbp, rsp
            0x48, 0x83, 0xc5, 0x00,             // add rbp, <fill later>
            0x48, 0x83, 0xec, 0x40};            // sub rsp, 0x40 (so five local vars max)
        data[7] = 8 * (number_of_func_params + 1);
        return data;
    };
    
    static constexpr int init_frame_size = 12;
}

#endif /* x86_commands_hpp */
