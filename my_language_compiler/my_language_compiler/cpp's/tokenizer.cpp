//
//  tokenizer.cpp
//  macho-compile
//
//  Created by Alexey Kudrinsky on 15.05.2020.
//  Copyright © 2020 Alexey Kudrinsky. All rights reserved.
//

#include <stdio.h>
#include "../headers/tokenizer.h"

bool not_special (char symbol) {
    return (symbol != ' ' && symbol != '\n' && symbol != '.' &&
            symbol != ',' && symbol != '\t' && symbol != '$' &&
            symbol != '+' && symbol != '-' && symbol != '*' &&
            symbol != '/');
}

bool not_divider (char symbol) {
    return (symbol != ' ' && symbol != '\n' &&
            symbol != '\t' && symbol != '$' &&
            symbol != '\0');
}

for_funcs::for_funcs (int size) {
    names = new node*[size];
    amount = 0;
}

void for_funcs::append (node* element) {
    names[amount] = element;
    ++amount;
}

void for_funcs::see () {
    for (int i = 0; i < amount; ++i) {
        printf ("func: %s\n", names[i]->right->data);
    }
}

int for_funcs::search_func (node* needed) {
    for (int i = 0; i < amount; ++i) {
        if (strcmp (names[i]->data, needed->data) == 0) {
            return i;
        }
    }
    return -1;
}

for_funcs::~for_funcs() {
    delete [] names;
    amount = -1;
}

cell::cell() {
    name = new char[default_size];
    type = UNKNOWN;
}

cell::~cell () {
    delete [] name;
    type = UNKNOWN;
}

cell* tokenize (char* str, const int max_nodes_number) {
    
    cell *nodes = new cell[max_nodes_number];
    int n_nodes = 0;
    
    while (*str != '\0') {
        
        //printf ("%s\n\n<------------------------------------>\n\n", str);
        
        while (*str == ' ' || *str == '\n' || *str == '\t' || *str == ',') {
            ++str;
        }
        
        if (*str == '$') {                                      //comments
            ++str;
            int scanfed = 0;
            char* for_rubbish = new char[default_size];
            sscanf (str, "%[^$]%n", for_rubbish, &scanfed);
            
            str += scanfed + 1;                                 // + '$'
            delete[] for_rubbish;
        }
        
        while (*str == ' ' || *str == '\n' || *str == '\t' || *str == ',') {
            ++str;
        }
        
        char* data = new char[default_size];
        
        for (int i = 0; i < default_size; ++i) {
            data[i] = '\0';
        }
        int data_len = 0;
        
        if (is_digit (*str)) {
            data[data_len] = *str;
            ++data_len;
            ++str;
            while (is_digit (*str) || *str == '.') {
                data[data_len] = *str;
                ++data_len;
                ++str;
            }
            nodes[n_nodes].type = NUMBER;
        }
        else {
            switch (*str) {
                case 'd': {
                    if (strncmp (str, "dictate", 7) == 0) {
                        nodes[n_nodes].type = DEFINITION;
                        strncpy (data, "dictate", default_size);
                        str += 7;
                        //printf ("flag\n");
                    }
                    else if (strncmp (str, "dead", 4) == 0) {
                        nodes[n_nodes].type = NUMBER;
                        strncpy (data, states[DEAD].name, default_size);
                        str += states[DEAD].length;
                    }
                    else if (strncmp (str, "disloyal", 9) == 0) {
                        nodes[n_nodes].type = NUMBER;
                        strncpy (data, states[DISLOYAL].name, default_size);
                        str += states[DISLOYAL].length;
                    }
                    else if (strncmp (str, "destroyed", 9) == 0) {
                        nodes[n_nodes].type = NUMBER;
                        strncpy (data, states[DESTROYED].name, default_size);
                        str += states[DESTROYED].length;
                    }
                    else if (strncmp (str, "dishonest", 9) == 0) {
                        nodes[n_nodes].type = NUMBER;
                        strncpy (data, states[DISHONEST].name, default_size);
                        str += states[DISHONEST].length;
                    }
                    else {
                        now_parse_that_long_long_name
                    }
                    break;
                }
                case ':': {
                    nodes[n_nodes].type = OPEN_BR;
                    strncpy (data, ":", default_size);
                    str += 1;
                    break;
                }
                case '.': {
                    nodes[n_nodes].type = CLOS_BR;
                    strncpy (data, ".", default_size);
                    str += 1;
                    break;
                }
                case '(': {
                    nodes[n_nodes].type = OPEN_EQ;
                    strncpy (data, "(", default_size);
                    str += 1;
                    break;
                }
                case ')': {
                    nodes[n_nodes].type = CLOS_EQ;
                    strncpy (data, ")", default_size);
                    str += 1;
                    break;
                }
                case '\"': {
                    nodes[n_nodes].type = COND_DIVIDER;
                    strncpy (data, "\"", default_size);
                    str += 1;
                    break;
                }
                case 'p': {
                    if (strncmp (str, "person", 6) == 0) {
                        nodes[n_nodes].type = VARIABLE;
                        strncpy (data, "person", default_size);
                        str += 6;
                    }
                    else if (strncmp (str, "punish", 6) == 0) {
                        nodes[n_nodes].type = DECR;
                        strncpy (data, "punish", default_size);
                        str += 6;
                    }
                    else if (strncmp (str, "mandate", 7) == 0) {
                        nodes[n_nodes].type = PRINT;
                        strncpy (data, "mandate", default_size);
                        str += 7;
                    }
                    else {
                        now_parse_that_long_long_name
                    }
                    break;
                }
                case 'n': {
                    if (strncmp (str, "now", 3) == 0) {
                        nodes[n_nodes].type = ASSIGN;
                        strncpy (data, "now", default_size);
                        str += 3;
                    }
                    else if (strncmp (str, "not", 3) == 0) {
                        nodes[n_nodes].type = NOT_EQUAL;
                        strncpy (data, "not", default_size);
                        str += 3;
                    }
                    else {
                        now_parse_that_long_long_name
                    }
                    break;
                }
                case 'i': {
                    if (strncmp (str, "if", 2) == 0) {
                        nodes[n_nodes].type = IF;
                        strncpy (data, "if", default_size);
                        str += 2;
                    }
                    else if (strncmp (str, "is", 2) == 0) {
                        nodes[n_nodes].type = IS_EQUAL;
                        strncpy (data, "is", default_size);
                        str += 2;
                    }
                    else {
                        now_parse_that_long_long_name
                    }
                    break;
                }
                case 'w': {
                    if (strncmp (str, "while", 5) == 0) {
                        nodes[n_nodes].type = WHILE;
                        strncpy (data, "while", default_size);
                        str += 5;
                    }
                    else if (strncmp (str, "worse", 5) == 0) {
                        nodes[n_nodes].type = IS_LESS;
                        strncpy (data, "worse", default_size);
                        str += 5;
                    }
                    else {
                        now_parse_that_long_long_name
                    }
                    break;
                }
                case 'o': {
                    if (strncmp (str, "otherwise", 9) == 0) {
                        nodes[n_nodes].type = CONDITION;
                        strncpy (data, "otherwise", default_size);
                        str += 9;
                    }
                    else if (strncmp (str, "of", 2) == 0) {
                        nodes[n_nodes].type = ARGUMENTS;
                        strncpy (data, "of", default_size);
                        str += 2;
                    }
                    else {
                        now_parse_that_long_long_name
                    }
                    break;
                }
                case 'h': {
                    if (strncmp (str, "honor", 5) == 0) {
                        nodes[n_nodes].type = INCR;
                        strncpy (data, "honor", default_size);
                        str += 5;
                    }
                    else if (strncmp (str, "happy", 5) == 0) {
                        nodes[n_nodes].type = NUMBER;
                        strncpy (data, states[HAPPY].name, default_size);
                        str += states[HAPPY].length;
                    }
                    else {
                        now_parse_that_long_long_name
                    }
                    break;
                }
                case '+': {
                    nodes[n_nodes].type = PLUS;
                    strncpy (data, "+", default_size);
                    str += 1;
                    break;
                }
                case '-': {
                    nodes[n_nodes].type = MINUS;
                    strncpy (data, "-", default_size);
                    str += 1;
                    break;
                }
                case '*': {
                    nodes[n_nodes].type = MULT;
                    strncpy (data, "*", default_size);
                    str += 1;
                    break;
                }
                case '/': {
                    nodes[n_nodes].type = DIVIDE;
                    strncpy (data, "/", default_size);
                    str += 1;
                    break;
                }
                case 'b': {
                    if (strncmp (str, "better", 6) == 0) {
                        nodes[n_nodes].type = IS_BIGGER;
                        strncpy (data, "better", default_size);
                        str += 6;
                    }
                    else {
                        now_parse_that_long_long_name
                    }
                    break;
                }
                case 'c': {
                    if (strncmp (str, "cause", 5) == 0) {
                        nodes[n_nodes].type = CALL;
                        strncpy (data, "cause", default_size);
                        str += 5;
                    }
                    else if (strncmp (str, "confused", 8) == 0) {
                        nodes[n_nodes].type = NUMBER;
                        strncpy (data, states[CONFUSED].name, default_size);
                        str += states[CONFUSED].length;
                    }
                    else {
                        now_parse_that_long_long_name
                    }
                    break;
                }
                case 's': {
                    if (strncmp (str, "summon", 6) == 0) {
                        nodes[n_nodes].type = RETURN;
                        strncpy (data, "summon", default_size);
                        str += 6;
                    }
                    else if (strncmp (str, "sad", 3) == 0) {
                        nodes[n_nodes].type = NUMBER;
                        strncpy (data, states[SAD].name, default_size);
                        str += states[SAD].length;
                    }
                    else {
                        now_parse_that_long_long_name
                    }
                    break;
                }
                case 'a': {
                    if (strncmp (str, "afraid", 6) == 0) {
                        nodes[n_nodes].type = NUMBER;
                        strncpy (data, states[AFRAID].name, default_size);
                        str += states[AFRAID].length;
                    }
                    else if (strncmp (str, "angry", 5) == 0) {
                        nodes[n_nodes].type = NUMBER;
                        strncpy (data, states[ANGRY].name, default_size);
                        str += states[ANGRY].length;
                    }
                    else if (strncmp (str, "and", 3) == 0) {
                        nodes[n_nodes].type = LAST_ARG;
                        strncpy (data, "and", default_size);
                        str += 3;
                    }
                    else if (strncmp (str, "a ", 2) == 0) {
                        str += 2;
                        while (*str == ' ') {
                            ++str;
                        }
                        int scanfed = 0;
                        
                        while (not_divider (*str)) {
                            ++str;
                            ++scanfed;
                        }
                        
                        itoa_ (scanfed, data);
                        nodes[n_nodes].type = NUMBER;
                    }
                    else {
                        now_parse_that_long_long_name
                    }
                    break;
                }
                case 'f': {
                    if (strncmp (str, "furious", 7) == 0) {
                        nodes[n_nodes].type = NUMBER;
                        strncpy (data, states[FURIOUS].name, default_size);
                        str += states[FURIOUS].length;
                    }
                    if (strncmp (str, "forgiven", 8) == 0) {
                        nodes[n_nodes].type = NUMBER;
                        strncpy (data, states[FORGIVEN].name, default_size);
                        str += states[FORGIVEN].length;
                    }
                    else {
                        now_parse_that_long_long_name
                    }
                    break;
                }
                case 'l': {
                    if (strncmp (str, "loyal", 5) == 0) {
                        nodes[n_nodes].type = NUMBER;
                        strncpy (data, states[LOYAl].name, default_size);
                        str += states[LOYAl].length;
                    }
                    else {
                        now_parse_that_long_long_name
                    }
                    break;
                }
                case 'k': {
                    if (strncmp (str, "know", 4) == 0) {
                        nodes[n_nodes].type = OUT;
                        strncpy (data, "know", default_size);
                        str += 4;
                    }
                    else {
                        now_parse_that_long_long_name
                    }
                    break;
                }
                default: {
                    now_parse_that_long_long_name
                }
            }
        }
        
        delete [] data;
        
        strncpy (nodes[n_nodes].name, data, default_size);
        //nodes[n_nodes].name = data;
        ++n_nodes;
    }
    
    return nodes;
}
