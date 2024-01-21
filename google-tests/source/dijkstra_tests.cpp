#include "dijkstra.h"
#include <gtest/gtest.h>

using graph_t = typename dijkstra::graph_t;

TEST(Dijkstra, read_graph) {
    ASSERT_THROW(dijkstra::read_graph("read_graph_files/do_not_exist_file_name.txt"), graph::GraphException);
    ASSERT_THROW(dijkstra::read_graph("read_graph_files/columns_greater_than_rows.txt"), graph::GraphException);
    ASSERT_THROW(dijkstra::read_graph("read_graph_files/rows_greater_than_columns.txt"), graph::GraphException);
    ASSERT_THROW(dijkstra::read_graph("read_graph_files/incorrect_symbol.txt"), graph::GraphException);
    ASSERT_THROW(dijkstra::read_graph("read_graph_files/incorrect_symbol_2.txt"), graph::GraphException);
    ASSERT_THROW(dijkstra::read_graph("read_graph_files/not_rectangle_1.txt"), graph::GraphException);
    ASSERT_THROW(dijkstra::read_graph("read_graph_files/not_rectangle_2.txt"), graph::GraphException);
    ASSERT_THROW(dijkstra::read_graph("read_graph_files/not_rectangle_3.txt"), graph::GraphException);
    ASSERT_THROW(dijkstra::read_graph("read_graph_files/empty_file.txt"), graph::GraphException);
    ASSERT_NO_THROW(dijkstra::read_graph("read_graph_files/good_1.txt"));
    ASSERT_NO_THROW(dijkstra::read_graph("read_graph_files/good_2.txt"));
}

TEST(Dijkstra, parse_args) {
    {
//        const int arg_count = 7;
//        char* arg_vars[arg_count];
//        auto [file, from, to] = dijkstra::parse_args(arg_count, arg_vars);
//        ASSERT_NO_THROW(dijkstra::parse_args(arg_count, arg_vars));
    }
}
TEST(Dijkstra, dijkstra_algorithm) {
    const double INF = std::numeric_limits<double>::infinity();
    auto graph = dijkstra::read_graph("read_graph_files/good_1.txt");
    {
        auto [route, vec] = dijkstra::dijkstra_algorithm(graph, 2, 1);
        ASSERT_EQ(route, 14.5);
        std::vector<dijkstra::node_name_t> vec_true = { 2, 4, 3, 1 };
        ASSERT_EQ(vec, vec_true);
    }
    {
        // tie
        auto [route, vec] = dijkstra::dijkstra_algorithm(graph, 0, 0);
        ASSERT_EQ(route, 0);
        std::vector<dijkstra::node_name_t> vec_true = { 0 };
        ASSERT_EQ(vec, vec_true);
    }
    {
        auto [route, vec] = dijkstra::dijkstra_algorithm(graph, 2, 0);
        ASSERT_EQ(route, 1.2);
        std::vector<dijkstra::node_name_t> vec_true = { 2, 0 };
        ASSERT_EQ(vec, vec_true);
    }
    {
        auto [route, vec] = dijkstra::dijkstra_algorithm(graph, 4, 1);
        ASSERT_EQ(route, 4);
        std::vector<dijkstra::node_name_t> vec_true = { 4, 3, 1 };
        ASSERT_EQ(vec, vec_true);
    }
    {
        ASSERT_THROW(dijkstra::dijkstra_algorithm(graph, 0, 2), graph::GraphException);
        ASSERT_THROW(dijkstra::dijkstra_algorithm(graph, 4, 2), graph::GraphException);
        ASSERT_THROW(dijkstra::dijkstra_algorithm(graph, 10, 0), graph::GraphException);
        ASSERT_THROW(dijkstra::dijkstra_algorithm(graph, 0, 10), graph::GraphException);
        ASSERT_THROW(dijkstra::dijkstra_algorithm(graph, 10, 10), graph::GraphException);
    }
    {
        auto graph_weight_less_than_zero = dijkstra::read_graph("read_graph_files/weight_less_than_zero.txt");
        ASSERT_THROW(dijkstra::dijkstra_algorithm(graph_weight_less_than_zero, 0, 1), graph::GraphException);
    }
}