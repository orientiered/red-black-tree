#include <benchmark/benchmark.h>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <regex>
#include <string>
#include <unistd.h>
#include <utility>
#include "rbtree.hpp"

enum QueryType {
    KEY = 0,
    QUERY = 1
};
struct Query {
    QueryType type;
    int a, b;
};

std::vector<Query> load_test(const std::string& filename) {
    std::fstream test_file(filename, std::ios::in);
    if (!test_file.good()) {
        // TODO: skip test with error
        std::cerr << "Failed to open file\n";
    }

    std::vector<Query> tests;

    std::string input = "";
    while (test_file >> input) {
        if (input == "k") {
            int key = 0; test_file >> key;

            tests.push_back({KEY, key, 0});
        }
        else if (input == "q") {
            int fst = 0, snd = 0;
            test_file >> fst >> snd;

            tests.push_back({QUERY, fst , snd});
        }
    }

    return tests;
}

std::vector<int> tree_bench(std::vector<Query>& queries) {
    RBTree::Tree<int> tree;

    std::vector<int> result;

    for (Query& q: queries) {
        if (q.type == KEY) {
            tree.insert(q.a);
        } else if (q.type == QUERY) {
            int distance = 0;
            if (q.b > q.a)
                distance = tree.distance(tree.lower_bound(q.a), tree.upper_bound(q.b));

            result.push_back(distance);
        }
    }

    return result;
}

std::vector<int> set_bench(std::vector<Query>& queries) {
    std::set<int> set;

    std::vector<int> result;

    for (Query& q: queries) {
        if (q.type == KEY) {
            set.insert(q.a);
        } else if (q.type == QUERY) {
            int distance = 0;
            if (q.b > q.a)
                distance = std::distance(set.lower_bound(q.a), set.upper_bound(q.b));

            result.push_back(distance);
        }
    }

    return result;
}

using ProcessFn = std::vector<int> (std::vector<Query>&);

template <ProcessFn tree_impl>
static void BM_Tree_impl(benchmark::State& state, std::string filename) {
    std::vector<Query> test = load_test(filename);
    for (auto _ : state) {
        tree_impl(test);
    }
}

static std::vector<std::pair<std::string, std::string>> parse_bench_list(std::fstream& list) {
    std::vector<std::pair<std::string, std::string>> tests;

    std::string line;

    std::regex pattern(R"(\s*\"([^\"]*)\"\s*,\s*\"([^\"]*)\"\s*)");
    std::smatch matches;
    while (std::getline(list, line)) {
        if (std::regex_match(line, matches, pattern)) {
            if (matches.size() == 3) {
                tests.emplace_back(matches[1].str(), matches[2].str());
            }
        } else if (line.find_first_not_of(" \t\r\n") != std::string::npos) {
            std::cerr << "Warning: Line doesn't match expected format: " << line << "\n";
        }
    }

    return tests;
}


int main(int argc, char **argv) {
    auto bench_tree = [](benchmark::State& state, std::string filename) {
        return BM_Tree_impl<tree_bench>(state, filename);
    };
    auto bench_set = [](benchmark::State& state, std::string filename) {
        return BM_Tree_impl<set_bench>(state, filename);
    };

    std::string bench_type = std::string(std::getenv("RBTREE_BENCH_TYPE"));
    auto bench_func = (bench_type == "STD_SET") ? bench_set : bench_tree;
    benchmark::AddCustomContext("Implementation", (bench_type == "STD_SET") ? "std::set" : "rbtree");

    std::string bench_list_file_name = std::string(std::getenv("RBTREE_BENCH_TEST_LIST"));
    std::fstream bench_list(bench_list_file_name, std::ios::in);
    if (!bench_list.good()) {
        std::cerr << "Failed to open bench list file!\n";
    }

    std::string path_prefix = std::string(std::getenv("RBTREE_BENCH_PATH_PREFIX"));

    auto tests = parse_bench_list(bench_list);
    for (auto test: tests) {
        ::benchmark::RegisterBenchmark(test.first, bench_func, path_prefix + "/" + test.second);
    }

    ::benchmark::Initialize(&argc, argv);
    ::benchmark ::RunSpecifiedBenchmarks();
    ::benchmark ::Shutdown();

    return 0;
}

// BENCHMARK_MAIN();
