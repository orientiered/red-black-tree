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
        T key_;

        Node *parent_ = nullptr;
        Node *left_   = nullptr;
        Node *right_  = nullptr;

        Color color_ = Color::black;

        Node(const T& key, Color color = Color::black) : key_(key), color_(color) {}
        Node(const T& key, Node * parent, Color color = Color::black) : key_(key), parent_(parent), color_(color) {}

        Node(const Node& rhs) = delete;
        Node &operator=(const Node& rhs) = delete;

        ~Node() {
            //TODO: find a better solution
            if (left_ != this)
                delete left_;
            if (right_ != this)
                delete right_;
        }

        void make_nil() {
            parent_ = this;
            right_  = this;
            left_   = this;
            color_ = Color::black;
        }

        /* DEBUG FUNCTIONS HERE */
        void print_debug(std::ostream stream, int indent) const {
            std::string indent_str;
            for (int i = 0; i < indent; i++)
                indent_str += "\t";

            stream << indent_str << "Node " << this << ":\n";
            stream << indent_str << "\t" << "color = " << ((color_ == Color::black) ? "black" : "red") << "\n";
            stream << indent_str << "\t" << "key = " << key_ << "\n";
            stream << indent_str << "\t" << "parent " << parent_ << "\n";
            stream << indent_str << "\t" << "left   " << left_   << "\n";
            stream << indent_str << "\t" << "right  " << right_  << "\n";
        }
    };

    Node *root_ = nullptr;
    Node *tree_nil_ = nullptr;

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
        while (node->parent_->color == Color::red)
        {
            if (node->parent_ == node->parent_->parent->left)
            {
                Node *y = node->parent_->parent->right;
                if (y->color_ == Color::red) {
                    node->parent_->color = Color::black;
                    y->color_ = Color::black;
                    node->parent_->parent->color = Color::red;
                    node = node->parent_->parent;
                } else {
                    if (node == node->parent_->right) {
                        node = node->parent_;
                        left_rotate(node);
                    }
                    node->parent_->color = Color::black;
                    node->parent_->parent->color = Color::red;
                    right_rotate(node->parent_->parent);
                }
            } else {
                Node *y = node->parent_->parent->left;
                if (y->color_ == Color::red) {
                    node->parent_->color = Color::black;
                    y->color_ = Color::black;
                    node->parent_->parent->color = Color::red;
                    node = node->parent_->parent;
                } else {
                    if (node == node->parent_->left) {
                        node = node->parent_;
                        right_rotate(node);
                    }
                    node->parent_->color = Color::black;
                    node->parent_->parent->color = Color::red;
                    left_rotate(node->parent_->parent);
                }
            }
        }

        root_->color_ = Color::BLACK;
    }
public:
    using iterator = Node *;

    Tree() {
        tree_nil_ = new Node(T());
        tree_nil_->make_nil();
    }
    ~Tree() {
        delete root_;
        delete tree_nil_;
    }
    Tree(const Tree& rhs) = delete;
    Tree &operator=(const Tree& rhs) = delete;


    void insert(const T& key) {
        Node *new_node = new Node(key, Color::red);

        Node *prev = tree_nil_;
        Node *cur = root_;

        while (cur != tree_nil_) {
            prev = cur;
            if (new_node->key_ < cur->key_) {
                cur = cur->left_;
            } else {
                cur = cur->right_;
            }
        }

        // cur is leaf, prev is its parent
        new_node->parent_ = prev;
        // empty tree
        if (prev == tree_nil_) {
            root_ = new_node;
        // othwerise put new node to the correct subtree
        } else if (new_node->key_ < prev->key_) {
            prev->left_ = new_node;
        } else {
            prev->right_ = new_node;
        }

        insert_fixup(new_node);
    }

    iterator get_root() const {
        return root_;
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

    print_sorted(stream,  node->left_);
    stream << node << " ";
    print_sorted(stream, node->right_);
}

template <typename T>
void Tree<T>::print_debug(std::ostream stream, iterator node, int indent) const {
    node->print_debug(stream, indent);
    print_debug(stream, node->left_, indent + 1);
    print_debug(stream, node->right_, indent + 1);
}

template <typename T>
void Tree<T>::left_rotate(Node *node) {
    /*
          x                y
        a   y    -->     x   c
      b   c            a   b
    */
    assert(node);
    if (node->right_ == tree_nil_) return;

    Node *&x = node;

    Node *y = x->right_;
    // (b) left y subtree -> right x subtree
    x->right_ = y->left_;

    // b parent
    if (y->left_ != tree_nil_) {
        y->left_->parent = x;
    }

    // y parent
    y->parent_ = x->parent_;

    // x was tree root
    if (x->parent_ == tree_nil_) {
        root_ = y;
    // x was left subtree
    } else if (x == x->parent_->left) {
        x->parent_->left = y;
    // x was right subtree
    } else {
        x->parent_->right = y;
    }

    y->left_ = x;
    x->parent_ = y;
}

template <typename T>
void Tree<T>::right_rotate(Node *node) {
    /*
        x           y
      y   c  -->  a   x
    a   b           b   c
    */
    assert(node);
    if (node->left_ == tree_nil_) return;

    Node *&x = node;

    Node *y = x->left_;
    // (b) right y subtree -> left x subtree
    x->left_ = y->right_;

    // b parent
    if (y->right_ != tree_nil_) {
        y->right_->parent = x;
    }

    // y parent
    y->parent_ = x->parent_;

    // x was tree root
    if (x->parent_ == tree_nil_) {
        root_ = y;
    // x was left subtree
    } else if (x == x->parent_->left) {
        x->parent_->left = y;
    // x was right subtree
    } else {
        x->parent_->right = y;
    }

    y->right_ = x;
    x->parent_ = y;
}

}; // namespace RBTree
