#include <fstream>
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
            std::cout << tree.distance(tree.lower_bound(fst), tree.upper_bound(snd)) << " ";
        } else if (input == "l") {
            int left = 0;
            std::cin >> left;
            if (verbose) std::cout << "Lower bound " << *tree.lower_bound(left) << "\n";
        } else if (input == "u") {
            int right = 0;
            std::cin >> right;
            RBTree::Tree<int>::iterator upper = tree.upper_bound(right);
            if (verbose) std::cout << "Upper bound " << *upper << "\n";
        }
    }
    std::cout << "\n";

    if (verbose) {
        tree.print_debug(std::cout, tree.get_root());
        std::string file_name = "graph.dot";
        std::fstream file(file_name, std::ios::out | std::ios::trunc);
        if (!file.good()) {
            std::cout << "Failed to open dot file\n";
        }
        tree.print_dot_debug(file, tree.get_root());
        file.close();
        std::system(("dot " + file_name + " -Tpng -o graph.png").c_str());
    }

    return 0;
}
