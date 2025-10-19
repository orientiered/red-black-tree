#pragma once

#include <functional>
#include <ostream>
#include <stack>
#include <cassert>

#include "gtest/gtest.h"
#include "gtest/gtest_prod.h"

namespace RBTree {

enum class Order {
    less,
    equal,
    greater
};

template<typename T, typename CompT>
Order compare(const T &lhs, const T &rhs) {
    if (CompT{}(lhs, rhs))
        return Order::less;
    else if (CompT{}(rhs, lhs))
        return Order::greater;

    return Order::equal;
}

template <typename T, typename CompT = std::less<T>>
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
        Node(const T& key, Node * parent, Node * left, Node * right, Color color = Color::black) : key_(key), parent_(parent), left_(left), right_(right), color_(color) {}

        Node(const Node& rhs) = delete;
        Node &operator=(const Node& rhs) = delete;

        bool is_nil() const { return this == parent_; }

        void make_nil() {
            parent_ = this;
            right_  = this;
            left_   = this;
            color_ = Color::black;
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

    void insert_fixup(Node *node);

    void print_dot_debug_recursive(std::ostream &stream, const Node *node) const;

public:
    struct iterator {
        const Node *ptr_;
        iterator(const Node *node): ptr_(node) {}

        const T& operator*() const { return ptr_->key_; }

        iterator& operator++() {
            assert(!ptr_->is_nil());

            if (!ptr_->right_->is_nil()) {
                ptr_ = ptr_->right_;
                while (!ptr_->left_->is_nil()) {
                    ptr_ = ptr_->left_;
                }
                return *this;
            } else {
                // const Node *parent = ptr_->parent_;
                while (ptr_->parent_->right_ == ptr_) {
                    ptr_ = ptr_->parent_;
                }
                ptr_ = ptr_->parent_;

                return *this;

            }
        }

        bool is_end() const {
            return ptr_->is_nil();
        }

        bool operator==(const iterator& rhs) {return ptr_ == rhs.ptr_; }
    };

    Tree() {
        tree_nil_ = new Node(T());
        tree_nil_->make_nil();
        root_ = tree_nil_;
    }
    ~Tree() {
        std::stack<Node *> stack;
        Node * node = root_;

        while (true) {
            while (node != tree_nil_) {
                stack.push(node);
                node = node->left_;
            }

            if (stack.empty())
                break;

            Node * prev_node = stack.top();
            stack.pop();

            node = prev_node->right_;
            delete prev_node;
        }

        delete tree_nil_;
    }

    Tree(const Tree& rhs) = delete;
    Tree &operator=(const Tree& rhs) = delete;

    Tree(Tree&& rhs): tree_nil_(rhs.tree_nil_), root_(rhs.root_) {
        rhs.tree_nil_ = new Node(T());
        rhs.tree_nil_->make_nil();
        rhs.root_ = rhs.tree_nil_;
    }
    FRIEND_TEST(Move, Ctor);

    Tree &operator=(Tree&& rhs) {
        if (&rhs == this)
            return *this;

        std::swap(rhs.root_, root_);
        std::swap(rhs.tree_nil_, tree_nil_);

        return *this;
    }
    FRIEND_TEST(Move, Assign);


    void insert(const T& key);

    iterator get_root() const {
        return iterator(root_);
    }

    iterator lower_bound(const T& key) const;
    iterator upper_bound(const T& key) const;

    int distance(iterator fst, iterator snd) const {
        int dst = 0;
        while (fst != snd) {
            dst++;
            ++fst;
        }

        return dst;
    }

    /* DEBUG FUNCTIONS HERE */
    void print_sorted(std::ostream &stream, const iterator it) const;

    void print_debug(std::ostream &stream, const iterator it, const unsigned indent = 0) const;

    void print_dot_debug(std::ostream &stream, const iterator it) const;

    FRIEND_TEST(RotateTest, LeftRotate);
    FRIEND_TEST(RotateTest, RightRotate);
};

template <typename T, typename CompT>
void Tree<T, CompT>::insert(const T& key) {
    Node *prev = tree_nil_;
    Node *cur = root_;

    while (cur != tree_nil_) {
        prev = cur;
        switch(compare<T, CompT>(key, cur->key_)) {
        case Order::less:
            cur = cur->left_;
            break;
        case Order::greater:
            cur = cur->right_;
            break;
        case Order::equal: default:
            // equals so do not insert
            return;
        }
    }

    // TODO: reduce number of comps
    Node *new_node = new Node(key, prev, Color::red);
    new_node->left_ = new_node->right_ = tree_nil_;

    // empty tree
    if (prev == tree_nil_) {
        root_ = new_node;
    // othwerise put new node to the correct subtree
    } else if (compare<T, CompT>(new_node->key_, prev->key_) == Order::less) {
        prev->left_ = new_node;
    } else {
        prev->right_ = new_node;
    }

    insert_fixup(new_node);
}

template <typename T, typename CompT>
void Tree<T, CompT>::insert_fixup(Node *node) {
    while (node->parent_->color_ == Color::red)
    {
        if (node->parent_ == node->parent_->parent_->left_) {
            Node *y = node->parent_->parent_->right_;
            if (y->color_ == Color::red) {
                node->parent_->color_ = Color::black;
                y->color_ = Color::black;
                node->parent_->parent_->color_ = Color::red;
                node = node->parent_->parent_;
            } else {
                if (node == node->parent_->right_) {
                    node = node->parent_;
                    left_rotate(node);
                }
                node->parent_->color_ = Color::black;
                node->parent_->parent_->color_ = Color::red;
                right_rotate(node->parent_->parent_);
            }
        }
        else {
            Node *y = node->parent_->parent_->left_;
            if (y->color_ == Color::red) {
                node->parent_->color_ = Color::black;
                y->color_ = Color::black;
                node->parent_->parent_->color_ = Color::red;
                node = node->parent_->parent_;
            } else {
                if (node == node->parent_->left_) {
                    node = node->parent_;
                    right_rotate(node);
                }
                node->parent_->color_ = Color::black;
                node->parent_->parent_->color_ = Color::red;
                left_rotate(node->parent_->parent_);
            }
        }
    }

    root_->color_ = Color::black;
}

template <typename T, typename CompT>
void Tree<T, CompT>::left_rotate(Node *node) {
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
        y->left_->parent_ = x;
    }

    // y parent
    y->parent_ = x->parent_;

    // x was tree root
    if (x->parent_ == tree_nil_) {
        root_ = y;
    // x was left subtree
    } else if (x == x->parent_->left_) {
        x->parent_->left_ = y;
    // x was right subtree
    } else {
        x->parent_->right_ = y;
    }

    y->left_ = x;
    x->parent_ = y;
}

template <typename T, typename CompT>
void Tree<T, CompT>::right_rotate(Node *node) {
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
        y->right_->parent_ = x;
    }

    // y parent
    y->parent_ = x->parent_;

    // x was tree root
    if (x->parent_ == tree_nil_) {
        root_ = y;
    // x was left subtree
    } else if (x == x->parent_->left_) {
        x->parent_->left_ = y;
    // x was right subtree
    } else {
        x->parent_->right_ = y;
    }

    y->right_ = x;
    x->parent_ = y;
}

/// Returns an iterator pointing to the first element that is not less than key.
template<typename T, typename CompT>
Tree<T, CompT>::iterator Tree<T, CompT>::lower_bound(const T& key) const {
    Node *node = root_;
    Node *last_closest = tree_nil_;

    while (node != tree_nil_) {
        switch(compare<T, CompT>(key, node->key_)) {
        case Order::less:
            last_closest = node;
            node = node->left_;
            break;
        case Order::greater:
            node = node->right_;
            break;
        case Order::equal: default:
            return node;
        }
    }
    return iterator(last_closest);
}

/// Returns an iterator pointing to the first element that is greater than key.
template<typename T, typename CompT>
Tree<T, CompT>::iterator Tree<T, CompT>::upper_bound(const T& key) const {
    Node *node = root_;
    Node *last_closest = tree_nil_;

    while (node != tree_nil_) {
        switch(compare<T, CompT>(key, node->key_)) {
        case Order::less:
            last_closest = node;
            node = node->left_;
            break;
        case Order::greater:
        case Order::equal: default:
            node = node->right_;
            break;
        }
    }
    return iterator(last_closest);
}


} // namespace RBTree
