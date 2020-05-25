#ifndef B_TREE_CPP
#define B_TREE_CPP

#include "b_tree.h"
#define debug(text) ;

template<typename T, const int min_degree, class compare>
B_tree<T, min_degree, compare>::B_tree () {
    root = new B_node;
}

template<typename T, const int min_degree, class compare>
B_tree<T, min_degree, compare>::B_node::B_node () {
    size = 0;
    tree_size = 0;
    keys = new T[2 * min_degree - 1];
    children = new B_node*[2 * min_degree];
    is_leaf = true;
}

template<typename T, const int min_degree, class compare>
bool B_tree<T, min_degree, compare>::B_node::is_in (const T& value) {
    compare less;
    for (int i = 0; i < size; ++i) {
        if (!less (keys[size], value) and !less (value, keys[size])) {
            return true;
        }
    }
    return false;
}

template<typename T, const int min_degree, class compare>
void B_tree<T, min_degree, compare>::insert (const T& value) {
    compare less;

    if (root == nullptr) {
        root = new B_node;
        root->keys[0] = value;
        root->size = 1;
        root->tree_size = 1;
    }
    else {
        if (root->is_full ()) {
            B_node* new_root = new B_node;
            new_root->is_leaf = false;
            new_root->children[0] = root;
            new_root->tree_size = root->tree_size + 1;
            new_root->split_child (0);

            int index = 0;
            if (less (new_root->keys[0], value)) {
                ++index;
            }
            new_root->children[index]->insert (value);

            root = new_root;
        }
        else {
            root->insert (value);
        }
    }
}

template<typename T, const int min_degree, class compare>
void B_tree<T, min_degree, compare>::B_node::insert (const T& value) {
    ++tree_size;
    compare less;

    int index = size - 1;

    if (is_leaf) {
        while (index >= 0 and less (value, keys[index])) {
            keys[index + 1] = keys[index];
            --index;
        }

        keys[index + 1] = value;
        ++size;
    }
    else {
        while (index >= 0 and less (value, keys[index])) {
            --index;
        }

        if (children[index + 1]->is_full ()) {
            split_child (index + 1);

            // after split we need to see where our value goes
            if (less (keys[index + 1], value)) {
                ++index;
            }
        }

        children[index + 1]->insert (value);
    }
}

template<typename T, const int min_degree, class compare>
void B_tree<T, min_degree, compare>::B_node::split_child (int index) {
    B_node* to_split = children[index];
    to_split->tree_size = min_degree - 1;
    B_node* node_new = new B_node;
    node_new->is_leaf = to_split->is_leaf;

    node_new->size = min_degree - 1;
    node_new->tree_size = min_degree - 1;
    for (int i = 0; i < min_degree - 1; ++i) {
        node_new->keys[i] = to_split->keys[min_degree + i];
    }

    if (!to_split->is_leaf) {
        for (int i = 0; i < min_degree; ++i) {
            node_new->children[i] = to_split->children[min_degree + i];
            node_new->tree_size += to_split->children[min_degree + i]->tree_size;
            to_split->tree_size += to_split->children[i]->tree_size;
        }
    }

    to_split->size = min_degree - 1;

    for (int i = size; i > index; --i) {
        children[i + 1] = children[i];
    }

    children[index + 1] = node_new;

    for (int i = size - 1; i >= index; --i) {
        keys[i + 1] = keys[i];
    }

    keys[index] = to_split->keys[min_degree - 1];

    ++size;
}

template<typename T, const int min_degree, class compare>
bool B_tree<T, min_degree, compare>::B_node::is_full () {
    return size == 2 * min_degree - 1;
}

template<typename T, const int min_degree, class compare>
std::pair<bool, T> B_tree<T, min_degree, compare>::B_node::next (const T& value) {
    compare less;
    if (is_leaf) {
        for (int i = 0; i < size; ++i) {
            if (!less (keys[i], value)) {                       // i - first index in this node such that ...
                return std::make_pair (true, keys[i]);
            }
        }
        return std::make_pair (false, -1);
    }
    else {
        for (int i = 0; i < size; ++i) {
            if (!less (keys[i], value)) {                       // i - first index in this node such that ...
                std::pair<bool, const T&> before_found = children[i]->next (value);
                if (before_found.first) {
                    return before_found;
                }
                else {
                    return std::make_pair (true, keys[i]);
                }
            }
        }
        return children[size]->next (value);
    }
}

template<typename T, const int min_degree, class compare>
std::ostream& operator<< (std::ostream& out, const B_tree<T, min_degree, compare>& tree) {
    std::string id = "$";
    std::cout << "Printing tree: root's id is $\n";
    tree.root->print (id);
}

template<typename T, const int min_degree, class compare>
B_tree<T, min_degree, compare>::~B_tree () {
    root->destroy_subtree ();
}

template<typename T, const int min_degree, class compare>
void B_tree<T, min_degree, compare>::B_node::destroy_subtree () {
    if (!is_leaf) {
        for (int i = 0; i <= size; ++i) {
            children[i]->destroy_subtree ();
        }
    }
    delete[] children;
    delete[] keys;
}

template<typename T, const int min_degree, class compare>
void B_tree<T, min_degree, compare>::B_node::print (const std::string& id) {
    std::cout << "<---------------------------->\nVertice id: " << id << (is_leaf ? " - leaf" : " - not a leaf") << "\ttree size: " << tree_size << "\nKeys are: \n";
    for (int i = 0; i < size; ++i) {
        std::cout << i << ": " << keys[i] << '\n';
    }
    std::cout << "\n<---------------------------->\n\n";

    if (!is_leaf) {
        for (int i = 0; i < size + 1; ++i) {
            children[i]->print (id + std::to_string (i));
        }
    }
}

template<typename T, const int min_degree, class compare>
bool B_tree<T, min_degree, compare>::erase (const T& value) {
    if (root == nullptr) {
        return false;
    }

    bool was_in = root->remove (value);

    if (root->size == 0) {
        B_node* tmp = root;
        root = (root->is_leaf) ? nullptr : root->children[0];

        delete [] tmp->children;
        delete [] tmp->keys;
    }

    debug ("remove\n")
    return was_in;
}

template<typename T, const int min_degree, class compare>
bool B_tree<T, min_degree, compare>::B_node::remove (const T& value) {
    --tree_size;
    compare less;

    int idx = find_not_less_index (value);

    if (idx < size and !less (keys[idx], value) and !less (value, keys[idx])) {
        if (is_leaf) {
            remove_leaf (idx);
        }
        else {
            remove_non_leaf (idx);
        }
    }
    else {
        if (is_leaf) {
            debug ("remove in node\n")
            return false;
        }

        bool last_merged = idx == size;
        if (children[idx]->size < min_degree) {
            fill_thin (idx);
            //last_merged = fill_thin (idx);
        }

        if (last_merged and idx > size) {
            children[idx - 1]->remove (value);
        }
        else {
            children[idx]->remove (value);
        }

    }
    debug ("remove in node\n")
    return true;
}

template<typename T, const int min_degree, class compare>
int B_tree<T, min_degree, compare>::B_node::find_not_less_index (const T& value) {
    compare less;
    int idx = 0;
    while (idx < size and less (keys[idx], value)) {
        ++idx;
    }
    return idx;
}

template<typename T, const int min_degree, class compare>
void B_tree<T, min_degree, compare>::B_node::remove_leaf (int idx) {
    for (int i = idx + 1; i < size; ++i) {
        keys[i - 1] = keys[i];
    }
    --size;
    //--tree_size;
    debug ("remove_leaf\n")
}

template<typename T, const int min_degree, class compare>
void B_tree<T, min_degree, compare>::B_node::remove_non_leaf (int idx) {
    //--tree_size;
    T value = keys[idx];
    if (children[idx]->size >= min_degree) {
        T pred = get_pred (idx);
        keys[idx]= pred;
        children[idx]->remove (pred);
    }
    else if (children[idx + 1]->size >= min_degree) {
        T succ = get_succ (idx);
        keys[idx] = succ;
        children[idx + 1]->remove (succ);
    }
    else {
        merge (idx);
        children[idx]->remove (value);
    }
    debug ("remove_not_leaf\n")
}

template<typename T, const int min_degree, class compare>
bool B_tree<T, min_degree, compare>::B_node::fill_thin (int idx) {
    if (idx != 0 and children[idx - 1]->size >= min_degree) {
        borrow_prev (idx);
    }
    else if (idx != size and children[idx + 1]->size >= min_degree) {
        borrow_next (idx);
    }
    else {
        if (idx != size) {
            merge (idx);
        }
        else {
            merge (idx - 1);
            return true;
        }
    }
    return false;
}

template<typename T, const int min_degree, class compare>
void B_tree<T, min_degree, compare>::B_node::merge (int idx) {
    B_node* child = children[idx];
    B_node* sibling = children[idx + 1];
    child->tree_size += sibling->tree_size + 1;

    child->keys[min_degree - 1] = keys[idx];

    for (int i = 0; i < sibling->size; ++i) {
        child->keys[min_degree + i] = sibling->keys[i];
    }

    if (!child->is_leaf) {
        for (int i = 0; i <= sibling->size; ++i) {
            child->children[min_degree + i] = sibling->children[i];
        }
    }

    for (int i = idx + 1; i < size; ++i) {
        keys[i - 1] = keys[i];
    }
    for (int i = idx + 2; i <= size; ++i) {
        children[i - 1] = children[i];
    }

    child->size += sibling->size + 1;
    --size;

    delete [] sibling->keys;
    delete [] sibling->children;
    debug ("merge\n")
}

template<typename T, const int min_degree, class compare>
const T& B_tree<T, min_degree, compare>::B_node::get_pred (int idx) {
    B_node* cur = children[idx];
    while (!cur->is_leaf) {
        cur = cur->children[cur->size];
    }

    return cur->keys[cur->size - 1];
}

template<typename T, const int min_degree, class compare>
const T& B_tree<T, min_degree, compare>::B_node::get_succ (int idx) {
    B_node* cur = children[idx + 1];
    while (!cur->is_leaf) {
        cur = cur->children[0];
    }

    // Return the first key of the leaf
    return cur->keys[0];
}

template<typename T, const int min_degree, class compare>
void B_tree<T, min_degree, compare>::B_node::borrow_prev (int idx) {
    B_node* child = children[idx];
    B_node* sibling = children[idx - 1];

    for (int i = child->size - 1; i >= 0; --i) {
        child->keys[i + 1] = child->keys[i];
    }

    if (!child->is_leaf) {
        for (int i = child->size; i >= 0; --i) {
            child->children[i + 1] = child->children[i];
        }
    }

    child->keys[0] = keys[idx - 1];

    if (!child->is_leaf) {
        child->tree_size += sibling->children[sibling->size]->tree_size;
        sibling->tree_size -= sibling->children[sibling->size]->tree_size;
        child->children[0] = sibling->children[sibling->size];
    }

    keys[idx - 1] = sibling->keys[sibling->size - 1];

    child->size++;
    child->tree_size++;
    sibling->size--;
    sibling->tree_size--;
    debug ("borrow_prev\n")
}

template<typename T, const int min_degree, class compare>
void B_tree<T, min_degree, compare>::B_node::borrow_next (int idx) {
    B_node* child = children[idx];
    B_node* sibling = children[idx + 1];

    child->keys[child->size] = keys[idx];

    if (!child->is_leaf) {
        child->tree_size += sibling->children[0]->tree_size;
        sibling->tree_size -= sibling->children[0]->tree_size;
        child->children[child->size + 1] = sibling->children[0];
    }

    keys[idx] = sibling->keys[0];

    for (int i = 1; i < sibling->size; ++i) {
        sibling->keys[i - 1] = sibling->keys[i];
    }

    if (!sibling->is_leaf) {
        for (int i = 1; i <= sibling->size; ++i) {
            sibling->children[i - 1] = sibling->children[i];
        }
    }

    child->size++;
    child->tree_size++;
    sibling->size--;
    sibling->tree_size--;

    debug ("borrow_next\n")
}

template<typename T, const int min_degree, class compare>
T& B_tree<T, min_degree, compare>::kth_maximum (int k) {
    return root->kth_max (k);
}

template<typename T, const int min_degree, class compare>
T& B_tree<T, min_degree, compare>::B_node::kth_max (int k) {
    if (is_leaf) {
        return keys[size - k];
    }
    else {
        int idx = size;
        while (children[idx]->tree_size < k) {
            k -= children[idx]->tree_size + 1;
            --idx;
        }
        if (k == 0) {
            return keys[idx];
        }
        return children[idx]->kth_max (k);
    }
}

#undef debug
#endif
