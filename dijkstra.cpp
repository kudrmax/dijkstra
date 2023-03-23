#include "dijkstra.h"
#include <sstream>
#include <fstream>
#include <functional>

//void test_gtest(int x) {
//    std::cout << "I'm here" << std::endl;
//}

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
        gr.insert_node(column, { INF, 0, false });
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

struct MyCompareForDijkstra {
    bool operator()(dijkstra::node_pair_t node1, dijkstra::node_pair_t node2) {
        if (node2.second.value().is_passed)
            return true;
        if (node1.second.value().is_passed)
            return false;
        return (node1.second.value().weight_node <
                node2.second.value().weight_node);
    }
};

std::pair<dijkstra::weight_t, dijkstra::route_t>
dijkstra::dijkstra_algorithm(const graph_t& gr, node_name_t key_from, node_name_t key_to) {
    const double INF = std::numeric_limits<double>::infinity();
    auto it_from_zero = gr.find(key_from);

    for (const auto& pair: gr) {
        pair.second.value().weight_node = INF;
        pair.second.value().is_passed = false;
        pair.second.value().who_change = { false };
    }
    it_from_zero->second.value().weight_node = 0;

    for (size_t i = 0; i < gr.size(); ++i) {
        auto min_ver = std::min_element(gr.begin(), gr.end(), MyCompareForDijkstra());
        for (const auto& edge_pair: min_ver->second) {
            auto weight_of_edge_to = edge_pair.second;
            auto from_edge = min_ver->first;
            auto it_from = gr.find(from_edge);
            auto weight_of_node_from = it_from->second.value().weight_node;
            auto to_edge = edge_pair.first;
            auto it_to = gr.find(to_edge);
            auto& weight_of_node_to = it_to->second.value().weight_node;
            auto sum = weight_of_node_from + weight_of_edge_to;
            if (!it_to->second.value().is_passed && sum < weight_of_node_to) {
                weight_of_node_to = sum;
                it_to->second.value().who_change.is_changed = true;
                it_to->second.value().who_change.who = it_from->first;
            }
        }
        min_ver->second.value().is_passed = true;
    }
    auto route = gr.find(key_to);

    std::vector<node_name_t> vec;

    for (auto it = gr.find(key_to); it != gr.find(key_from) || !it->second.value().who_change.is_changed; it = gr.find(
            it->second.value().who_change.who)) {
//        if (it != gr.find(key_to) && it != gr.find(key_from))
        vec.push_back(it->first);
        if (!it->second.value().who_change.is_changed) break;
    }

    std::reverse(vec.begin(), vec.end());


    if (route->second.value().weight_node == 0) {
        vec.clear();
        vec.push_back(gr.find(key_from)->first);
    } else if (route->second.value().weight_node == INF) {
        vec.clear();
    }

    return { route->second.value().weight_node, vec };
}

void dijkstra::print(const dijkstra::node_data_t& val) noexcept {
    std::cout << "{ " <<
              val.weight_node << ", " <<
              val.is_passed << ", " <<
              val.who_change.who << " }";
}

void dijkstra::print(const node_pair_t& pair, std::string str) noexcept {
    std::cout << str;
    std::cout << pair.first << ": ";
    print(pair.second.value());
    std::cout << std::endl;
//    for (auto const& edges: pair.second.get_edge()) {
//        std::cout << " —> " << edges.first << " (" << edges.second << ")" << std::endl;
//    }
}

void dijkstra::print(const dijkstra::graph_t& gr) noexcept {
    std::cout << "GRAPHHH\n" << std::endl;
    for (auto const& pair: gr) {
        print(pair);
    }
    std::cout << std::endl;
}

void make_dot(const dijkstra::graph_t& gr, const std::string& dot, dijkstra::node_name_t node_1,
              dijkstra::node_name_t node_2) {
    auto [route, vec] = dijkstra::dijkstra_algorithm(gr, node_1, node_2);
    auto it_vec = vec.begin();
    std::ofstream fout(dot);
    fout << "digraph G {\n";
    fout << "node [shape=circle style=filled]\n";
    for (const auto& pair: gr) {
        auto node_from = pair.first;
        auto node = pair.second;
        for (const auto& edge: node) {
            const auto& node_to = edge.first;
            fout << "\t" << node_from << " -> " << node_to << " [label = " << edge.second;
            if (auto k_from = std::find(vec.begin(), vec.end(), node_from),
                        k_to = std::find(vec.begin(), vec.end(), node_to);
                    (++k_from) == k_to) {
                fout << " color = red";
            }
            fout << "];\n";
        }
    }
    for (const auto& el: vec)
        fout << el << " [color = red fillcolor=lightgray];\n";
    fout << "}\n";
}

void make_dot(const dijkstra::graph_t& gr, const std::string& dot) {
    std::ofstream fout(dot);
    fout << "digraph G {\n";
    fout << "node [shape=circle style=filled]\n";
    for (const auto& pair: gr) {
        auto node_from = pair.first;
        auto node = pair.second;
        for (const auto& edge: node) {
            const auto& node_to = edge.first;
            fout << "\t" << node_from << " -> " << node_to << " [label = " << edge.second;
            fout << "];\n";
        }
    }
    fout << "}\n";
}

void make_str_for_make_image() {

}

void dijkstra::make_image(const graph_t& gr, const std::string& name) {

    std::string str_dot = "graph.dot";
    std::string str;
    str = "dot -Tpng " + str_dot + " -o " + name;

    make_dot(gr, str_dot);

    char str_c_style[str.length() + 1];
    strcpy(str_c_style, str.c_str());

    system(str_c_style);
//    system("dot -Tpng graph.dot -o graph.png");
}

void dijkstra::make_image(const graph_t& gr, node_name_t node_1, node_name_t node_2, const std::string& name) {
    std::string str_dot = "graph.dot";
    std::string str;
    str = "dot -Tpng " + str_dot + " -o " + name;

    make_dot(gr, str_dot, node_1, node_2);

    char str_c_style[str.length() + 1];
    strcpy(str_c_style, str.c_str());

    system(str_c_style);
//    system("dot -Tpng graph.dot -o graph.png");
}







