#include "../../source/dijkstra.h"
#include <gtest/gtest.h>

using graph_t = typename graph::Graph<dijkstra::node_name_t, int, dijkstra::weight_t>;
using node_t = typename graph::Graph<dijkstra::node_name_t, int, dijkstra::weight_t>::Node;

TEST(Graph, default_constructor) {
    graph::Graph<int, int, int> graph1;
    graph::Graph<std::string, std::string, double> graph2;
    ASSERT_EQ(graph1.empty(), true);
    ASSERT_EQ(graph1.size(), 0);
    ASSERT_EQ(graph2.empty(), true);
    ASSERT_EQ(graph2.size(), 0);
}
TEST(Graph, copy_constructor) {
    graph_t graph;
    graph.insert_node(1, 0);

    graph_t graph1(graph);

    ASSERT_EQ(graph1.size(), graph.size());
    ASSERT_EQ(graph1.empty(), graph.empty());
    ASSERT_EQ(graph1.begin()->first, graph.begin()->first);
    ASSERT_EQ(graph1.begin()->second.value(), graph.begin()->second.value());
    ASSERT_EQ(graph1.degree_in(graph1.begin()->first), graph.degree_in(graph.begin()->first));
    ASSERT_EQ(graph1.degree_out(graph1.begin()->first), graph.degree_out(graph.begin()->first));
}
TEST(Graph, move_constructor) {
    graph_t graph;
    graph.insert_node(1, 0);
    graph.insert_node(2, 0);

    ASSERT_EQ(graph.size(), 2);

    graph_t graph1 = std::move(graph);

    ASSERT_EQ(graph.empty(), true);
    ASSERT_EQ(graph1.size(), 2);
}
TEST(Graph, size) {
    graph_t gr_zero;
    graph_t graph;
    graph.insert_node(1, 0);
    graph.insert_node(2, 0);
    ASSERT_EQ(gr_zero.size(), 0);
    ASSERT_EQ(graph.size(), 2);
}
TEST(Graph, empty) {
    graph_t gr_zero;
    graph_t graph;
    graph.insert_node(1, 0);
    graph.insert_node(2, 0);
    ASSERT_EQ(gr_zero.empty(), true);
    ASSERT_EQ(graph.empty(), false);
}
TEST(Graph, clear) {
    graph_t graph;
    graph.insert_node(1, 0);
    graph.insert_node(2, 0);
    graph.clear();
    ASSERT_EQ(graph.empty(), true);
}
TEST(Graph, swap) {
    graph_t graph1;
    graph_t graph2;
    graph1.insert_node(1, 0);
    graph1.swap(graph2);
    ASSERT_EQ(graph1.size(), 0);
    ASSERT_EQ(graph2.size(), 1);
    graph::swap(graph1, graph2);
    ASSERT_EQ(graph1.size(), 1);
    ASSERT_EQ(graph2.size(), 0);
}
TEST(Graph, operator_square_brackets) {
    graph_t graph;
    graph.insert_node(0, 1);
    ASSERT_EQ(graph[0].value(), 1);
    graph[0].value() = 2;
    ASSERT_EQ(graph[0].value(), 2);
    node_t node{ 3 };
    graph[0] = node;
    ASSERT_EQ(graph[0].value(), node.value());
    graph[1] = node;
    ASSERT_EQ(graph.size(), 2);
    graph[2];
    ASSERT_EQ(graph.size(), 3);
}
TEST(Graph, at) {
    graph_t graph;
    graph.insert_node(0, 1);
    ASSERT_EQ(graph.at(0).value(), 1);
    graph.at(0).value() = 2;
    ASSERT_EQ(graph.at(0).value(), 2);
    node_t node{ 3 };
    graph.at(0) = node;
    ASSERT_EQ(graph.at(0).value(), node.value());
    ASSERT_THROW(graph.at(1) = node, graph::GraphException);

    const auto& cref = graph;
    ASSERT_EQ(cref.at(0).value(), node.value());
    ASSERT_THROW(cref.at(1).value(), graph::GraphException);
}
TEST(Graph, find) {
    graph_t graph;
    graph.insert_node(1, 11);
    graph.insert_node(2, 22);
    graph.insert_node(3, 33);
    ASSERT_EQ(graph.find(2)->second.value(), 22);
    const auto& cref = graph;
    ASSERT_EQ(cref.find(2)->second.value(), 22);
}
TEST(Graph, degree_in_out) {
    graph_t graph;
    graph.insert_node(1, 11);
    graph.insert_node(2, 22);
    graph.insert_node(3, 33);
    graph.insert_node(4, 33);
    graph.insert_edge({ 2, 1 }, 21);
    graph.insert_edge({ 3, 1 }, 31);
    graph.insert_edge({ 3, 2 }, 32);
    graph.insert_edge({ 3, 3 }, 11);
    ASSERT_EQ(graph.degree_in(1), 2);
    ASSERT_EQ(graph.degree_in(2), 1);
    ASSERT_EQ(graph.degree_in(3), 1);
    ASSERT_EQ(graph.degree_in(4), 0);
    ASSERT_THROW(graph.degree_in(5), graph::GraphException);
    ASSERT_EQ(graph.degree_out(1), 0);
    ASSERT_EQ(graph.degree_out(2), 1);
    ASSERT_EQ(graph.degree_out(3), 3);
    ASSERT_EQ(graph.degree_out(4), 0);
    ASSERT_THROW(graph.degree_out(5), graph::GraphException);
}
TEST(Graph, loop) {
    graph_t graph;
    graph.insert_node(1, 11);
    graph.insert_node(2, 22);
    graph.insert_node(3, 33);
    graph.insert_node(4, 33);
    graph.insert_edge({ 2, 1 }, 21);
    graph.insert_edge({ 3, 1 }, 31);
    graph.insert_edge({ 3, 2 }, 32);
    graph.insert_edge({ 3, 3 }, 11);
    ASSERT_EQ(graph.loop(1), false);
    ASSERT_EQ(graph.loop(2), false);
    ASSERT_EQ(graph.loop(3), true);
    ASSERT_EQ(graph.loop(4), false);
    ASSERT_THROW(graph.degree_out(5), graph::GraphException);
}
TEST(Graph, insert_node) {
    graph_t graph;
    {
        auto [it, success] = graph.insert_node(0, 1);
        ASSERT_EQ(it->second.value(), 1);
        ASSERT_EQ(success, true);
    }
    {
        auto [it, success] = graph.insert_node(0, 2);
        ASSERT_EQ(it->second.value(), 1);
        ASSERT_EQ(success, false);
    }
    {
        auto [it, success] = graph.insert_node(1);
        ASSERT_EQ(success, true);
    }
    {
        auto [it, success] = graph.insert_node(1);
        ASSERT_EQ(success, false);
    }
}
TEST(Graph, insert_or_assign_node) {
    graph_t graph;
    {
        auto [it, success] = graph.insert_or_assign_node(0, 1);
        ASSERT_EQ(it->second.value(), 1);
        ASSERT_EQ(success, true);
    }
    {
        auto [it, success] = graph.insert_or_assign_node(0, 2);
        ASSERT_EQ(it->second.value(), 2);
        ASSERT_EQ(success, false);
    }
}

TEST(Graph, insert_edge) {
    graph_t graph;
    graph.insert_node(1, 11);
    graph.insert_node(2, 22);
    {
        auto [it, success] = graph.insert_edge({ 1, 2 }, 12);
        ASSERT_EQ(success, true);
        ASSERT_EQ(graph[1].edge().empty(), false);
        ASSERT_EQ(graph[1].edge().size(), 1);
        ASSERT_EQ(it->second, 12);
    }
    {
        auto [it, success] = graph.insert_edge({ 1, 2 }, 1122);
        ASSERT_EQ(success, false);
        ASSERT_EQ(it->second, 12);
    }
    {
        ASSERT_THROW(graph.insert_edge({ 1, 3 }, 13), graph::GraphException);
    }
}
TEST(Graph, insert_or_assign_edge) {
    graph_t graph;
    graph.insert_node(1, 11);
    graph.insert_node(2, 22);
    {
        auto [it, success] = graph.insert_or_assign_edge({ 1, 2 }, 12);
        ASSERT_EQ(success, true);
        ASSERT_EQ(graph[1].edge().empty(), false);
        ASSERT_EQ(graph[1].edge().size(), 1);
        ASSERT_EQ(it->second, 12);
    }
    {
        auto [it, success] = graph.insert_or_assign_edge({ 1, 2 }, 1122);
        ASSERT_EQ(success, false);
        ASSERT_EQ(it->second, 1122);
    }
    {
        ASSERT_THROW(graph.insert_or_assign_edge({ 1, 3 }, 13), graph::GraphException);
    }
}
TEST(Node, default_constructor) {
    graph::Graph<int, int, int>::Node node1;
    graph::Graph<std::string, std::string, double>::Node node2;
    ASSERT_EQ(node1.empty(), true);
    ASSERT_EQ(node2.empty(), true);
}
TEST(Node, value_constructor) {
    graph::Graph<int, int, int>::Node node1{ 5 };
    graph::Graph<std::string, std::string, double>::Node node2{ "text" };
    ASSERT_EQ(node1.value(), 5);
    ASSERT_EQ(node2.value(), "text");
}
TEST(Node, copy_constructor) {
    graph_t graph;
    graph.insert_node(1, 11);
    graph.insert_node(2, 22);
    graph.insert_node(3, 33);
    graph.insert_node(4, 33);
    graph.insert_edge({ 3, 1 }, 31);
    graph.insert_edge({ 3, 2 }, 32);
    graph.insert_edge({ 3, 3 }, 11);
    node_t& node = graph.find(3)->second;
    node_t node_copy(node);
    ASSERT_EQ(node_copy.empty(), false);
    ASSERT_EQ(node_copy.value(), node.value());
    ASSERT_EQ(node_copy.size(), node.size());
    ASSERT_EQ(node_copy.size(), 3);
}
TEST(Node, move_constructor) {
    graph_t graph;
    graph.insert_node(1, 11);
    graph.insert_node(2, 22);
    graph.insert_node(3, 33);
    graph.insert_node(4, 33);
    graph.insert_edge({ 3, 1 }, 31);
    graph.insert_edge({ 3, 2 }, 32);
    graph.insert_edge({ 3, 3 }, 11);
    auto& node = graph.find(3)->second;
    node_t node_move = std::move(node);
    ASSERT_EQ(node.empty(), true);
    ASSERT_EQ(node_move.empty(), false);
    ASSERT_EQ(node_move.value(), 33);
    ASSERT_EQ(node_move.edge().size(), 3);
}
TEST(Node, size) {
    graph_t graph;
    graph.insert_node(1, 11);
    graph.insert_node(2, 22);
    graph.insert_node(3, 33);
    graph.insert_node(4, 33);
    graph.insert_edge({ 3, 1 }, 31);
    graph.insert_edge({ 3, 2 }, 32);
    graph.insert_edge({ 3, 3 }, 11);
    node_t& node_value_edge = graph.find(3)->second;
    node_t node_value{ 5 };
    node_t node_empty;
    ASSERT_EQ(node_value_edge.size(), 3);
    ASSERT_EQ(node_value.size(), 0);
    ASSERT_EQ(node_empty.size(), 0);
}
TEST(Node, empty) {
    graph_t graph;
    graph.insert_node(1, 11);
    graph.insert_node(2, 22);
    graph.insert_node(3, 33);
    graph.insert_node(4, 33);
    graph.insert_edge({ 3, 1 }, 31);
    graph.insert_edge({ 3, 2 }, 32);
    graph.insert_edge({ 3, 3 }, 11);
    node_t& node1 = graph.find(3)->second;
    node_t& node2 = graph.find(4)->second;
    ASSERT_EQ(node1.empty(), false);
    ASSERT_EQ(node2.empty(), true);
}
TEST(Node, clear) {
    graph_t graph;
    graph.insert_node(1, 11);
    graph.insert_node(2, 22);
    graph.insert_node(3, 33);
    graph.insert_node(4, 33);
    graph.insert_edge({ 3, 1 }, 31);
    graph.insert_edge({ 3, 2 }, 32);
    graph.insert_edge({ 3, 3 }, 11);
    node_t& node = graph.find(3)->second;
    node.clear();
    ASSERT_EQ(node.empty(), true);
}
TEST(Node, erase_edge) {
    graph_t graph;
    graph.insert_node(1, 11);
    graph.insert_node(2, 22);
    graph.insert_node(3, 33);
    graph.insert_node(4, 33);
    graph.insert_edge({ 3, 1 }, 31);
    graph.insert_edge({ 3, 2 }, 32);
    graph.insert_edge({ 3, 3 }, 11);
    node_t& node = graph.find(3)->second;

    size_t flag;
    flag = node.erase_edge(1);
    ASSERT_EQ(node.size(), 2);
    ASSERT_EQ(flag, 1);
    flag = node.erase_edge(5);
    ASSERT_EQ(node.size(), 2);
    ASSERT_EQ(flag, 0);

    auto it = node.begin();
    auto it_erase = node.erase_edge(it);
    ASSERT_EQ(node.size(), 1);
    ASSERT_EQ(it_erase, node.begin());
}
TEST(Node, erase_node) {
    graph_t graph;
    graph.insert_node(1);
    graph.insert_node(2);
    graph.insert_node(3);
    graph.insert_node(4);
    graph.insert_edge({ 2, 1 }, 21);
    graph.insert_edge({ 3, 1 }, 31);
    graph.insert_edge({ 3, 2 }, 32);
    graph.insert_edge({ 3, 3 }, 33);
    auto graph1 = graph;
    size_t flag;

    ASSERT_EQ(graph.size(), 4);
    flag = graph.erase_node(2);
    ASSERT_EQ(graph.size(), 3);
    ASSERT_EQ(flag, 1);
    ASSERT_EQ(graph.find(3)->second.size(), 2);
    ASSERT_EQ(graph.find(1)->second.size(), 0);
    ASSERT_EQ(graph.find(2), graph.end());
    flag = graph.erase_node(5);
    ASSERT_EQ(graph.size(), 3);
    ASSERT_EQ(flag, 0);

    ASSERT_EQ(graph1.size(), 4);
    auto it = graph1.find(2);
    auto it_erase = graph1.erase_node(it);
    ASSERT_EQ(graph1.size(), 3);
    ASSERT_EQ(graph1.find(3)->second.size(), 2);
    ASSERT_EQ(graph1.find(1)->second.size(), 0);
    ASSERT_EQ(graph1.find(2), graph1.end());
}
TEST(Node, value) {
    node_t node{ 5 };
    ASSERT_EQ(node.value(), 5);
    node.value() = 6;
    ASSERT_EQ(node.value(), 6);
    const node_t& cref_node = node;
    ASSERT_EQ(cref_node.value(), 6);
}
TEST(Node, edge) {
    graph_t graph;
    graph.insert_node(1);
    graph.insert_node(2);
    graph.insert_edge({ 1, 2 }, 12);
    node_t& node = graph.find(1)->second;
    auto& edge = node.edge();
    ASSERT_EQ(edge.begin()->second, 12);
    const auto& const_edge = node.edge();
    ASSERT_EQ(const_edge.begin()->second, 12);
}
