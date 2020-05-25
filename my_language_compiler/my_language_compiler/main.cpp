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
    if (argc == 1) {
        printf ("Mylang error: should enter at least name of file to compile\n");
        return 0;
    }
    else if (argc == 2) {
        node* nd = rec_descent (argv[1]);
        nd->photo ();
        
        backend (nd);
    }
    else if (argc == 4) {
        if (strncmp (argv[2], "-o", 2) == 0) {
            node* nd = rec_descent (argv[1]);
            nd->photo ();
            
            backend (nd, argv[3]);
        }
        else if (strncmp (argv[2], "-p", 2) == 0) {
            node* nd = rec_descent (argv[1]);
            nd->photo ();
            
            backend (nd, "my.out", atoi (argv[3]));
        }
        else {
            printf ("Mylang error: unknown compile option: %s\n", argv[2]);
            return 0;
        }
    }
    else if (argc == 6) {
        node* nd = rec_descent (argv[1]);
        nd->photo ();
        
        backend (nd, argv[3], atoi (argv[5]));
    }
    else {
        printf ("Mylang error: problems with compile options. Now the syntax must be like:\n\t./my_language_compiler <name of file to compile> -o <output file name> -p <precision (power of 2)>\n");
    }

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

