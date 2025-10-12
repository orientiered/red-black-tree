#include <iostream>

#include "rbtree.hpp"

int main() {
    RBTree::Tree<int> tree;

    std::string input = "";
    while (std::cin >> input) {
        if (input == "k") {
            int key = 0;
            std::cin >> key;

            std::cout << "Add key " << key << "\n";
            tree.insert(key);
        } else if (input == "q") {
            int fst = 0, snd = 0;
            std::cin >> fst >> snd;

            std::cout << "Range query " << fst << " " << snd << "\n";
        }
    }

    tree.print_debug(std::cerr, tree.get_root());
    return 0;
}
