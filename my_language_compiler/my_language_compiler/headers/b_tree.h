#ifndef B_TREE_H
#define B_TREE_H

#include <iostream>
#include <string>

// Basic b-tree class
template<typename T, const int min_degree, class compare = std::less<T> >
class B_tree {
private:
    struct B_node {
        int size;                                   // number of keys
        T* keys;
        B_node** children;
        bool is_leaf;
        int tree_size;

        B_node ();
        //! frees memory of a vertice and its children
        void destroy_subtree ();
        //! Inserts value to a tree if it is not full. The node corresponding to this index must be full when this function is called.
        void insert (const T& value);
        //! Splits index_node of this node. The node corresponding to this index must be full when this function is called.
        void split_child (int index);
        //! If the given node is full
        bool is_full ();
        //! Checks if a given value is in node
        bool is_in (const T& value);
        //! Gets next element (recursive for "next" in tree)
        std::pair<bool, T> next (const T& value);
        //! Prints node in print of tree function
        void print (const std::string& identifier);
        //! removes value from a tree
        bool remove (const T& value);
        //! finds index of the first element in a node that is equal or greater than a given one
        int find_not_less_index (const T& value);
        //! removes value from a leaf node
        void remove_leaf (int idx);
        //! removes value from a non-leaf node
        void remove_non_leaf (int idx);
        //! if a child has too little keys - returns if the last child was merged to the pre-last
        bool fill_thin (int idx);
        //! merges two subtrees (idx and idx + 1)
        void merge (int idx);
        //! gets predecessor of keys[idx]
        const T& get_pred (int idx);
        //! gets successor of keys[idx]
        const T& get_succ (int idx);
        //! borrows key from children[idx - 1] and inserts it to children[idx]
        void borrow_prev (int idx);
        //! borrows key from children[idx + 1] and inserts it to children[idx]
        void borrow_next (int idx);
        //! returns k-th maximum in a tree
        T& kth_max (int k);
    };
    B_node* root;
public:
    explicit B_tree ();
    ~B_tree ();
    
    B_tree (const B_tree&)              = delete;
    B_tree (B_tree&&)                   = delete;
    B_tree& operator= (const B_tree&)   = delete;
    B_tree& operator= (B_tree&&)        = delete;

    //!==========================|
    //! Inserts element to a tree
    //!==========================|
    void insert (const T& value);

    //!==========================|
    //! Prints tree
    //!==========================|
    friend std::ostream& operator<< (std::ostream& out, const B_tree& c);

    //!=====================================================================|
    //! Deletes value from tree (if it was not in a tree, then does nothing)
    //! Returns if the value actually was in a tree
    //!=====================================================================|
    bool erase (const T& value);

    //!=====================================================================|
    //! Returns element which is k-th maximum in our tree
    //!=====================================================================|
    T& kth_maximum (int k);
};

#endif
