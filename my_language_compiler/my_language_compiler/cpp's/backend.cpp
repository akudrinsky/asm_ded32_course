//
//  backend.cpp
//  macho-compile
//
//  Created by Alexey Kudrinsky on 15.05.2020.
//  Copyright © 2020 Alexey Kudrinsky. All rights reserved.
//

#include <stdio.h>
#include "../headers/backend.h"
#include "../headers/x86_commands.hpp"

b_end::b_end (node *root, int precision) : precision (precision) {
    code = new unsigned char[max_len];
    main_nd = root;
    cond_number = 0;
    local_vars = nullptr;
    
    write (commands::call (0, 0), commands::call_size);
    ++cond_number;

    //show_cmd (end_programm)
    cmd (end_programm)
}

b_end::~b_end () {
    delete [] code;
}

void b_end::write (const unsigned char cmd[], const int cmd_len) {
    if (len + cmd_len > max_len) {                      // need to make it more safe
        err_info ("not enough code length\n");
    }
    memcpy ((char*) code + len, (char*) cmd, cmd_len);
    ON_DEBUG (printf ("offset: %x\n", 0x250 + len))
    len += cmd_len;
}

uint32_t b_end::reg_name (node *nd) {
    if (nd->data == nullptr) {
        err_info ("problems in reg_name\n");
        return -1;
    }
    int index = local_vars->search_name (nd->data);
    
    if (index == -1) {
        err_info ("bad name in reg_name: ");
        err_info (nd->data);
        err_info ("\n");
        return -1;
    }
    else {
        return index;
        //fprintf (dist, "%d", index);
    }
}

bool b_end::all () {
    if (main_nd == nullptr) {
        err_info ("problems in backend: all (1)\n");
        return false;
    }
    
    ON_DEBUG (printf ("\n\nSTART\n\n"))
    
    if (main_nd->type == PROGRAMM) {
        return block (main_nd->right);
    }
    else if (main_nd->type == BLOCK) {
        return block (main_nd);
    }
    else {
        err_info ("problems in backend: all (2)\n");
        return false;
    }
}

bool b_end::block (node *nd) {
    ON_DEBUG (printf ("block:%9s\t\t%s\n\n", tokens[nd->type].name, nd->data))
    bool have_problems = false;
    
    if (nd == nullptr) {
        err_info ("problems in backend: block (1)\n");
        return false;
    }
    
    if (nd->type != BLOCK) {
        err_info ("problems in backend: block (2)\n");
        return false;
    }
    
    if (nd->left != nullptr) {
        have_problems = have_problems || block (nd->left);
    }
    
    if (nd->right != nullptr) {
        if (nd->right->type == DEFINITION) {
            have_problems = have_problems || func_def (nd->right);
        }
        else if (nd->right->type == VARIABLE) {
            have_problems = have_problems || variable (nd->right);
        }
    }
    else {
        err_info ("problems in backend: block (3)\n");
        return false;
    }
    
    return !have_problems;
}

bool b_end::func_def (node *nd) {
    bool have_problems = false;
    
    if (nd == nullptr) {
        err_info ("problems in backend: func_def (1)\n");
        return false;
    }
    ON_DEBUG (printf ("func_def:%9s\t\t%s\n\n", tokens[nd->type].name, nd->data))
    
    if (nd->type != DEFINITION) {                                       //current node - func definition
        err_info ("problems in backend: func_def (2)\n");
        return false;
    }
    
    for_names* local_names = new for_names;
    if (nd->is_left ()) {
        node* tmp = nd->left;
        while (tmp->left != nullptr) {
            local_names->append (tmp->right->data);
            tmp = tmp->left;
        }
        local_names->append (tmp->right->data);
        local_names->initial_ammount++;
    }
    local_vars = local_names;
    ON_DEBUG (local_vars->print())
    
    if (nd->right == nullptr) {
        err_info ("problems in backend: func_def (3)\n");
        return false;
    }
    else {
        node* id_nd = nd->right;
        
        //write ("\nfunc ");
        //write (id_nd->data);
        //write ("\npushr ax\npush 5\nadd\npopr ax\n\n");

        if (strncmp (nd->right->data, "reign", 6) == 0) {
            commands::funs_ids.insert (std::make_pair (nd->right->data, 0));
            commands::labels.insert ({0, len});
        }
        else {
            commands::funs_ids.insert (std::make_pair (nd->right->data, cond_number));
            commands::labels.insert ({cond_number, len});
            ++cond_number;
        }
        
        //cmd (init_frame)
        write (commands::init_frame (local_vars->initial_ammount), commands::init_frame_size);
        
        //parameters
        /*
        for (int i = 1; i <= local_vars->amount; ++i) {
            //fprintf (dist, "popm [ax+%d]\n", local_names->initial_ammount - i);
            popm (local_names->initial_ammount - i);
        }
         */
        //write ("\npushr fx\npush 10\nadd\npopr fx\n");
        
        if (id_nd->right == nullptr) {
            err_info ("problems in backend: func_def (4)\n");
        }
        else {
            have_problems = have_problems || instructions (id_nd->right);
        }
    }
    
    return !have_problems;
}

bool b_end::func_return (node *nd) {
    ON_DEBUG (printf ("func_ret:%9s\t\t%s\n\n", tokens[nd->type].name, nd->data))
    if (nd == nullptr) {
        err_info ("problems in backend: func_return (1)\n");
        return false;
    }
    
    if (nd->type != RETURN) {                                       //current node - return
        err_info ("problems in backend: func_return (2)\n");
        return false;
    }
    ON_DEBUG (local_vars->print())
    
    if (nd->is_right() && nd->right->type == ID) {
        //fprintf (dist, "pushm [ax+%d]\n", local_vars->search_name (nd->right->data));
        //pushm (local_vars->search_name (nd->right->data));
        write (commands::mov_rax_qword_rbp (reg_name (nd->right)), commands::mov_rax_qword_rbp_size);
    }
    else if (nd->is_right() && nd->right->type == NUMBER) {
        write (commands::mov_rax (atoi (nd->right->data)), commands::mov_rax_size);
    }
    
    //write ("\npushr ax\npush 5\nsub\npopr ax\n\nret\n");
    cmd (add_rsp_40)
    cmd (pop_rbp)
    cmd (ret)
    
    return true;
}

bool b_end::func_call (node *nd) {
    if (nd == nullptr) {
        err_info ("problems in backend: func_call (1)\n");
        return false;
    }
    ON_DEBUG (printf ("func_call:%9s\t\t%s\n\n", tokens[nd->type].name, nd->data));
    
    if (nd->type != CALL) {
        err_info ("problems in backend: func_call (2)\n");
        return false;
    }
    
    if (nd->is_right()) {
        int num = 0;
        node* tmp = nd;
        while (tmp->left != nullptr) {
            tmp = tmp->left;
            push (tmp->right);
            ++num;
        }
        
        //write ("call ", nd->right->data, "\n");
        // (new) write (commands::call (commands::funs_ids[nd->right->data], len), commands::call_size);
        try {
            write (commands::call (commands::funs_ids[nd->right->data], len), commands::call_size);
        } catch (...) {
            printf ("Undefined symbol or reference: %s", nd->right->data);
            throw std::runtime_error ("no such a function");
        }
        
        write (commands::add_rsp (num * register_size), commands::add_rsp_size);
        cmd (push_rax)
    }
    else {
        err_info ("problems in backend: func_call (3)\n");
        return false;
    }
    
    return true;
}

bool b_end::variable (node *nd) {
    bool have_problems = false;
    ON_DEBUG (printf ("variable:%9s\t\t%s\n\n", tokens[nd->type].name, nd->data))
    
    if (nd == nullptr) {
        err_info ("problems in backend: variable (1)\n");
        return false;
    }
    
    if (nd->type != VARIABLE) {                                       //current node - variable
        err_info ("problems in backend: variable (2)\n");
        return false;
    }
    
    if (nd->left != nullptr) {
        local_vars->append (nd->left->data);
    }
    else {
        err_info ("problems in backend: variable (3)\n");
        return false;
    }
    
    return !have_problems;
}

bool b_end::instructions (node *nd) {
    bool have_problems = false;
    
    if (nd == nullptr) {
        err_info ("problems in backend: instructions (1)\n");
        return false;
    }
    ON_DEBUG (printf ("instructions:%9s\t\t%s\n\n", tokens[nd->type].name, nd->data))
    
    if (nd->type != INSTRUCTION) {
        err_info ("problems in backend: instructions (2)\n");
        return false;
    }
    
    if (nd->left != nullptr) {
        have_problems = have_problems || instructions (nd->left);
    }
    
    if (nd->right != nullptr) {
        switch (nd->right->type) {
            case VARIABLE: {
                have_problems = have_problems || variable (nd->right);
                break;
            }
            case ASSIGN: {
                have_problems = have_problems || assign (nd->right);
                break;
            }
            case RETURN: {
                have_problems = have_problems || func_return (nd->right);
                break;
            }
            case CALL: {
                have_problems = have_problems || func_call (nd->right);
                break;
            }
            case IF: {
                have_problems = have_problems || condition (nd->right);
                break;
            }
            case INCR: {
                have_problems = have_problems || increm (nd->right);
                break;
            }
            case DECR: {
                have_problems = have_problems || decrem (nd->right);
                break;
            }
            case WHILE: {
                have_problems = have_problems || cycle (nd->right);
                break;
            }
            case OUT: {
                have_problems = have_problems || out (nd->right);
                break;
            }
            default: {
                err_info ("problems in backend: instructions (4)\n");
                err_info (nd->data);
            }
        }
    }
    else {
        err_info ("problems in backend: instructions (3)\n");
        return false;
    }
    
    return !have_problems;
}

bool b_end::assign (node *nd) {
    bool have_problems = false;
    
    if (nd == nullptr) {
        err_info ("problems in backend: variable (1)\n");
        return false;
    }
    ON_DEBUG (printf ("assign:%9s\t\t%s\n\n", tokens[nd->type].name, nd->data))
    
    if (nd->type != ASSIGN) {                                       //current node - variable
        err_info ("problems in backend: variable (2)\n");
        return false;
    }
    
    //write ("\npush ", nd->right->data, "\n");
    have_problems = have_problems || equation (nd->right);
    
    popm (reg_name (nd->left));
    /*
    write ("popm [ax+");
    reg_name (nd->left);
    write ("]\n");
     */
    
    return !have_problems;
}

void b_end::pushm (int32_t var_id) {
    write (commands::mov_rax_qword_rbp (var_id), commands::mov_rax_qword_rbp_size);
    cmd (push_rax)
}

void b_end::popm (int32_t var_id) {
    cmd (pop_rax)
    write (commands::mov_qword_rbp_rax (var_id), commands::mov_qword_rbp_rax_size);
}

bool b_end::condition (node *nd) {
    bool have_problems = false;
    char now_cond = cond_number + 1;
    
    if (nd == nullptr) {
        err_info ("problems in backend: condition (1)\n");
        return false;
    }
    ON_DEBUG (printf ("condition:%9s\t\t%s\n\n", tokens[nd->type].name, nd->data))
    
    if (nd->type != IF) {                                       //current node - variable
        err_info ("problems in backend: condition (2)\n");
        return false;
    }
    
    if (nd->left != nullptr) {
        node* cond = nd->left;
        
        if (cond->is_left() && cond->is_right()) {
            //write ("\n");
            push (cond->left);
            push (cond->right);
            cmd (pop_rbx)
            cmd (pop_rax)
            cmd (cmp_rax_rbx)
            
            ON_DEBUG (printf ("cond:%9s\t\t%s\n\n", tokens[cond->type].name, cond->data))
            
            switch (cond->type) {
                case IS_EQUAL: {
                    //fprintf (dist, "je %c\n", cond_number);
                    write (commands::je (cond_number, len), commands::je_size);
                    break;
                }
                case IS_BIGGER: {
                    //fprintf (dist, "ja %c\n", cond_number);
                    write (commands::ja (cond_number, len), commands::ja_size);
                    break;
                }
                case IS_LESS: {
                    //fprintf (dist, "jb %c\n", cond_number);
                    write (commands::jb (cond_number, len), commands::jb_size);
                    break;
                }
                case NOT_EQUAL: {
                    //fprintf (dist, "jne %c\n", cond_number);
                    write (commands::jne (cond_number, len), commands::jne_size);
                    break;
                }
                default: {
                    err_info ("problems in backend: condition (5)\n");
                    return false;
                }
            }
        }
        else {
            err_info ("problems in backend: condition (4)\n");
            return false;
        }
        
        write (commands::jmp (cond_number + 1, len), commands::jmp_size);
        
        commands::labels.insert ({cond_number, len});
        //fprintf (dist, "jump %c\n\nlabel %c\n", cond_number + 1, cond_number);
        cond_number += 2;
    }
    else {
        err_info ("problems in backend: condition (3)\n");
        return false;
    }
    
    if (nd->is_right ()) {
        if (nd->right->type == CONDITION) {
            if (nd->right->is_right()) {
                have_problems = have_problems || instructions (nd->right->right);
            }
            else if (nd->right->is_left()) {
                have_problems = have_problems || instructions (nd->right->left);
            }
        }
        else {
            have_problems = have_problems || instructions (nd->right);
        }
    }
    else {
        err_info ("problems in backend: condition (6)\n");
        return false;
    }
    
    commands::labels.insert ({now_cond, len});
    //fprintf (dist, "label %c\n", now_cond);
    
    return !have_problems;
}

bool b_end::cycle (node *nd) {
    bool have_problems = false;
    
    if (nd == nullptr) {
        err_info ("problems in backend: cycle (1)\n");
        return false;
    }
    ON_DEBUG (printf ("cycle:%9s\t\t%s\n\n", tokens[nd->type].name, nd->data))
    
    if (nd->type != WHILE) {                                       //current node - variable
        err_info ("problems in backend: cycle (2)\n");
        return false;
    }
    
    if (nd->left != nullptr) {
        commands::labels.insert ({cond_number, len});
        //fprintf (dist, "\nlabel %c\n", cond_number);
        
        cond_number += 3;
        
        node* cond = nd->left;
        
        if (cond->is_left() && cond->is_right()) {
            //write ("\n");
            push (cond->left);
            push (cond->right);
            
            switch (cond->type) {
                case IS_EQUAL: {
                    //fprintf (dist, "je %c\n", cond_number - 2);
                    write (commands::je (cond_number - 2, len), commands::je_size);
                    break;
                }
                case IS_BIGGER: {
                    //fprintf (dist, "ja %c\n", cond_number - 2);
                    write (commands::ja (cond_number - 2, len), commands::ja_size);
                    break;
                }
                case IS_LESS: {
                    //fprintf (dist, "jb %c\n", cond_number - 2);
                    write (commands::jb (cond_number - 2, len), commands::jb_size);
                    break;
                }
                case NOT_EQUAL: {
                    //fprintf (dist, "jne %c\n", cond_number - 2);
                    write (commands::jne (cond_number - 2, len), commands::jne_size);
                    break;
                }
                default: {
                    err_info ("problems in backend: cycle (5)\n");
                    return false;
                }
            }
            len += commands::je_size;
            //fprintf (dist, "jump %c\n", cond_number - 1);
            commands::jmp (cond_number - 1, len);
            len += commands::jmp_size;
        }
        else {
            err_info ("problems in backend: cycle (4)\n");
            return false;
        }
        commands::labels.insert ({cond_number - 2, len});
        //fprintf (dist, "\nlabel %c\n", cond_number - 2);
    }
    else {
        err_info ("problems in backend: cycle (3)\n");
        return false;
    }
    
    if (nd->is_right ()) {
        have_problems = have_problems || instructions (nd->right);
        commands::jmp (cond_number - 3, len);
        len += commands::jmp_size;
        commands::labels.insert ({cond_number - 1, len});
        //fprintf (dist, "jump %c\nlabel %c\n", cond_number - 3, cond_number - 1);
    }
    else {
        err_info ("problems in backend: cycle (6)\n");
        return false;
    }
    
    return !have_problems;
}

bool b_end::push (node *nd) {
    if (nd == nullptr) {
        err_info ("problems in backend: push (1)\n");
        return false;
    }
    ON_DEBUG (printf ("push:%9s\t\t%s\n\n", tokens[nd->type].name, nd->data))
    
    if (nd->type == ID) {
        pushm (reg_name (nd));
        /*
        write ("pushm [ax+");
        reg_name (nd);
        write ("]\n");
         */
    }
    else if (nd->type == NUMBER) {
        write (commands::push_num (atoi (nd->data)), commands::push_num_size);
        //write ("push ", nd->data, "\n");
    }
    else {
        err_info ("problems in backend: push (2)\n");
        return false;
    }
    
    return true;
}

bool b_end::increm (node *nd) {
    if (nd == nullptr) {
        err_info ("problems in backend: increm (1)\n");
        return false;
    }
    
    if (nd->type == INCR) {
        if (nd->right == nullptr) {
            err_info ("problems in backend: increm (3)\n");
            return false;
        }
        
        push (nd->right);
        
        //write ("\npush 1\nadd\npopm [ax+");
        cmd (pop_rax)
        cmd (inc_rax)

        write (commands::mov_qword_rbp_rax (reg_name (nd->right)), commands::mov_qword_rbp_rax_size);
        
        //write ("]\n");
        
        return true;
    }
    else {
        err_info ("problems in backend: increm (2)\n");
        return false;
    }
}

bool b_end::decrem (node *nd) {
    if (nd == nullptr) {
        err_info ("problems in backend: decrem (1)\n");
        return false;
    }
    
    if (nd->type == DECR) {
        if (nd->right == nullptr) {
            err_info ("problems in backend: decrem (3)\n");
            return false;
        }
        
        push (nd->right);
        
        //write ("\npush 1\nsub\npopm [ax+");
        cmd (pop_rax)
        cmd (dec_rax)
        
        write (commands::mov_qword_rbp_rax (reg_name (nd->right)), commands::mov_qword_rbp_rax_size);
        
        return true;
    }
    else {
        err_info ("problems in backend: decrem (2)\n");
        return false;
    }
}

bool b_end::out (node *nd) {
    if (nd == nullptr) {
        err_info ("problems in backend: out (1)\n");
        return false;
    }
    
    ON_DEBUG (printf ("out:%9s\t\t%s\n\n", tokens[nd->type].name, nd->data))
    
    if (nd->type == OUT) {
        if (nd->right == nullptr) {
            err_info ("problems in backend: out (3)\n");
            return false;
        }
        
        push (nd->right);
        //write ("out\n");
        cmd (out)
        
        return true;
    }
    else {
        err_info ("problems in backend: decrem (2)\n");
        return false;
    }
}

bool b_end::equation (node *nd) {
    if (nd == nullptr) {
        err_info ("problems in backend: out (1)\n");
        return false;
    }
    
    ON_DEBUG (printf ("equation:%9s\t\t%s\n\n", tokens[nd->type].name, nd->data))
    
    bool have_problems = false;
    
    if (nd->is_left () && nd->type != CALL) {
        have_problems = have_problems || equation (nd->left);
    }
    if (nd->is_right () && nd->type != CALL) {
        have_problems = have_problems || equation (nd->right);
    }
    
    switch (nd->type) {
        case ID: {
            push (nd);
            break;
        }
        case NUMBER: {
            push (nd);
            break;
        }
        case CALL: {
            func_call (nd);
            break;
        }
        case PLUS: {
            cmd (pop_rbx)
            cmd (pop_rax)
            cmd (add_rax_rbx)
            cmd (push_rax)
            //write ("add\n");
            break;
        }
        case MINUS: {
            cmd (pop_rbx)
            cmd (pop_rax)
            cmd (sub_rax_rbx)
            cmd (push_rax)
            //write ("sub\n");
            break;
        }
        case MULT: {
            cmd (pop_rbx)
            cmd (pop_rax)
            cmd (imul_rax_rbx)
            cmd (push_rax)
            //write ("mul\n");
            break;
        }
        case DIVIDE: {
            cmd (pop_rbx)
            cmd (pop_rax)
            cmd (idiv_rbx)
            cmd (push_rax)
            //write ("div\n");
            break;
        }
    }
    
    return have_problems;
}

void b_end::fill_labels () {
    for (unsigned char label = 0; label < cond_number; ++label) {
        try {
            auto range = commands::to_fill.equal_range (label);
            for (auto element = range.first; element != range.second; ++element) {
                ON_DEBUG (printf ("Offsets of label %d: %x - %x\n", label, 0x250 + commands::labels.at (label), 0x250 + element->second.next_instruction))
                *reinterpret_cast<int*> (code + element->second.offset) = commands::labels.at (label) - element->second.next_instruction;
            }
        }
        catch (std::out_of_range) {
            ON_DEBUG (printf ("There was no label %d in one of two maps\n", label));
        }
    }
}

bool backend (node* nd, const char* program_name, int precision) {
    b_end nds = b_end (nd, precision);
    //nds.write ("call reign\nend\n\n");
    bool were_problems = false;
    try {
        were_problems = nds.all ();
    } catch (std::runtime_error) {
        were_problems = false;
    }
    
    nds.fill_labels ();
    
    make_file (program_name, nds.code, nds.len);
    return were_problems;
}
