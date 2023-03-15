#include "dijkstra.h"

void dijkstra::print_results(weight_t weight, const route_t& route) {
    std::cout << "route:";
    for (auto const& key: route)
        std::cout << " " << key;
    std::cout << std::endl;
    std::cout << "weight: " << weight << std::endl;
}

dijkstra::graph_t read_graph(const char* file_name){}
