#include "dijkstra.h"
#include <sstream>

//void dijkstra::print_results(weight_t weight, const route_t& route) {
//    std::cout << "route:";
//    for (auto const& key: route)
//        std::cout << " " << key;
//    std::cout << std::endl;
//    std::cout << "weight: " << weight << std::endl;
//}

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
