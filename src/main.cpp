#include <iostream>

#include "rbtree.hpp"

int main(int argc, const char *argv[]) {
    bool verbose = false;

    for (int arg_index = 1; arg_index < argc; arg_index++) {
        std::string arg(argv[arg_index]);

        if (arg == "-v")
            verbose = true;
    }

    RBTree::Tree<int> tree;

    std::string input = "";
    while (std::cin >> input) {
        if (input == "k") {
            int key = 0;
            std::cin >> key;
            tree.insert(key);
            if (verbose) std::cout << "Add key " << key << "\n";
        } else if (input == "q") {
            int fst = 0, snd = 0;
            std::cin >> fst >> snd;
            if (verbose) std::cout << "Range query " << fst << " " << snd << "\n";
        } else if (input == "l") {
            int left = 0;
            std::cin >> left;
            if (verbose) std::cout << "Lower bound " << tree.lower_bound(left) << "\n";
        } else if (input == "u") {
            int right = 0;
            std::cin >> right;
            if (verbose) std::cout << "Upper bound " << tree.upper_bound(right) << "\n";
        }
    }

    tree.print_sorted(std::cout, tree.get_root());
    std::cout << "\n";

    if (verbose)
        tree.print_debug(std::cout, tree.get_root());

    return 0;
}
