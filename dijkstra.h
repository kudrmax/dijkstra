#pragma once

#include "graph/graph.h"
#include <vector>
#include <iostream>
#include <optional>

namespace dijkstra {
    using node_name_t = typename std::size_t;
    using weight_t = double;
    struct NodeData {
        struct who_change_t {
            who_change_t() = default;
            bool is_changed = false;
            node_name_t who;
            explicit who_change_t(bool flag) : is_changed(flag){};
        };
        weight_t weight_node = std::numeric_limits<double>::infinity();
        who_change_t who_change;
        bool is_passed = false;
    };
    using node_data_t = NodeData;
    using graph_t = typename graph::Graph<node_name_t, node_data_t, weight_t>;
    using route_t = std::vector<node_name_t>;
    using node_pair_t = typename std::pair<node_name_t, graph_t::Node>;

    std::tuple<const char*, node_name_t, node_name_t> parse_args(int arg_count, char* arg_vars[]);
    graph_t read_graph(const char* file_name);
    std::pair<weight_t, route_t> dijkstra_algorithm(graph_t& graph, const node_name_t& key_from, const node_name_t& key_to);
    void print_results(weight_t, const route_t&);
    void print(const std::exception& ex) noexcept;
    void print(const graph_t& gr) noexcept;
    void print(const node_pair_t& pair, std::string str = "") noexcept;
    void print(const NodeData&) noexcept;
    void make_image(const graph_t& gr, const std::string& name = "graph.png");
    void make_image(graph_t& gr, node_name_t node_1, node_name_t node_2, const std::string& name = "graph.png");
}

