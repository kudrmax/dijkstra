#include "dijkstra.h"
#include <iostream>
#include <fstream>

int main(int arg_count, char* arg_vars[]) {
    try{
        auto [file, from, to] = dijkstra::parse_args(arg_count, arg_vars);
//        std::cout << file << std::endl;
//        std::cout << from << std::endl;
//        std::cout << to << std::endl;
        dijkstra::read_graph(file);
    } catch(std::exception& ex){
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
