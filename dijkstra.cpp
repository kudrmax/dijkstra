#include "dijkstra.h"
#include <sstream>
#include <fstream>

//void dijkstra::print_results(weight_t weight, const route_t& route) {
//    std::cout << "route:";
//    for (auto const& key: route)
//        std::cout << " " << key;
//    std::cout << std::endl;
//    std::cout << "weight: " << weight << std::endl;
//}

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

//void add_row_to_graph(dijkstra::graph_t& gr, std::string& str) {
//
//    std::vector<double> vec;
//    double d;
//    size_t count_columns = 0;
//    size_t size_of_matrix = 0;
//    size_t count_rows = 0;
//
//    std::cout << str << std::endl;
//    delete_slashes(str);
//
//    std::istringstream iss;
//    iss.str(str);
//
//    for (count_columns = 0; iss >> d; ++count_columns)
//        vec.push_back(d);
//
//    if (!iss.eof())
//        throw std::runtime_error("There is no correct symbol");
//    if (count_rows == 0)
//        size_of_matrix = count_columns;
//    else if (size_of_matrix != count_columns)
//        throw std::runtime_error("Matrix should be square");
//
//    for (const auto el: vec)
//        std::cout << el << std::endl;
//    vec.clear();
//}
//
//
//dijkstra::graph_t dijkstra::read_graph(const char* file_name) {
//    using namespace std::string_literals;
//
//    std::ifstream fin(file_name);
//    if (!fin.is_open())
//        throw std::runtime_error("Can't open file named: "s + file_name);
//
//    dijkstra::graph_t gr;
//
//    for (std::string str; std::getline(fin, str); ++count_rows) {
//        add_row_to_graph(gr, str);
//    }
//    if (count_rows != size_of_matrix)
//        throw std::runtime_error("Matrix should be square");
//    return gr;
//}

size_t add_string_to_graph(dijkstra::graph_t& gr, std::string& str, size_t row) {
    const auto INF = std::numeric_limits<int>::infinity();
    std::cout << str << std::endl;
    dijkstra::weight_t weight;
    size_t column = 0;
    std::istringstream iss;
    iss.str(str);
    for (; iss >> weight; ++column) {
        if (row == 0) {
            gr.insert_node(column, { INF, 0 });
        }
        if (weight != 0) {
            gr.insert_edge({ row, column }, weight);
        }
    }
    if (!iss.eof())
        throw std::runtime_error("There is no correct symbol");
    std::cout << "column: " << column << std::endl;
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

    for (; std::getline(fin, str); ++row) {
        if(row > column_first)
            throw std::runtime_error("row if bigger");
        delete_slashes(str);
        if (row == 0) {
            column_first = add_string_to_graph(gr, str, row);
            column_this = column_first;
        } else
            column_this = add_string_to_graph(gr, str, row);
        if (column_this != column_first) {
            throw std::runtime_error("Matrix should be square 1");
        }
        std::cout << "column_first: " << column_first << std::endl;
        std::cout << "row: " << row << std::endl;
    }
    std::cout << "row_f: " << row << std::endl;
    if (row - 1 != column_first)
        throw std::runtime_error("Matrix should be square 2");
    return gr;
}