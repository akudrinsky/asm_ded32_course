#ifndef LIST_CPP_WAS_DEFINED
#define LIST_CPP_WAS_DEFINED
#include "list.h"

bool is_loud = true;

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Prints error info to a given file
//!
//! @param [in] str - error info
//! @param [in] file - name of file
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
void err_info (const char* str, const char* file) {
    ASSERT (file != nullptr)
    ASSERT (str != nullptr)
    FILE* pFile = fopen (file, "a");
    ASSERT (pFile != nullptr)
    
    fprintf (pFile, "%s", str);
    
    fclose (pFile);
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! List construct
//!
//! @param [in] lst - pointer to list
//! @param [in] lst_name - name of list (if you don't need it, it will be "lst" by default)
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
int List_Construct (List_t* lst, const char* lst_name, const int starting_size) {
    ASSERT (lst != nullptr)
    ASSERT (lst_name != nullptr)
    
    lst->name = lst_name;
    lst->data = (Elem_t *)calloc (starting_size, sizeof(Elem_t));
    lst->next = (Elem_t *)calloc (starting_size, sizeof(Elem_t));
    lst->prev = (Elem_t *)calloc (starting_size, sizeof(Elem_t));
    
    if (lst->name == nullptr || lst->next == nullptr || lst->prev == nullptr || lst->data == nullptr) {
        if (is_loud)
            printf ("We have no memory left!\n");
        return no_memory;
    }
    
    for (int i = 1; i < starting_size; ++i) {
        lst->next[i] = i + 1;
        lst->prev[i] = -1;
    }
    lst->next[starting_size - 1] = 0;
    
    lst->head = 0;
    lst->tail = 0;
    lst->free = 1;
    lst->size = 0;
    lst->max_size = starting_size;
    lst->is_sorted = true;
    lst->status = ok;
    
    List_OK (*lst);
    return 0;
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! List desctruct
//!
//! @param [in] lst - pointer to list
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
int Lt_destruct (List_t* lst) {
    ASSERT (lst != nullptr)
    
    if (lst->status == destructed) {
        if (is_loud)
            err_info ("You are trying to destruct what had already been destructed!\n");
        return destructed;
    }
    else {
        lst->name = nullptr;
        free (lst->data);
        free (lst->next);
        free (lst->prev);
        lst->head = 0;
        lst->tail = 0;
        lst->free = 0;
        lst->size = 0;
        lst->max_size = 0;
        lst->is_sorted = true;
        lst->status = destructed;
    }
    
    return 0;
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//!
//! @param [in] lst - pointer to list
//! @param [in] file - file where this Dump_f was called
//! @param [in] line - line in file where this Dump_f was called
//! @param [in] func_name - name of function in file where this Dump_f was called
//! @param [in] is_silent - if you need to stop the program if list is not ok
//! @param [in] no_corr - if you need to print info about valid list or not
//! @param [in] file_name - where to print info about list
//!
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
void Dump_f (const List_t* lst, const char* file,
             int line, const char* func_name, const char* info,
             bool is_silent, bool no_corr, const char* file_name) {
    ASSERT (lst != nullptr)
    
    FILE* pFile = fopen (file_name, "ab");
    ASSERT (pFile != nullptr)
    
    const char* status = "ok";
    if (lst->status != ok)
        status = "NOT OK";
    
    if (!no_corr || lst->status != 0) {
        fprintf (pFile, "%s from %s in line %d (file: %s)\n", info, func_name, line, file);
        fprintf (pFile, "List_t <%s> [%p] (%s)\n", lst->name, lst, status);
        fprintf (pFile, "{\n\tsize = %d\n\tdata[%d] = %p\n", lst->size, lst->max_size, lst->data);
        fprintf (pFile, "\t{\n");
        
        for (int i = 0; i < lst->size; ++i) {
            fprintf (pFile, "\t* [%2d] = %3d;\t next = %2d;\t prev = %2d\n", i, lst->data[i], lst->next[i], lst->prev[i]);
        }
        
        for (int i = lst->size; i < lst->max_size; ++i) {
            fprintf (pFile, "\t  [%2d] = %3d;\t next = %2d;\t prev = %2d\n", i, lst->data[i], lst->next[i], lst->prev[i]);
        }
        fprintf(pFile, "\t}\n");
        
        switch (lst->status) {
            case ok:
                break;
            case nulldata:
                fprintf (pFile, "Pointer to data is nullptr\n");
                break;
            case size_out_of_max:
                fprintf (pFile, "Number of elements is bigger than max value\n");
                break;
            case destructed:
                fprintf (pFile, "List was destructed\n");
                break;
            default:
                fprintf (pFile, "Unknown lst status\n");
                break;
        }
        fprintf (pFile, "\thead = %d\n", lst->head);
        fprintf (pFile, "\ttail = %d\n", lst->tail);
        fprintf (pFile, "\tfree = %d\n", lst->free);
        fprintf (pFile, "\tis_sorted = %d\n", lst->is_sorted);
        fprintf (pFile, "}\n\n\n");
    }
    
    if (!is_silent && lst->status != 0) {
        err_info ("Abort was made. For info look in list_dump.txt\n");
        abort ();
    }
    
    fclose (pFile);
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Makes picture of a list
//!
//! @param [in] lst - pointer to list
//! @param [in] pict_name - name of picture (list.png by default)
//! @param [in] pict_type - type of picture (png by default)
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
void list_graph (List_t* lst, const char* pict_name, const char* pict_type) {
    ASSERT (lst != nullptr)
    ASSERT (pict_name != nullptr)
    ASSERT (pict_type != nullptr)
    
    FILE* pFile = fopen ("list_graph.dot", "w");
    ASSERT (pFile != nullptr)
    
    fprintf (pFile, "digraph G{\nedge[color=\"chartreuse4\",fontcolor=\"blue\",fontsize=12];\nnode[shape=\"rectangle\",fontsize=15];\n");
    for (int i = 1; i < lst->max_size; ++i) {
        fprintf (pFile, " %d [color=\"white\",label=<<table border=\"0\" cellborder=\"1\" cellspacing=\"0\"><tr><td bgcolor=\"lightblue\">%d</td></tr>\n\t<tr><td bgcolor=\"#f0e3ff\">%d</td></tr></table>>];\n", i, i, lst->data[i]);
    }
    
    int pos = lst->head;
    while (lst->next[pos] != 0) {
        fprintf (pFile, "%d->%d;\n", pos, lst->next[pos]);
        pos = lst->next[pos];
    }
    
    fprintf (pFile, "edge[color=\"cyan3\",fontcolor=\"blue\",fontsize=12];\n");
    pos = lst->tail;
    while (lst->prev[pos] != 0) {
        fprintf (pFile, "%d->%d;\n", pos, lst->prev[pos]);
        pos = lst->prev[pos];
    }
    
    fprintf (pFile, "edge[color=\"palevioletred4\",fontcolor=\"blue\",fontsize=12];\n");
    pos = lst->free;
    while (lst->next[pos] != 0) {
        fprintf (pFile, "%d->%d;\n", pos, lst->next[pos]);
        pos = lst->next[pos];
    }
    fprintf (pFile, "info [shape=record,label=\"{green arrow - next|blue arrow - prev|brown arrow - free|blue cell - physical number|pink cell - data}\",fillcolor=\"palegreen1\"];\n");
    
    fprintf (pFile, "}");
    fclose (pFile);
    
    //executing command in terminal
    const int max_cmd_size = 50;
    char command[max_cmd_size] = "dot list_graph.dot -T ";
    strcat (command, pict_type);
    strcat (command, " -o ");
    strcat (command, pict_name);
    
    system (command);
    
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Answers if list is valid or not
//!
//! @param [in] lst - pointer to list
//! @return if everything is OK
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
bool List_okey (List_t* lst) {
    if (lst == nullptr) {
        return false;
    }
    
    if (lst->data == nullptr || lst->next == nullptr || lst->prev == nullptr || lst->name == nullptr) {
        lst->status = nulldata;
        return false;
    }
    
    if (lst->size > lst->max_size) {
        lst->status = size_out_of_max;
        return false;
    }
    
    if (lst->max_size < 10) {
        lst->status = strange_value;
        return false;
    }
    
    if (lst->size < 0) {
        lst->status = strange_value;
        return false;
    }
    
    if (lst->status != ok) {
        return false;
    }
    
    return true;
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Inserts element after a given position
//!
//! @param [in] lst - pointer to list
//! @param [in] pos - physical number of element to insert elem after
//! @param [in] elem - element to insert
//! @return physical number of where element was written at
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
int insert_after (List_t* lst, const int pos, const Elem_t elem) {
    ASSERT (lst != nullptr)
    
    if (lst->prev[pos] == -1) {
        if (is_loud)
            err_info ("Trying to insert element after non-existing element (it was not in list)\n");
        lst->status = calling_nowhere;
        List_OK (*lst)
        return 0;
    }
    else {
        int tmp = lst->next[lst->free];
        //err_info ("pos = %d = %d = tail\n", pos, lst->tail);
        
        if (pos == lst->tail) {
            lst->tail = lst->free;
        }
        else {
            lst->is_sorted = false;
        }
        
        lst->prev[lst->next[pos]] = lst->free;
        lst->data[lst->free] = elem;
        lst->prev[lst->free] = pos;
        lst->next[lst->free] = lst->next[pos];
        
        lst->next[pos] = lst->free;
        
        if (pos == 0) {
            lst->head = lst->free;
        }
        
        lst->free = tmp;
        ++lst->size;
        
        List_OK (*lst)
        return tmp;
    }
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Inserts element before a given position
//!
//! @param [in] lst - pointer to list
//! @param [in] pos - physical number of element to insert elem before
//! @param [in] elem - element to insert
//! @return physical number of where element was written at
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
int insert_before (List_t* lst, const int pos, const Elem_t elem) {
    ASSERT (lst != nullptr)
    
    if (lst->prev[pos] == -1) {
        if (is_loud)
            err_info ("Trying to insert element before non-existing element (it was not in list)\n");
        lst->status = calling_nowhere;
        List_OK (*lst)
        return 0;
    }
    else {
        int tmp = lst->next[lst->free];                             //next free element
        //err_info ("pos = %d = %d = tail\n", pos, lst->tail);
        
        //if inserting before first (logically) element
        if (pos == lst->head) {
            lst->head = lst->free;
        }
        
        lst->next[lst->prev[pos]] = lst->free;
        lst->data[lst->free] = elem;
        lst->next[lst->free] = pos;
        lst->prev[lst->free] = lst->prev[pos];
        
        lst->prev[pos] = lst->free;
        
        //if list was empty
        if (lst->head == 0) {
            lst->head = lst->free;
        }
        
        lst->free = tmp;
        ++lst->size;
        
        lst->is_sorted = false;
        
        List_OK (*lst)
        return tmp;
    }
}
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Deletes element by a given position
//!
//! @param [in] lst - pointer to list
//! @param [in] pos - physical number of element to delete
//! @return physical number of deleted element
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
int delete_el (List_t* lst, const int pos) {
    ASSERT (lst != nullptr)
    
    if (lst->prev[pos] == -1) {
        if (is_loud)
            err_info ("Trying to delete non-existing element (it was not in list)\n");
        lst->status = calling_nowhere;
        List_OK (*lst)
        return 0;
    }
    else {
        if (pos != lst->tail || pos != lst->head) {
            lst->is_sorted = false;
        }
        lst->next[lst->prev[pos]] = lst->next[pos];
        lst->prev[lst->next[pos]] = lst->prev[pos];
        
        lst->data[pos] = 0;
        lst->prev[pos] = -1;
        //list_graph (lst);
        
        lst->next[pos] = lst->free;
        lst->free = pos;
        --lst->size;
        
        List_OK (*lst)
        return pos;
    }
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Deletes element after a given position
//!
//! @param [in] lst - pointer to list
//! @param [in] pos - physical number of element element to delete after
//! @return physical number of deleted element
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
int delete_after (List_t* lst, const int pos) {
    ASSERT (lst != nullptr)
    
    if (lst->prev[pos] == -1) {
        if (is_loud)
            err_info ("Trying to delete element after non-existing element (it was not in list)\n");
        lst->status = calling_nowhere;
        List_OK (*lst)
        return 0;
    }
    else {
        int tmp = lst->next[pos];
        if (tmp != lst->tail) {
            lst->is_sorted = false;
        }
        lst->next[pos] = lst->next[lst->next[pos]];
        lst->prev[lst->next[pos]] = pos;
        
        lst->data[tmp] = 0;
        lst->prev[tmp] = -1;
        
        lst->next[tmp] = lst->free;
        lst->free = tmp;
        --lst->size;
        
        List_OK (*lst)
        return tmp;
    }
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Deletes element before a given position
//!
//! @param [in] lst - pointer to list
//! @param [in] pos - physical number of element element to delete before
//! @return physical number of deleted element
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
int delete_before (List_t* lst, const int pos) {
    ASSERT (lst != nullptr)
    
    if (lst->prev[pos] == -1) {
        if (is_loud)
            err_info ("Trying to delete element before non-existing element (it was not in list)\n");
        lst->status = calling_nowhere;
        List_OK (*lst)
        return 0;
    }
    else {
        int tmp = lst->prev[pos];               // - physical number of element to delete
        lst->prev[pos] = lst->prev[tmp];
        lst->next[lst->prev[tmp]] = pos;
        
        lst->data[tmp] = 0;
        lst->prev[tmp] = -1;
        
        lst->next[tmp] = lst->free;
        lst->free = tmp;
        --lst->size;
        
        lst->is_sorted = false;
        
        List_OK (*lst)
        return tmp;
    }
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Makes physical numbers of elements match their logical ones (to fasten search)
//!
//! @param [in] lst - pointer to list
//! @return if everything was okey
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
bool phys_match_log (List_t* lst) {
    ASSERT (lst != nullptr)
    
    Elem_t* new_data = (Elem_t*) calloc (lst->max_size, sizeof (Elem_t));
    
    if (new_data == nullptr) {
        if (is_loud)
            err_info ("We have no memory for new_data!\n");
        return false;
    }
    
    int i = 0;
    for (i = 1; i <= lst->size; ++i) {
        new_data[i] = lst->data[lst->head];
        lst->head = lst->next[lst->head];
    }
    
    lst->head = 1;
    lst->tail = lst->size;
    lst->free = lst->size + 1;
    
    free (lst->data);
    lst->data = new_data;
    
    for (int j = 1; j <= lst->size; ++j) {
        lst->next[j] = j + 1;
        lst->prev[j] = j - 1;
    }
    lst->next[i-1] = 0;
    
    for (; i < lst->max_size; ++i) {
        lst->next[i] = i + 1;
        lst->prev[i] = -1;
    }
    lst->next[lst->max_size-1] = 0;
    lst->is_sorted = true;
    
    List_OK (*lst)
    return true;
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//! Searches for value in a list
//!
//! @param [in] lst - pointer to list
//! @param [in] value - value to search for
//! @param [in] need_log - if we need logical number or physical one
//! @return position of element with such value (0 if was not found)
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
int val_in_list (List_t* lst, const int value, const bool need_log) {
    ASSERT (lst != nullptr)
    
    int pos = lst->head;
    int log_number = 1;
    
    while (pos != lst->tail) {
        //err_info ("pos: %d\n", pos);
        if (lst->data[pos] == value) {
            if (need_log) {
                return log_number;
            }
            else {
                return pos;
            }
        }
        pos = lst->next[pos];
        ++log_number;
    }
    
    if (lst->data[pos] == value) {          //in case the value is the last one
        if (need_log) {
            return log_number;
        }
        else {
            return pos;
        }
    }
    
    return 0;
}

//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
//!
//! BIG test for list
//!
//‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
void list_test () {
    List_t lst = {};
    Lt_construct (lst)
    
    insert_after (&lst, 0, 1);
    insert_after (&lst, 0, 2);
    insert_before (&lst, 2, 3);
    
    delete_before (&lst, 2);
    
    insert_after (&lst, 0, 4);
    insert_after (&lst, 3, 5);
    
    if (lst.size != 4) {
        printf ("list_text_1 is not ok (1)\n");
    }
    if (lst.head != 3) {
        printf ("list_text_1 is not ok (3)\n");
    }
    if (lst.tail != 1) {
        printf ("list_text_1 is not ok (4)\n");
    }
    if (lst.data[2] != 2) {
        printf ("list_text_1 is not ok (5)\n");
    }
    if (lst.free != 5) {
        printf ("list_text_1 is not ok (6)\n");
    }
    
    phys_match_log (&lst);
    
    if (lst.free != 5) {
        printf ("list_text_1 is not ok (7)\n");
    }
    if (lst.head != 1) {
        printf ("list_text_1 is not ok (9)\n");
    }
    if (lst.size != 4) {
        printf ("list_text_1 is not ok (10)\n");
    }
    if (lst.data[2] != 5) {
        printf ("list_text_1 is not ok (11)\n");
    }
    if (lst.tail != 4) {
        printf ("list_text_1 is not ok (12)\n");
    }
    
    Lt_destruct (&lst);
    
    Lt_construct ( lst);
    
    for (int i = 1; i < 6; ++i) {
        insert_after (&lst, i - 1, i);
    }
    
    if (lst.free != 6) {
        printf ("list_text_1 is not ok (13)\n");
    }
    if (lst.head != 1) {
        printf ("list_text_1 is not ok (15)\n");
    }
    if (lst.size != 5) {
        printf ("list_text_1 is not ok (16)\n");
    }
    if (lst.tail != 5) {
        printf ("list_text_1 is not ok (17)\n");
    }
    
    for (int i = 1; i < 6; ++i) {
        if (lst.data[i] != i) {
            printf ("list_text_1 is not ok (18)\n");
        }
        if (lst.next[i-1] != i) {
            printf ("list_text_1 is not ok (19)\n");
        }
        if (lst.prev[i] != i - 1) {
            printf ("list_text_1 is not ok (20)\n");
        }
    }
    
    for (int i = 6; i < 9; ++i) {
        if (lst.data[i] != 0) {
            printf ("list_text_1 is not ok (21)\n");
        }
        if (lst.next[i] != i + 1) {
            printf ("list_text_1 is not ok (22)\n");
        }
        if (lst.prev[i] != -1) {
            printf ("list_text_1 is not ok (23)\n");
        }
    }
    
    bool need_log = true;
    if (val_in_list (&lst, 2, need_log) != 2) {
        printf ("list_text_1 is not ok (24)\n");
    }
    
    insert_before (&lst, lst.tail, 10);
    if (val_in_list (&lst, 10, !need_log) != 6) {
        printf ("list_text_1 is not ok (25)\n");
    }
    
    delete_after (&lst, 3);
    //delete_el (&lst, 3);
    
    list_graph (&lst);
    
    if (lst.next[4] != 7) {
        printf ("list_text_1 is not ok (26)\n");
    }
    if (lst.prev[4] != -1) {
        printf ("list_text_1 is not ok (26)\n");
    }
    if (lst.prev[6] != 3) {
        printf ("list_text_1 is not ok (27)\n");
    }
    
    Lt_destruct (&lst);
}

#endif
