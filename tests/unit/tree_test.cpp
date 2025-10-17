#include "rbtree.hpp"
#include "gtest/gtest.h"

using namespace RBTree;

TEST(TreeItTest, Op_successor) {
    Tree<int> tree;
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


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
