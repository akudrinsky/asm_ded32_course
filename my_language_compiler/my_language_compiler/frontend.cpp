//
//  frontend.cpp
//  macho-compile
//
//  Created by Alexey Kudrinsky on 15.05.2020.
//  Copyright © 2020 Alexey Kudrinsky. All rights reserved.
//

#include <stdio.h>
#include "frontend.h"

for_funcs* functions = new for_funcs;
for_names* ids = new for_names;

frontend::frontend (cell *nodes) {
    nds = nodes;
}

node* frontend::all () {
    node* nd = global ();
    
    return nd;
}

node* frontend::global() {
    node* nd = nullptr;
    
    while (nds->type == VARIABLE || nds->type == DEFINITION) {
        //printf ("global:%9s\t\t%s\n\n", tokens[nds->type].name, nds->name);
        
        if (nds->type == VARIABLE) {
            nd = create_node (nullptr, BLOCK, nd, variable ());
        }
        else {
            nd = create_node (nullptr, BLOCK, nd, def_func ());
            //nds++;
        }
        
    }
    
    return nd;
}

node* frontend::def_func() {
    if (nds->type == DEFINITION) {
        //printf ("def_func:%9s\t\t%s\n\n", tokens[nds->type].name, nds->name);
        nds++;
        
        node* nd = get_id ();
        node* args = nullptr;
        
        switch (nds->type) {
            case ARGUMENTS: {
                nds++;
                args = arguments ();
            }
            case OPEN_BR: {
                nds++;
                break;
            }
            default: {
                err_info ("Strange things in def_func [1] (frontend): ");
                err_info (tokens[nds->type].name);
                err_info ("\n");
            }
        }
        //printf ("%s\n", nd->data);
        
        nd->right = instruction ();
        
        if (nds->type == CLOS_BR) {
            nds++;
        }
        else {
            err_info ("Strange things in def_func [2] (frontend): ");
            err_info (tokens[nds->type].name);
            err_info ("\n");
        }
        
        node* def_function = create_node (nullptr, DEFINITION, args, nd);
        functions->append (def_function);
        return def_function;
        
    }
    return nullptr;
}

node* frontend::arguments() {
    node* nd = nullptr;
    bool not_last = true;
    
    if (nds->type == ARGUMENTS) {
        nds++;
    }
    while ((nds->type == ID || nds->type == NUMBER || nds->type == LAST_ARG) && not_last) {
        if ((nds - 1)->type == LAST_ARG) {
            not_last = false;
        }
        
        if (nds->type == ID) {
            nd = create_node (nullptr, VARLIST, nd, get_id ());
        }
        else if (nds->type == NUMBER) {
            nd = create_node (nullptr, VARLIST, nd, get_number ());
        }
        else {
            nds++;
        }
    }
    
    return nd;
}

node* frontend::variable() {
    if (nds->type == VARIABLE) {
        //printf ("variable:%9s\t\t%s\n\n", tokens[nds->type].name, nds->name);
        nds++;
        ids->append (nds->name);                     //maybe its better to give number to variables in the beginning
        
        return create_node (nullptr, VARIABLE, get_id(), nullptr);
    }
    return nullptr;
}

node* frontend::get_id() {
    node* nd = nullptr;
    if (nds->type == ID) {
        //printf ("get_id:%9s\t\t%s\n\n", tokens[nds->type].name, nds->name);
        nd = create_node (nds->name, ID, nullptr, nullptr);
        nds++;
    }
    else if (nds->type == NUMBER) {
        return get_number ();
    }
    return nd;
}

node* frontend::instruction() {
    node* nd = nullptr;
    
    while (nds->type == VARIABLE    || nds->type == ID ||
           nds->type == INCR        || nds->type == DECR ||
           nds->type == CALL        || nds->type == RETURN ||
           nds->type == IF          || nds->type == WHILE ||
           nds->type == OUT) {
        //printf ("instruct:%9s\t\t%s\n\n", tokens[nds->type].name, nds->name);
        switch (nds->type) {
            case VARIABLE: {
                nd = create_node (nullptr, INSTRUCTION, nd, variable ());
                break;
            }
            case ID: {
                node* tmp = get_id ();
                
                switch (nds->type) {
                    case ASSIGN: {
                        nds++;
                        tmp = create_node (nullptr, ASSIGN, tmp, equation ());
                        break;
                    }
                    default: {
                        err_info ("Strange things in instruction (frontend): ");
                        err_info (tokens[nds->type].name);
                        err_info ("\n");
                        return nullptr;
                    }
                }
                
                nd = create_node (nullptr, INSTRUCTION, nd, tmp);
                break;
            }
            case INCR: {
                nds++;
                node* tmp = get_id ();
                tmp = create_node (nullptr, INCR, nullptr, tmp);
                
                nd = create_node (nullptr, INSTRUCTION, nd, tmp);
                break;
            }
            case DECR: {
                nds++;
                node* tmp = get_id ();
                tmp = create_node (nullptr, DECR, nullptr, tmp);
                
                nd = create_node (nullptr, INSTRUCTION, nd, tmp);
                break;
            }
            case CALL: {
                nds++;
                node* tmp = get_id ();
                tmp = create_node (nullptr, CALL, arguments (), tmp);
                
                nd = create_node (nullptr, INSTRUCTION, nd, tmp);
                break;
            }
            case RETURN: {                                                  //return equation!!!!!!!!!!!!!!!!!!!!
                nds++;
                node* tmp = get_id ();
                tmp = create_node (nullptr, RETURN, nullptr, tmp);
                
                nd = create_node (nullptr, INSTRUCTION, nd, tmp);
                break;
            }
            case IF: {
                node* tmp = condition ();
                
                nd = create_node (nullptr, INSTRUCTION, nd, tmp);
                break;
            }
            case WHILE: {
                node* tmp = cycle ();
                
                nd = create_node (nullptr, INSTRUCTION, nd, tmp);
                break;
            }
            case OUT: {
                nds++;
                node* tmp = get_id ();
                tmp = create_node (nullptr, OUT, nullptr, tmp);
                
                nd = create_node (nullptr, INSTRUCTION, nd, tmp);
                break;
            }
            case PRINT: {
                nds++;
                
            }
        }
        
    }
    
    //printf ("%s\n", tokens[nd->type].name);
    
    return nd;
}

node* frontend::get_number() {
    node* nd = nullptr;
    //printf ("number:%9s\t\t%s\n\n", tokens[nds->type].name, nds->name);
    if (nds->type == NUMBER) {
        nd = create_node (nds->name, NUMBER, nullptr, nullptr);
        ++nds;
    }
    return nd;
}

node* frontend::condition() {
    node* nd = nullptr;
    if (nds->type == IF) {
        //printf ("condition:%9s\t\t%s\n\n", tokens[nds->type].name, nds->name);
        nds++;
        if (nds->type == COND_DIVIDER) {
            nds++;
            node* left = get_id ();
            
            switch (nds->type) {
                case IS_EQUAL: {
                    nds++;
                    nd = create_node (nullptr, IS_EQUAL, left, get_id ());
                    break;
                }
                case IS_BIGGER: {
                    nds++;
                    nd = create_node (nullptr, IS_BIGGER, left, get_id ());
                    break;
                }
                case IS_LESS: {
                    nds++;
                    nd = create_node (nullptr, IS_LESS, left, get_id ());
                    break;
                }
                case NOT_EQUAL: {
                    nds++;
                    nd = create_node (nullptr, NOT_EQUAL, left, get_id ());
                    break;
                }
                default: {
                    err_info ("Strange things in condition [0] (frontend): ");
                    err_info (tokens[nds->type].name);
                    err_info ("\n");
                    return nullptr;
                }
            }
            
            if (nds->type == COND_DIVIDER) {
                nds++;
            }
            else {
                err_info ("Strange things in condition [1] (frontend): ");
                err_info (tokens[nds->type].name);
                err_info ("\n");
                return nullptr;
            }
            
            if (nds->type == OPEN_BR) {
                nds++;
            }
            else {
                err_info ("Strange things in condition [2] (frontend): ");
                err_info (tokens[nds->type].name);
                err_info ("\n");
                return nullptr;
            }
        }
        else {
            err_info ("Strange things in condition [3] (frontend): ");
            err_info (tokens[nds->type].name);
            err_info ("\n");
            return nullptr;
        }
    }
    
    nd = create_node (nullptr, IF, nd, instruction ());
    
    if (nds->type == CLOS_BR) {
        nds++;
    }
    else {
        err_info ("Strange things in condition [3] (frontend): ");
        err_info (tokens[nds->type].name);
        err_info ("\n");
    }
    
    return nd;
}

node* frontend::cycle() {
    //printf ("cycle:%9s\t\t%s\n\n", tokens[nds->type].name, nds->name);
    
    node* nd = nullptr;
    if (nds->type == WHILE) {
        nds++;
        if (nds->type == COND_DIVIDER) {
            nds++;
            node* left = get_id ();
            
            switch (nds->type) {
                case IS_EQUAL: {
                    nds++;
                    nd = create_node (nullptr, IS_EQUAL, left, get_id ());
                    break;
                }
                case IS_BIGGER: {
                    nds++;
                    nd = create_node (nullptr, IS_BIGGER, left, get_id ());
                    break;
                }
                case IS_LESS: {
                    nds++;
                    nd = create_node (nullptr, IS_LESS, left, get_id ());
                    break;
                }
                case NOT_EQUAL: {
                    nds++;
                    nd = create_node (nullptr, NOT_EQUAL, left, get_id ());
                    break;
                }
                default: {
                    err_info ("Strange things in condition [0] (frontend): ");
                    err_info (tokens[nds->type].name);
                    err_info ("\n");
                    return nullptr;
                }
            }
            
            if (nds->type == COND_DIVIDER) {
                nds++;
            }
            else {
                err_info ("Strange things in condition [1] (frontend): ");
                err_info (tokens[nds->type].name);
                err_info ("\n");
                return nullptr;
            }
            
            if (nds->type == OPEN_BR) {
                nds++;
            }
            else {
                err_info ("Strange things in condition [2] (frontend): ");
                err_info (tokens[nds->type].name);
                err_info ("\n");
                return nullptr;
            }
        }
        else {
            err_info ("Strange things in condition [3] (frontend): ");
            err_info (tokens[nds->type].name);
            err_info ("\n");
            return nullptr;
        }
    }
    
    nd = create_node (nullptr, WHILE, nd, instruction ());
    
    if (nds->type == CLOS_BR) {
        nds++;
    }
    else {
        err_info ("Strange things in condition [3] (frontend): ");
        err_info (tokens[nds->type].name);
        err_info ("\n");
    }
    
    return nd;
}

node* frontend::equation() {
    //printf ("equation:%9s\t\t%s\n\n", tokens[nds->type].name, nds->name);
    node* nd = multiplication ();
    
    while (nds->type == PLUS || nds->type == MINUS) {
        if (nds->type == PLUS) {
            nds++;
            nd = create_node (nullptr, PLUS, nd, multiplication ());
        }
        else {
            nds++;
            nd = create_node (nullptr, MINUS, nd, multiplication ());
        }
    }
    
    return nd;
}

node* frontend::multiplication() {
    //printf ("multiplication:%9s\t\t%s\n\n", tokens[nds->type].name, nds->name);
    node* nd = brackets ();
    
    while (nds->type == MULT || nds->type == DIVIDE) {
        if (nds->type == MULT) {
            nds++;
            nd = create_node (nullptr, MULT, nd, brackets ());
        }
        else {
            nds++;
            nd = create_node (nullptr, DIVIDE, nd, brackets ());
        }
    }
    
    return nd;
}

node* frontend::brackets() {
    if (nds->type == OPEN_EQ) {
        ++nds;
        node* nd = equation ();
        
        if (nds->type != CLOS_EQ) {
            err_info ("frontend brackets - no closing bracket in equation\n");
        }
        ++nds;
        return nd;
    }
    else {
        if (nds->type == ID || nds->type == NUMBER) {
            return get_id ();
        }
        else if (nds->type == CALL) {
            nds++;
            node* tmp = get_id ();
            return create_node (nullptr, CALL, arguments (), tmp);
        }
    }
    
    return nullptr;
}
