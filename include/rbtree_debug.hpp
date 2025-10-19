#pragma once

#include <string>

#include "rbtree.hpp"

namespace RBTree {

template <typename T, typename CompT>
void Tree<T, CompT>::print_sorted(std::ostream &stream, const iterator it) const {
    const Node *node = it.ptr_;
    if (node == tree_nil_)
        return;

    print_sorted(stream,  node->left_);
    stream << node->key_ << " ";
    print_sorted(stream, node->right_);
}

template <typename T, typename CompT>
void Tree<T, CompT>::print_debug(std::ostream &stream, const iterator it, const unsigned indent) const {
    const Node *node = it.ptr_;
    if (node == tree_nil_)
        return;

    std::string indent_str(indent, '\t');

    stream << indent_str << "Node " << node << ":\n";
    stream << indent_str << "color = " << ((node->color_ == Color::black) ? "black" : "red") << "\n";
    stream << indent_str << "key = " << node->key_ << "\n";
    stream << indent_str << "parent " << node->parent_ << "\n";
    stream << indent_str << "left   " << node->left_   << "\n";
    stream << indent_str << "right  " << node->right_  << "\n";
    print_debug(stream, node->left_, indent + 1);
    print_debug(stream, node->right_, indent + 1);
}

template <typename T, typename CompT>
void Tree<T, CompT>::print_dot_debug_recursive(std::ostream &stream, const Node *node) const {
    if (node == tree_nil_)
        return;

    std::string fillcolor = (node->color_ == Color::black) ? "#D5D5D5FF": "#F54927";
    stream << "\t" << "node" << node << "[shape = Mrecord, label = \"{"
            << "node[" << node << "] | parent[" << node->parent_ << "] | "
            << node->key_ << " | "
            << "{<left> L | <right> R}}\""
            << ", style = filled, fillcolor = \"" << fillcolor << "\"];\n";

    if (node->left_ != tree_nil_)
        stream << "\tnode" << node << ":<left> -> node" << node->left_ << ";\n";
    if (node->right_ != tree_nil_)
        stream << "\tnode" << node << ":<right> -> node" << node->right_ << ";\n";

    print_dot_debug_recursive(stream, node->left_);
    print_dot_debug_recursive(stream, node->right_);
}

template <typename T, typename CompT>
void Tree<T, CompT>::print_dot_debug(std::ostream &stream, const iterator it) const {
    stream << "digraph {\n"
              "graph [splines=line]\n";
    print_dot_debug_recursive(stream, it.ptr_);
    stream << "}\n";
}

} // namespace RBTree
