#include "gtest/gtest.h"

#include "rbtree.hpp"

namespace RBTree {

using TreeInt = Tree<int>;

#define NIL tree.tree_nil_

/*************** ROTATE ****************/
TEST(RotateTest, LeftRotate) {
    using Node = TreeInt::Node;
    TreeInt tree;

    auto old_root = tree.root_ = new Node(2, NIL);
    auto old_left  = tree.root_->left_  = new Node(1, tree.root_, NIL, NIL);
    auto old_right = tree.root_->right_ = new Node(3, tree.root_, NIL, NIL);

    tree.left_rotate(tree.root_);
    /*
            2               3
          1   3    ->     2
                        1
    */
    EXPECT_EQ(tree.root_, old_right);
        EXPECT_EQ(tree.root_->parent_, tree.tree_nil_);

        EXPECT_EQ(tree.root_->left_, old_root);
            EXPECT_EQ(tree.root_->left_->parent_, tree.root_);

            EXPECT_EQ(tree.root_->left_->left_, old_left);
                EXPECT_EQ(tree.root_->left_->left_->parent_, old_root);
                EXPECT_EQ(tree.root_->left_->left_->left_, tree.tree_nil_);
                EXPECT_EQ(tree.root_->left_->left_->right_, tree.tree_nil_);

            EXPECT_EQ(tree.root_->left_->right_, tree.tree_nil_);

        EXPECT_EQ(tree.root_->right_, tree.tree_nil_);
}

TEST(RotateTest, RightRotate) {
    using Node = TreeInt::Node;
    TreeInt tree;

    auto old_root = tree.root_ = new Node(2, NIL);
    auto old_left  = tree.root_->left_  = new Node(1, tree.root_, NIL, NIL);
    auto old_right = tree.root_->right_ = new Node(3, tree.root_, NIL, NIL);

    tree.right_rotate(tree.root_);
    /*
            2               1
          1   3    ->         2
                                3

    */
    EXPECT_EQ(tree.root_, old_left);
        EXPECT_EQ(tree.root_->parent_, tree.tree_nil_);

        EXPECT_EQ(tree.root_->left_, tree.tree_nil_);

        EXPECT_EQ(tree.root_->right_, old_root);
            EXPECT_EQ(tree.root_->right_->parent_, tree.root_);

            EXPECT_EQ(tree.root_->right_->left_, tree.tree_nil_);

            EXPECT_EQ(tree.root_->right_->right_, old_right);
                EXPECT_EQ(tree.root_->right_->right_->parent_, old_root);
                EXPECT_EQ(tree.root_->right_->right_->left_, tree.tree_nil_);
                EXPECT_EQ(tree.root_->right_->right_->right_, tree.tree_nil_);
}

/************** ITERATORS **************/
TEST(TreeItTest, Op_successor) {
    TreeInt tree;
    tree.insert(2);
    Tree<int>::iterator it2 = tree.get_root();
    tree.insert(1);
    tree.insert(3);
    tree.insert(4);
    tree.insert(5);

    EXPECT_EQ(*it2, 2);
    ++it2;
    EXPECT_EQ(*it2, 3);
    ++it2;
    EXPECT_EQ(*it2, 4);
    ++it2;
    EXPECT_EQ(*it2, 5);
    ++it2;
    EXPECT_TRUE(it2.is_end());
}

}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
