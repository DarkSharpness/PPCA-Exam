#include <fstream>
#include <iostream>
#include <optional>
#include <vector>
#include <span>
#include <set>

// Lines of integers.

using _Vec = std::vector <std::pair <int, int>>;

int extract_int(std::string_view &str) {
    while (str.size() && std::isspace(str[0]))
        str.remove_prefix(1);

    if (str.empty()) return 0;

    int result = 0;
    while (str.size()) {
        if (!std::isdigit(str[0])) break;
        result = result * 10 + (str[0] - '0');
        str.remove_prefix(1);
    }
    return result;
}

_Vec read(std::istream& file) {
    _Vec result;
    int x, y;
    while (file >> x >> y) result.push_back({x, y});
    return result;
}

std::set <std::pair <int, int>> edges;
std::size_t node_count;

void init_graph(_Vec vec) {
    if (vec.empty()) return;
    auto slice = std::span(vec);

    auto [n, m] = slice[0];
    slice = slice.subspan(1);

    if (slice.size() != std::size_t(m))
        throw std::runtime_error("Duplicate input edges");

    node_count = n;
    for (auto edge : slice) {
        edges.insert(edge);
        if (edge.first < 1 || edge.first > n || edge.second < 1 || edge.second > n)
            throw std::runtime_error("Invalid input node");
    }
}

void check_matching(std::span <std::pair <int, int>> vec) {
    std::vector <bool> s(node_count + 1);
    std::vector <bool> t(node_count + 1);
    if (vec.size() != node_count)
        throw std::runtime_error("Invalid matching size");

    for (auto [a, b] : vec) {
        if (s.at(a) || t.at(b))
            throw std::runtime_error("Invalid matching");

        s.at(a) = true;
        t.at(b) = true;
    }
}

void verify(_Vec vec) {
    if (vec.empty()) return;
    auto slice = std::span(vec);
    for (auto edge : slice)
        if (!edges.count(edge))
            throw std::runtime_error("Invalid edge in output");

    std::set <std::pair <int, int>> visited;
    for (auto edge : slice)
        if (!visited.insert(edge).second)
            throw std::runtime_error("Duplicate edge in output");

    check_matching(slice.subspan(0, node_count));
    check_matching(slice.subspan(node_count));
}

signed main(int argc, char* argv[]) {
    std::ifstream input (argv[1]);
    std::ifstream output(argv[2]);
    std::ifstream answer(argv[3]);
    std::ofstream score(argv[4]);
    std::ofstream message(argv[5]);
    std::string str, tmp;

    try {
        init_graph(read(input));

        bool has_answer {};
        answer >> has_answer;
        std::string tmp;
        output >> tmp;
        if (!has_answer) {
            if (tmp.starts_with("No")) {
                score << 1;
            } else {
                score << 0;
            }
        } else {
            if (!tmp.starts_with("Yes")) {
                score << 0;
            } else {
                verify(read(output));
                score << 1;
            }
        }
    } catch(std::exception &e) {
        message << e.what();
        score << 0;
        return 0;
    } catch(...) {
        message << "wtf is this?";
        score << 0;
        return 0;
    }
}
