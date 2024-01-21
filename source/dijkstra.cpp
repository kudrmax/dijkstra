#include "dijkstra.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string.h>
#include <stdexcept>

void dijkstra::print_results(const weight_t& weight, const route_t& route) {
    std::cout << "route:";
    for (auto const& key: route)
        std::cout << " " << key;
    std::cout << std::endl;
    std::cout << "weight: " << weight << std::endl;
}

std::tuple<const char*, dijkstra::node_name_t, dijkstra::node_name_t>
dijkstra::parse_args(int arg_count, char* arg_vars[]) {
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
    dijkstra::node_name_t from;
    dijkstra::node_name_t to;
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
        throw graph::GraphException("Matrix isn't square");
    dijkstra::weight_t weight;
    size_t column = 0;
    std::istringstream iss;
    iss.str(str);
    for (; iss >> weight; ++column)
        if (weight != 0 && column <= size_of_matrix)
            gr.insert_edge({ row, column }, weight);
    if (!iss.eof())
        throw graph::GraphException("Incorrect file");
    if (column - 1 != size_of_matrix)
        throw graph::GraphException("Matrix isn't square");
}

size_t add_node_to_graph_from_string(dijkstra::graph_t& gr, std::string& str) {
    size_t column = 0;
    std::istringstream iss;
    iss.str(str);
    dijkstra::weight_t weight;
    for (; iss >> weight; ++column)
        gr.insert_node(column);
    if (!iss.eof())
        throw graph::GraphException("Incorrect file");
    return column - 1;
}

void delete_slashes(std::string& str) {
    if (str.back() == '|' || str.back() == '/' || str.back() == '\\')
        str.pop_back();
    if (str.front() == '|' || str.front() == '/' || str.front() == '\\')
        str.erase(str.begin());
}

dijkstra::graph_t dijkstra::read_graph(const char* file_name) {
    using namespace std::string_literals;
    graph_t gr;

    std::ifstream fin(file_name);
    if (!fin.is_open())
        throw graph::GraphException("Can't open file named: "s + file_name);

    int row = 0;
    std::string str;
    size_t size_of_matrix = 0;

    for (; std::getline(fin, str); ++row) {
        delete_slashes(str);
        if (row == 0)
            size_of_matrix = add_node_to_graph_from_string(gr, str);
        add_string_to_graph(gr, str, row, size_of_matrix);
    }
    if (row - 1 != size_of_matrix)
        throw graph::GraphException("Matrix isn't square");
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

void initialization_value(dijkstra::graph_t& gr, const dijkstra::node_name_t& key_from) {
    const double INF = std::numeric_limits<double>::infinity();
    for (auto& pair: gr) {
        pair.second.value().weight_node = INF;
        pair.second.value().is_passed = false;
        pair.second.value().who_change = dijkstra::node_data_t::who_change_t{ false };
    }
    auto it_from = gr.find(key_from);
    it_from->second.value().weight_node = 0;
}

void change_graph_for_dijkstra(dijkstra::graph_t& gr) {
    for (size_t i = 0; i < gr.size(); ++i) {

        auto min_vertex = std::min_element(gr.begin(), gr.end(), MyCompareForDijkstra());

        for (const auto& edge_pair: min_vertex->second.edge()) {

            auto it_from = gr.find(min_vertex->first);
            auto it_to = gr.find(edge_pair.first);

            const auto& value_from = it_from->second.value();
            auto& value_to = it_to->second.value();

            const auto& weight_node_from = value_from.weight_node;
            auto& weight_node_to = value_to.weight_node;

            const auto& weight = edge_pair.second;
            if (weight < 0)
                throw graph::GraphException("Graph has edge with weight < 0");
            auto sum = weight_node_from + weight;

            if (!value_to.is_passed && sum < weight_node_to) {
                value_to.weight_node = sum;
                value_to.who_change.is_changed = true;
                value_to.who_change.who = it_from->first;
            }
        }
        min_vertex->second.value().is_passed = true;
    }
}

dijkstra::route_t
restore_route(const dijkstra::graph_t& gr, const dijkstra::node_name_t& key_from, const dijkstra::node_name_t& key_to) {
    dijkstra::route_t vec;

    if (key_from == key_to) {
        vec.push_back(gr.find(key_from)->first);
        return vec;
    }

    auto it_from = gr.find(key_from);
    auto it_to = gr.find(key_to);

    for (auto it = it_to;
         it != it_from || !it->second.value().who_change.is_changed;
         it = gr.find(it->second.value().who_change.who)) {
        vec.push_back(it->first);
        if (!it->second.value().who_change.is_changed) break;
    }
    std::reverse(vec.begin(), vec.end());
    return vec;
}

std::pair<dijkstra::weight_t, dijkstra::route_t>
dijkstra::dijkstra_algorithm(graph_t& gr, const node_name_t& key_from, const node_name_t& key_to) {
    if (gr.find(key_from) == gr.end() || gr.find(key_to) == gr.end())
        throw graph::GraphException("There is no key");
    initialization_value(gr, key_from);
    change_graph_for_dijkstra(gr);
    auto route = gr.find(key_to)->second.value().weight_node;
    if(route == std::numeric_limits<double>::infinity())
        throw graph::GraphException("There is no route");
    auto vec = restore_route(gr, key_from, key_to);
    return { route, vec };
}

void make_dot(dijkstra::graph_t& gr, const std::string& dot, dijkstra::node_name_t node_1,
              dijkstra::node_name_t node_2) {
    auto [route, vec] = dijkstra::dijkstra_algorithm(gr, node_1, node_2);
    std::ofstream fout(dot);
    fout << "digraph G {\n";
    fout << "node [shape=circle style=filled color=lightgrey ]\n";
    for (const auto& pair: gr) {
        const auto& key_from = pair.first;
        for (const auto& edge: pair.second) {
            const auto& key_to = edge.first;
            fout << "\t" << key_from << " -> " << key_to << " [label = " << edge.second;
            if (auto vec_el_from = std::find(vec.begin(), vec.end(), key_from),
                        vec_el_to = std::find(vec.begin(), vec.end(), key_to);
                    *(++vec_el_from) == *vec_el_to && vec_el_to != vec.end()) {
                fout << " fillcolor=lightblue";
            }
            else fout << " color=lightgrey fontcolor=lightgrey";
            fout << "];\n";
        }
    }
    for (const auto& el: vec)
        fout << el << " [ color = black fillcolor=lightblue];\n";
    fout << "}\n";
}

void make_dot(const dijkstra::graph_t& gr, const std::string& dot) {
    std::ofstream fout(dot);
    fout << "digraph G {\n";
    fout << "node [shape=circle style=filled]\n";
    for (const auto& pair: gr) {
        const auto& key_from = pair.first;
        for (const auto& edge: pair.second) {
            const auto& key_to = edge.first;
            fout << "\t" << key_from << " -> " << key_to << " [label = " << edge.second;
            fout << "];\n";
        }
    }
    fout << "}\n";
}

void dijkstra::make_image(const graph_t& gr, const std::string& name) {
    std::string str_dot = "graph.dot";
    std::string str;
    make_dot(gr, str_dot);
    str = "dot -Tpng " + str_dot + " -o " + name;
    system(str.c_str());
    std::remove(str_dot.c_str());
}

void dijkstra::make_image(graph_t& gr, const node_name_t& node_1, const node_name_t& node_2, const std::string& name) {
    std::string str_dot = "graph.dot";
    std::string str;
    make_dot(gr, str_dot, node_1, node_2);
    str = "dot -Tpng " + str_dot + " -o " + name;
    system(str.c_str());
    std::remove(str_dot.c_str());
}







