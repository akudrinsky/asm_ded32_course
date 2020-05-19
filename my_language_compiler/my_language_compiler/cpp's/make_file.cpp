//
//  make_file.cpp
//  my_language_compiler
//
//  Created by Alexey Kudrinsky on 17.05.2020.
//  Copyright © 2020 Alexey Kudrinsky. All rights reserved.
//

#ifndef MAKE_BINARY_FILE
#define MAKE_BINARY_FILE

#include <mach-o/loader.h>
//#include <mach/machine/unixthread.h>
#include <stdio.h>
#include "make_file.hpp"

void make_file (const char* filename, const char* code, int len) {
    constexpr int code_beginning = sizeof (mach_header_64) + 2 * sizeof (segment_command_64) + sizeof (section_64) + sizeof (unixthread);
    
    mach_header_64 header {
        .magic      = 0xFEEDFACF,
        .cputype    = 0x01000007,
        .cpusubtype = static_cast<cpu_subtype_t> (0x80000003),
        .filetype   = 0x00000002,
        .ncmds      = 3,
        .sizeofcmds = 2 * sizeof (segment_command_64) + sizeof (section_64) + sizeof (unixthread),
        .flags      = 0x00000001,
        .reserved   = 0x00000000
    };
    
    segment_command_64 page_zero {
        .cmd        = 0x00000019,
        .cmdsize    = sizeof (segment_command_64),
        .segname    = "__PAGEZERO\0\0\0\0\0",
        .vmaddr     = 0x0000000000000000,
        .vmsize     = 0x0000000001000000,
        .fileoff    = 0x0000000000000000,
        .filesize   = 0x0000000000000000,
        .maxprot    = 0x00000000,
        .initprot   = 0x00000000,
        .nsects     = 0x00000000,
        .flags      = 0x00000000
    };
    
    segment_command_64 text_segment {
        .cmd        = 0x00000019,
        .cmdsize    = sizeof (segment_command_64),
        .segname    = "__TEXT\0\0\0\0\0\0\0\0\0",
        .vmaddr     = 0x0000000001000000,
        .vmsize     = 0x0000000000010000,
        .fileoff    = 0x0000000000000000,
        .filesize   = 0x0000000000010000,
        .maxprot    = 0x00000007,
        .initprot   = 0x00000005,
        .nsects     = 0x00000001,
        .flags      = 0x00000000
    };
    
    section_64 text_section {
        .sectname   = "__text\0\0\0\0\0\0\0\0\0",
        .segname    = "__TEXT\0\0\0\0\0\0\0\0\0",
        .addr       = static_cast<uint64_t> (0x001000000 | code_beginning),
        .size       = static_cast<uint64_t> (len),
        .offset     = static_cast<uint32_t> (code_beginning),
        .align      = 0x00000001,
        .reloff     = 0x00000000,
        .nreloc     = 0x00000000,
        .flags      = 0x80000400,
        .reserved1  = 0x00000000,
        .reserved2  = 0x00000000,
        .reserved3  = 0x00000000
    };
    
    unixthread u_thread {
        .cmd       = 0x00000005,
        .cmdsize   = sizeof (unixthread),
        .flavor    = 0x00000004,
        .count     = 0x0000002A,
        .rax       = 0,
        .rbx       = 0,
        .rcx       = 0,
        .rdx       = 0,
        .rdi       = 0,
        .rsi       = 0,
        .rbp       = 0,
        .rsp       = 0,
        .r8        = 0,
        .r9        = 0,
        .r10       = 0,
        .r11       = 0,
        .r12       = 0,
        .r13       = 0,
        .r14       = 0,
        .r15       = 0,
        .rip       = 0x001000000 | code_beginning,
        .rflags    = 0,
        .cs        = 0,
        .fs        = 0,
        .gs        = 0
    };
    
    FILE* executable = fopen (filename, "wb");
    
    fwrite (&header,        sizeof (char),  sizeof (mach_header_64),     executable);
    fwrite (&page_zero,     sizeof (char),  sizeof (segment_command_64), executable);
    fwrite (&text_segment,  sizeof (char),  sizeof (segment_command_64), executable);
    fwrite (&text_section,  sizeof (char),  sizeof (section_64),         executable);
    fwrite (&u_thread,      sizeof (char),  sizeof (unixthread),         executable);
    fwrite (code,           sizeof (char),  len,                         executable);
    
    fclose (executable);
    
    // add rights to execute the file
    const size_t filename_len = strlen (filename);
    const size_t cmd_len = 9;
    char* make_executable = new char[cmd_len + filename_len + 1];
    
    strncpy (make_executable, "chmod +x ", cmd_len);
    strncpy (make_executable + cmd_len, filename, filename_len);
    make_executable[cmd_len + filename_len] = '\0';
    
    //printf ("%s\n", make_executable);
    system (make_executable);
    delete [] make_executable;
}

#endif
