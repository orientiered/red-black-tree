#pragma once

#include <ostream>
#include <string>

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

        ~Node() {
            //TODO: find a better solution
            delete left;
            delete right;
        }

        Node *root = nullptr;

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

public:
    using iterator = Node *;

    Tree() {}
    ~Tree() {
        delete root;
    }

    void insert(const T& key);

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

}; // namespace RBTree

