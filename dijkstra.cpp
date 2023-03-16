#include "dijkstra.h"
#include <sstream>
#include <fstream>
#include<functional>

void dijkstra::print_results(weight_t weight, const route_t& route) {
    std::cout << "route:";
    for (auto const& key: route)
        std::cout << " " << key;
    std::cout << std::endl;
    std::cout << "weight: " << weight << std::endl;
}

void delete_slashes(std::string& str) {
    if (str.back() == '|' || str.back() == '/' || str.back() == '\\')
        str.pop_back();
    if (str.front() == '|' || str.front() == '/' || str.front() == '\\')
        str.erase(str.begin());
}

std::tuple<const char*, dijkstra::node_name_t, dijkstra::node_name_t> dijkstra::parse_args(int arg_count,
                                                                                           char** arg_vars) {
    using namespace std::string_literals;
    if (arg_count != 7)
        throw std::runtime_error("Invalid number of arguments");
    std::unordered_map<std::string, const char*> arg_map = {
            { "--file", nullptr },
            { "--from", nullptr },
            { "--to",   nullptr }};
    for (size_t i = 1; i < 7; i += 2) {
        std::string str = arg_vars[i];
        auto it = arg_map.find(arg_vars[i]);
        if (it == arg_map.end())
            throw std::runtime_error("Invalid argument: "s + arg_vars[i]);
        if (it->second != nullptr)
            throw std::runtime_error("Duplicated argument: "s + arg_vars[i]);
        it->second = arg_vars[i + 1];
    }
    size_t from;
    size_t to;
    std::stringstream stream;
    stream << arg_map["--from"] << ' ' << arg_map["--to"];
    stream >> from >> to;
    if (!stream.eof())
        throw std::runtime_error("Invalid parameters of arguments");
    return { arg_map["--file"], from, to };
}

void dijkstra::print(const std::exception& ex) noexcept {
    std::cout << ex.what() << std::endl;
}

void add_string_to_graph(dijkstra::graph_t& gr, std::string& str, size_t row, size_t size_of_matrix) {
    if (row > size_of_matrix)
        throw std::runtime_error("row > size_of_matrix");
    dijkstra::weight_t weight;
    size_t column = 0;
    std::istringstream iss;
    iss.str(str);
    for (; iss >> weight; ++column)
        if (weight != 0 && column <= size_of_matrix)
            gr.insert_edge({ row, column }, weight);
    if (column - 1 != size_of_matrix)
        throw std::runtime_error("column != size_of_matrix");
    if (!iss.eof())
        throw std::runtime_error("There is no correct symbol");
}


size_t add_node_to_graph_from_string(dijkstra::graph_t& gr, std::string& str) {
    const double INF = std::numeric_limits<double>::infinity();
    size_t column = 0;
    std::istringstream iss;
    iss.str(str);
    dijkstra::weight_t weight;
    for (; iss >> weight; ++column)
        gr.insert_node(column, { INF, INF });
    return column - 1;
}

dijkstra::graph_t dijkstra::read_graph(const char* file_name) {
    using namespace std::string_literals;
    graph_t gr;

    std::ifstream fin(file_name);
    if (!fin.is_open())
        throw std::runtime_error("Can't open file named: "s + file_name);

    int row = 0;
    std::string str;
    size_t column_first = 0;
    size_t column_this = 0;
    size_t size_of_matrix = 0;

    for (; std::getline(fin, str); ++row) {
        delete_slashes(str);
        if (row == 0)
            size_of_matrix = add_node_to_graph_from_string(gr, str);
        add_string_to_graph(gr, str, row, size_of_matrix);
    }
    if (row - 1 != size_of_matrix)
        throw std::runtime_error("row < size_of_matrix");
    return gr;
}

struct MyLess {
//    bool operator()(dijkstra::graph_t::Node it1, dijkstra::graph_t::Node it2) const {
    bool operator()(std::pair<dijkstra::node_name_t, dijkstra::graph_t::Node> it1,
                    std::pair<dijkstra::node_name_t, dijkstra::graph_t::Node> it2) const {
        return it1.second.value().first < it2.second.value().first;
    }
};

std::pair<dijkstra::weight_t, dijkstra::route_t>
dijkstra::dijkstra_algorithm(const graph_t& gr, node_name_t key_from, node_name_t key_to) {
    const double INF = std::numeric_limits<double>::infinity();
    auto it_from = gr.find(key_from);
    it_from->second.value() = { 0, 0 };
    (++it_from)->second.value() = { 1, 0 };

    auto min_ver = std::min_element(gr.begin(), gr.end(),
                                  [](std::pair<dijkstra::node_name_t, dijkstra::graph_t::Node> it1,
                                     std::pair<dijkstra::node_name_t, dijkstra::graph_t::Node> it2) {
                                      return it1.second.value().first < it2.second.value().first;
                                  });

    return { 5, { 5 }};
}
