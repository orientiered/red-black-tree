#pragma once

#include <ostream>
#include <string>
#include <cassert>

namespace RBTree {

template <typename T>
class Tree {
    enum class Color {
        black,
        red,
    };

    struct Node {
        T key;

        Node *parent = nullptr;
        Node *left   = nullptr;
        Node *right  = nullptr;

        Color color = Color::black;

        Node(const T& key, Color color = Color::black) : key(key), color(color) {}
        Node(const T& key, Node * parent, Color color = Color::black) : key(key), parent(parent), color(color) {}

        void make_nil() {
            parent = this;
            right  = this;
            left   = this;
            color = Color::black;
        }

        ~Node() {
            //TODO: find a better solution
            if (left != this)
                delete left;
            if (right != this)
                delete right;
        }

        /* DEBUG FUNCTIONS HERE */
        void print_debug(std::ostream stream, int indent) const {
            std::string indent_str;
            for (int i = 0; i < indent; i++)
                indent_str += "\t";

            stream << indent_str << "Node " << this << ":\n";
            stream << indent_str << "\t" << "color = " << ((color == Color::black) ? "black" : "red") << "\n";
            stream << indent_str << "\t" << "key = " << key << "\n";
            stream << indent_str << "\t" << "parent " << parent << "\n";
            stream << indent_str << "\t" << "left   " << left   << "\n";
            stream << indent_str << "\t" << "right  " << right  << "\n";
        }
    };

    Node *root = nullptr;
    Node *tree_nil = nullptr;

    /*
          x               y
        a   y    -->    x   c
      b   c           a   b
    */
    void left_rotate(Node *node);

    /*
          x           y
        y   c  -->  a   x
      a   b           b   c
    */
    void right_rotate(Node *node);

    void insert_fixup(Node *node) {
        while (node->parent->color == Color::red)
        {
            if (node->parent == node->parent->parent->left)
            {
                Node *y = node->parent->parent->right;
                if (y->color == Color::red) {
                    node->parent->color = Color::black;
                    y->color = Color::black;
                    node->parent->parent->color = Color::red;
                    node = node->parent->parent;
                } else {
                    if (node == node->parent->right) {
                        node = node->parent;
                        left_rotate(node);
                    }
                    node->parent->color = Color::black;
                    node->parent->parent->color = Color::red;
                    right_rotate(node->parent->parent);
                }
            } else {
                Node *y = node->parent->parent->left;
                if (y->color == Color::red) {
                    node->parent->color = Color::black;
                    y->color = Color::black;
                    node->parent->parent->color = Color::red;
                    node = node->parent->parent;
                } else {
                    if (node == node->parent->left) {
                        node = node->parent;
                        right_rotate(node);
                    }
                    node->parent->color = Color::black;
                    node->parent->parent->color = Color::red;
                    left_rotate(node->parent->parent);
                }
            }
        }

        root->color = Color::BLACK;
    }
public:
    using iterator = Node *;

    Tree() {
        tree_nil = new Node(T());
        tree_nil->make_nil();
    }
    ~Tree() {
        delete root;
        delete tree_nil;
    }

    void insert(const T& key) {
        Node *new_node = new Node(key, Color::red);

        Node *prev = tree_nil;
        Node *cur = root;

        while (cur != tree_nil) {
            prev = cur;
            if (new_node->key < cur->key) {
                cur = cur->left;
            } else {
                cur = cur->right;
            }
        }

        // cur is leaf, prev is it's parent
        new_node->parent = prev;
        // empty tree
        if (prev == tree_nil) {
            root = new_node;
        // othwerise putting new node to the correct subtree
        } else if (new_node->key < prev->key) {
            prev->left = new_node;
        } else {
            prev->right = new_node;
        }

        insert_fixup(new_node);
    }

    iterator get_root() const {
        return root;
    }

    iterator lower_bound(const T& key) const;
    iterator upper_bound(const T& key) const;

    int distance(iterator fst, iterator snd) const;

    /* DEBUG FUNCTIONS HERE */
    void print_sorted(std::ostream stream, iterator node) const;

    void print_debug(std::ostream stream, iterator node, int indent = 0) const;
};

template <typename T>
void Tree<T>::print_sorted(std::ostream stream, iterator node) const {
    if (node == nullptr)
        return;

    print_sorted(stream,  node->left);
    stream << node << " ";
    print_sorted(stream, node->right);
}

template <typename T>
void Tree<T>::print_debug(std::ostream stream, iterator node, int indent) const {
    node->print_debug(stream, indent);
    print_debug(stream, node->left, indent + 1);
    print_debug(stream, node->right, indent + 1);
}

template <typename T>
void Tree<T>::left_rotate(Node *node) {
    /*
          x                y
        a   y    -->     x   c
      b   c            a   b
    */
    assert(node);
    if (node->right == tree_nil) return;

    Node *&x = node;

    Node *y = x->right;
    // (b) left y subtree -> right x subtree
    x->right = y->left;

    // b parent
    if (y->left != tree_nil) {
        y->left->parent = x;
    }

    // y parent
    y->parent = x->parent;

    // x was tree root
    if (x->parent == tree_nil) {
        root = y;
    // x was left subtree
    } else if (x == x->parent->left) {
        x->parent->left = y;
    // x was right subtree
    } else {
        x->parent->right = y;
    }

    y->left = x;
    x->parent = y;
}

template <typename T>
void Tree<T>::right_rotate(Node *node) {
    /*
        x           y
      y   c  -->  a   x
    a   b           b   c
    */
    assert(node);
    if (node->left == tree_nil) return;

    Node *&x = node;

    Node *y = x->left;
    // (b) right y subtree -> left x subtree
    x->left = y->right;

    // b parent
    if (y->right != tree_nil) {
        y->right->parent = x;
    }

    // y parent
    y->parent = x->parent;

    // x was tree root
    if (x->parent == tree_nil) {
        root = y;
    // x was left subtree
    } else if (x == x->parent->left) {
        x->parent->left = y;
    // x was right subtree
    } else {
        x->parent->right = y;
    }

    y->right = x;
    x->parent = y;
}

}; // namespace RBTree
