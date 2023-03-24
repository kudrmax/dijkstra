#include "dijkstra.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>

int main(int arg_count, char* arg_vars[]) {
    try {
        auto [file, from, to] = dijkstra::parse_args(arg_count, arg_vars);
        dijkstra::graph_t  graph = dijkstra::read_graph(file);
        auto [route, vec] = dijkstra::dijkstra_algorithm(graph, from, to);
        dijkstra::print_results(route, vec);
        dijkstra::make_image(graph, "graph.png");
        dijkstra::make_image(graph, from, to, "graph_dijkstra.png");
        return 0;
    } catch (std::exception& ex) {
        dijkstra::print(ex);
        return 1;
    }

    return 0;
}
