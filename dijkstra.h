#pragma once

#include "/Users/kudr.max/CLionProjects/Graph/sources/graph.h"
#include <vector>
#include <iostream>

namespace dijkstra {
    using node_name_t = typename std::size_t;
    using node_data_t = typename std::pair<int, int>;
    using weight_t = double;
    using graph_t = typename graph::Graph<node_name_t, node_data_t, weight_t>;
    using route_t = std::vector<node_name_t>;

    std::tuple<const char*, node_name_t, node_name_t> parse_args(int arg_count, char* arg_vars[]);
    graph_t read_graph(const char* file_name);
    void print_results(weight_t, const route_t&);
    std::pair<weight_t, route_t>
    dijkstra(const graph_t& graph, node_name_t key_from, node_name_t key_to);
    void print(const std::exception& ex) noexcept;
}

