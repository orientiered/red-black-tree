#include <iostream>
#include <iterator>
#include <set>

int main() {
    std::set<int> set;

    std::string input = "";
    while (std::cin >> input) {
        if (input == "k") {
            int key = 0;
            std::cin >> key;

            set.insert(key);
        } else if (input == "q") {
            int fst = 0, snd = 0;
            std::cin >> fst >> snd;

            int dist = 0;
            if (snd > fst) {
                auto lower = set.upper_bound(fst);
                auto upper = set.upper_bound(snd);

                dist = std::distance(lower, upper);
            }

            std::cout << dist << " ";
        }
    }
    std::cout << "\n";

    return 0;
}
