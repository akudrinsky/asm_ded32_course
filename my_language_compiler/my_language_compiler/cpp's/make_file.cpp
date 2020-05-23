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
#include <stdio.h>
#include "make_file.hpp"

template <typename T>
const T& max (const T& a, const T& b) {
    return a < b ? b : a;
}

/* Try MachOView, if you do not understand something.
 Or better code it yourself. */

void make_file (const char* filename, const unsigned char* code, uint64_t len) {
    constexpr uint32_t code_beginning = sizeof (mach_header_64) + 3 * sizeof (segment_command_64) + 2 * sizeof (section_64) + sizeof (unixthread);
    
    constexpr int minimum_size = 0x1000;
    
    if (len < minimum_size)
        len = minimum_size;
    
    mach_header_64 header {
        .magic      = MH_MAGIC_64,
        .cputype    = CPU_TYPE_X86_64,
        .cpusubtype = CPU_SUBTYPE_X86_64_ALL,
        .filetype   = MH_EXECUTE,
        .ncmds      = 4,
        .sizeofcmds = code_beginning - sizeof (mach_header_64),
        .flags      = 0x85,
        .reserved   = 0x0
    };
    
    constexpr int page_zero_size = 0x1000;
    segment_command_64 page_zero {
        .cmd        = LC_SEGMENT_64,
        .cmdsize    = sizeof (segment_command_64),
        .segname    = "__PAGEZERO\0\0\0\0\0",
        .vmaddr     = 0x0,
        .vmsize     = page_zero_size,
        .fileoff    = 0x0,
        .filesize   = 0x0,
        .maxprot    = 0x0,
        .initprot   = 0x0,
        .nsects     = 0x0,
        .flags      = 0x0
    };
    
    segment_command_64 text_segment {
        .cmd        = LC_SEGMENT_64,
        .cmdsize    = sizeof (segment_command_64) + sizeof (section_64),
        .segname    = "__TEXT\0\0\0\0\0\0\0\0\0",
        .vmaddr     = page_zero_size,
        .vmsize     = len,
        .fileoff    = 0x00,
        .filesize   = len,
        .maxprot    = 0x7,
        .initprot   = 0x5,
        .nsects     = 0x1,
        .flags      = 0x0
    };
    
    section_64 text_section {
        .sectname   = "__text\0\0\0\0\0\0\0\0\0",
        .segname    = "__TEXT\0\0\0\0\0\0\0\0\0",
        .addr       = page_zero_size + code_beginning,
        .size       = len - code_beginning,
        .offset     = code_beginning,
        .align      = 0x1,
        .reloff     = 0x0,
        .nreloc     = 0x0,
        .flags      = 0x80000400,
        .reserved1  = 0x0,
        .reserved2  = 0x0,
        .reserved3  = 0x0
    };
    
    segment_command_64 data_segment {
        .cmd        = LC_SEGMENT_64,
        .cmdsize    = sizeof (segment_command_64) + sizeof (section_64),
        .segname    = "__DATA\0\0\0\0\0\0\0\0\0",
        .vmaddr     = page_zero_size + len,
        .vmsize     = minimum_size,
        .fileoff    = len,
        .filesize   = minimum_size,
        .maxprot    = 0x3,
        .initprot   = 0x3,
        .nsects     = 0x1,
        .flags      = 0x0
    };
    
    section_64 data_section {
        .sectname   = "__data\0\0\0\0\0\0\0\0\0",
        .segname    = "__DATA\0\0\0\0\0\0\0\0\0",
        .addr       = page_zero_size + len,
        .size       = minimum_size,
        .offset     = static_cast<uint32_t> (len),
        .align      = 0x00000001,
        .reloff     = 0x0,
        .nreloc     = 0x0,
        .flags      = 0x0,
        .reserved1  = 0x0,
        .reserved2  = 0x0,
        .reserved3  = 0x0
    };
    
    unixthread u_thread {
        .cmd       = LC_UNIXTHREAD,
        .cmdsize   = sizeof (unixthread),
        .flavor    = x86_THREAD_STATE64,
        .count     = 0x2A,
        .rax       = 0,
        .rbx       = 0,
        .rcx       = 0,
        .rdx       = 0,
        .rdi       = 0,
        .rsi       = page_zero_size + len,
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
        .rip       = page_zero_size + code_beginning,
        .rflags    = 0,
        .cs        = 0,
        .fs        = 0,
        .gs        = 0
    };
    
    FILE* executable = fopen (filename, "wb");
    
    char initial_data = '\0';

    fwrite (&header,        sizeof (char),  sizeof (mach_header_64),     executable);
    fwrite (&page_zero,     sizeof (char),  sizeof (segment_command_64), executable);
    fwrite (&text_segment,  sizeof (char),  sizeof (segment_command_64), executable);
    fwrite (&text_section,  sizeof (char),  sizeof (section_64),         executable);
    fwrite (&data_segment,  sizeof (char),  sizeof (segment_command_64), executable);
    fwrite (&data_section,  sizeof (char),  sizeof (section_64),         executable);
    fwrite (&u_thread,      sizeof (char),  sizeof (unixthread),         executable);
    fwrite (code,           sizeof (char),  len,                         executable);
    //fwrite (&initial_data,  sizeof (char),  data_size,                   executable);
    for (int i = 0; i < minimum_size; ++i)
        fputc (initial_data, executable);
    
    fclose (executable);
    
    // add rights to execute the file
    const size_t filename_len = strlen (filename);
    const size_t cmd_len = 9;
    char* make_executable = new char[cmd_len + filename_len + 1];
    
    strncpy (make_executable, "chmod +x ", cmd_len);
    strncpy (make_executable + cmd_len, filename, filename_len);
    make_executable[cmd_len + filename_len] = '\0';
    
    ON_DEBUG (printf ("%s\n", make_executable))
    system (make_executable);
    delete [] make_executable;
}

#endif
