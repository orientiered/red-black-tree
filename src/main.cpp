#include <fstream>
#include <iostream>

#include "rbtree.hpp"

void check_cin(const std::string err_msg);

void check_cin(const std::string err_msg) {
    if (!std::cin.good()) {
        std::cerr << err_msg;
        exit(EXIT_FAILURE);
    }
    return;
}

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
        check_cin("Failed to read input\n");

        if (input == "k") {
            int key = 0;
            std::cin >> key;
            check_cin("Failed to read key\n");
            tree.insert(key);

            if (verbose) std::cout << "Add key " << key << "\n";
        }
        else if (input == "q") {
            int fst = 0, snd = 0;
            std::cin >> fst;
            check_cin("Failed to read first key\n");

            std::cin >> snd;
            check_cin("Failed to read second key\n");

            if (verbose) std::cout << "Range query " << fst << " " << snd << "\n";

            int distance = 0;
            if (snd > fst) distance = tree.distance(tree.upper_bound(fst), tree.upper_bound(snd));

            std::cout << distance << " ";
        }
        else {
            std::cerr << "Unexpected keyword:" << input << "\n";
            exit(EXIT_FAILURE);
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
