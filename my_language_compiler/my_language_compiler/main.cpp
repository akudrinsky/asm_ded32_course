//
//  main.cpp
//  macho-compile
//
//  Created by Alexey Kudrinsky on 15.05.2020.
//  Copyright © 2020 Alexey Kudrinsky. All rights reserved.
//

#include "headers/backend.h"
#include "headers/tree.hpp"
#include "headers/frontend-1.h"
#include "headers/make_file.hpp"
//#include "x86_commands.hpp"
#include "string.h"

#include <iostream>

int main (int argc, const char* argv[]) {
    /*
    if (argc == 1) {
        printf ("Mylang error: should enter at least name of file to compile\n");
    }
     */
    
    node* nd = rec_descent ();
    nd->photo ();
    
    backend (nd);
    
    /*
    char* code = new char[10];
    strncpy (code, (char*)commands::push_rax, commands::push_rax_size);
    strncpy (code + commands::push_rax_size, (char*)commands::pop_rax, commands::pop_rax_size);
    
    make_file ("try1", (char*)commands::push_rax, commands::push_rax_size);
    
    delete [] code;
     */
    
    return 0;
}

/*
 $ You could use python,
 but you chose your destiny... $
 
 
 dictate betrayal of iter :
 person sum
 sum now iter
 
 iter now iter - 1
 
 if " iter worse dead " :
 sum now sum * cause betrayal of iter .
 
 summon sum
 .
 
 dictate reign :
 person minister
 person collector
 
 minister now dead
 collector now a disgrace
 
 minister now cause betrayal of collector
 
 know minister
 
 summon dead
 .
 */

