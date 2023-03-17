#pragma once

#include "/Users/kudr.max/CLionProjects/Graph/sources/graph.h"
#include <vector>
#include <iostream>

namespace dijkstra {
    using node_name_t = typename std::size_t;
    using weight_t = double;
//    using node_data_t = typename std::pair<double, double>;
    struct NodeData{
        weight_t weight_node;
        node_name_t who_change;
        bool is_passed = false;
        void print() const { std::cout << "{ " << weight_node << ", " << who_change << ", " << is_passed << " }" << std::endl;}
    };
    using node_data_t = NodeData;
    using graph_t = typename graph::Graph<node_name_t, node_data_t, weight_t>;
    using route_t = std::vector<node_name_t>;

    std::tuple<const char*, node_name_t, node_name_t> parse_args(int arg_count, char* arg_vars[]);
    graph_t read_graph(const char* file_name);
    std::pair<weight_t, route_t> dijkstra_algorithm(const graph_t& graph, node_name_t key_from, node_name_t key_to);
    void print_results(weight_t, const route_t&);
    void print(const std::exception& ex) noexcept;
    void print(const graph_t& gr) noexcept;
    void print(const std::pair<key_t, dijkstra::graph_t::Node>& pair, std::string str = "") noexcept;
    void print(const NodeData&) noexcept;
}

