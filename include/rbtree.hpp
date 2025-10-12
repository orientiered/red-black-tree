#pragma once

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
    };

    Node *root = nullptr;
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
};

}; // namespace RBTree
