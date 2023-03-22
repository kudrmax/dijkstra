#include "dijkstra.h"
#include <iostream>
#include <fstream>
#include <cstdlib>

int main(int arg_count, char* arg_vars[]) {
    try {
        auto [file, from, to] = dijkstra::parse_args(arg_count, arg_vars);
        auto gr = dijkstra::read_graph(file);
//        print(gr);
//        auto [route, vec] = dijkstra::dijkstra_algorithm(gr, 2, 1);
//        print(gr);
//        double route;
//        dijkstra::route_t vec;
//        auto pair = dijkstra::dijkstra_algorithm(gr, 0, 0);

//        for (size_t i = 0; i < gr.size(); ++i) {
//            for (size_t j = 0; j < gr.size(); ++j) {
//                auto [route, vec] = dijkstra::dijkstra_algorithm(gr, i, j);
//                std::cout << i << " -> " << j << ": " << route << ", { ";
//                for (const auto& el: vec) {
//                    std::cout << el << " ";
//                }
//                std::cout << " }" << std::endl;
//            }
//            std::cout << std::endl;
//        }

//        auto [route, vec] = dijkstra::dijkstra_algorithm(gr, 3, 2);
//        std::cout << route << std::endl;
//        for (const auto& el: vec)
//            std::cout << el << " ";
//
//
//        system("dot -Tpng graph.dot -o graph.png");
//        dijkstra::make_image(gr, "graph.png", 2, 1);
        dijkstra::make_image(gr, "graph.png");



        gr.teest();
        return 0;


//        std::cout <<std::endl;
//        std::cout << dijkstra::dijkstra_algorithm(gr, 2, 1).first << std::endl;
//        std::cout << dijkstra::dijkstra_algorithm(gr, 0, 1).first << std::endl;
//        std::cout << dijkstra::dijkstra_algorithm(gr, 0, 2).first << std::endl;
//        std::cout << dijkstra::dijkstra_algorithm(gr, 0, 3).first << std::endl;
//        std::cout << dijkstra::dijkstra_algorithm(gr, 0, 4).first << std::endl;

    } catch (std::exception& ex) {
        dijkstra::print(ex);
        return 1;
    }


//    graph::Graph<std::string, int, double> gr;
//    gr.insert_node("one", 1);
//    gr.insert_node("two", 2);
//    gr.insert_node("three", 3);
//    gr.insert_node("four", 4);
//
//    gr.insert_edge(std::make_pair("one", "two"), 12);
//    gr.insert_edge(std::make_pair("one", "three"), 13);
//    gr.insert_edge(std::make_pair("four", "two"), 42);
//    gr.insert_edge(std::make_pair("two", "four"), 24);
//    gr.insert_edge(std::make_pair("two", "two"), 24);
//
//    gr.print();
//
//    double weight = 3.14;
//    std::vector<size_t> route = { 0, 3, 2, 5 };
//    dijkstra::print_results(weight, route);
    return 0;
}
