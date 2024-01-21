#pragma once

#include <unordered_map>
#include <stdexcept>

namespace graph {

    struct GraphException : public std::runtime_error {
        using runtime_error::runtime_error;
    };

    template<typename key_type, typename value_type, typename weight_type>
    class Graph {
    public:
        class Node;
        using const_iterator = typename std::unordered_map<key_type, Node>::const_iterator;
        using iterator = typename std::unordered_map<key_type, Node>::iterator;
        using edge_iterator = typename Node::iterator;
        using const_edge_iterator = typename Node::const_iterator;
        using node_type = Node;

        const_iterator cbegin() const noexcept { return m_map.cbegin(); }
        const_iterator cend() const noexcept { return m_map.cend(); }
        iterator begin() noexcept { return m_map.begin(); }
        iterator end() noexcept { return m_map.end(); }
        const_iterator begin() const noexcept { return m_map.begin(); }
        const_iterator end() const noexcept { return m_map.end(); }

        bool empty() const noexcept { return m_map.empty(); }
        size_t size() const noexcept { return m_map.size(); }
        void clear() noexcept { m_map.clear(); }
        void swap(Graph<key_type, value_type, weight_type>& gr) { m_map.swap(gr.m_map); } // noexcept?

//        const Node& operator[](const key_type& key) const { return m_map[key]; }
        node_type& operator[](const key_type& key) noexcept { return m_map[key]; }
        const node_type& at(const key_type& key) const;
        node_type& at(const key_type& key);
        const_iterator find(const key_type& key) const { return m_map.find(key); }
        iterator find(const key_type& key) { return m_map.find(key); }

        size_t degree_out(const key_type& key) const;
        size_t degree_in(const key_type& key) const;
        bool loop(const key_type& key) const;

        std::pair<iterator, bool> insert_node(const key_type& key, const value_type& value);
        std::pair<iterator, bool> insert_node(const key_type& key);
        std::pair<iterator, bool> insert_or_assign_node(const key_type& key, const value_type& value);
        std::pair<edge_iterator, bool> insert_edge(const std::pair<key_type, key_type>& p, const weight_type&);
        std::pair<edge_iterator, bool>
        insert_or_assign_edge(const std::pair<key_type, key_type>& p, const weight_type&);

        size_t erase_node(const key_type& key) noexcept;
        iterator erase_node(const iterator& it_erase) noexcept;

    private:
        std::unordered_map<key_type, Node> m_map;
    };

    template<typename key_type, typename value_type, typename weight_type>
    void swap(Graph<key_type, value_type, weight_type>& gr1,
              Graph<key_type, value_type, weight_type>& gr2) { gr1.swap(gr2); }
}

template<typename key_type, typename value_type, typename weight_type>
class graph::Graph<key_type, value_type, weight_type>::Node {
public:
    Node() = default;
    explicit Node(value_type value) : m_value(value) {};

    using const_iterator = typename std::unordered_map<key_type, weight_type>::const_iterator;
    using iterator = typename std::unordered_map<key_type, weight_type>::iterator;
    using edge_type = typename std::unordered_map<key_type, weight_type>;

    const_iterator cbegin() const noexcept { return m_edge.cbegin(); }
    const_iterator cend() const noexcept { return m_edge.cend(); }
    iterator begin() noexcept { return m_edge.begin(); }
    iterator end() noexcept { return m_edge.end(); }
    const_iterator begin() const noexcept { return m_edge.begin(); }
    const_iterator end() const noexcept { return m_edge.end(); }

    bool empty() const noexcept { return m_edge.empty(); }
    size_t size() const noexcept { return m_edge.size(); }
    void clear() noexcept { m_edge.clear(); }

    const value_type& value() const noexcept { return m_value; }
    value_type& value() noexcept { return m_value; }
    const edge_type& edge() const noexcept { return m_edge; }
    edge_type& edge() noexcept { return m_edge; }

    size_t erase_edge(const key_type& key) noexcept { return m_edge.erase(key); };
    iterator erase_edge(const iterator& it) noexcept { return m_edge.erase(it); };

private:
    value_type m_value;
    edge_type m_edge;
};

template<typename key_type, typename value_type, typename weight_type>
size_t graph::Graph<key_type, value_type, weight_type>::erase_node(const key_type& key) noexcept {
    size_t temp = m_map.erase(key);
    for (auto& pair: *this)
        pair.second.erase_edge(key);
    return temp;
};

template<typename key_type, typename value_type, typename weight_type>
typename graph::Graph<key_type, value_type, weight_type>::iterator
graph::Graph<key_type, value_type, weight_type>::erase_node(const iterator& it_erase) noexcept {
    auto key = it_erase->first;
    auto temp = m_map.erase(it_erase);
    for (auto& pair: *this)
        pair.second.erase_edge(key);
    return temp;
};

template<typename key_type, typename value_type, typename weight_type>
std::pair<typename graph::Graph<key_type, value_type, weight_type>::iterator, bool>
graph::Graph<key_type, value_type, weight_type>::insert_node(const key_type& key, const value_type& value) {
    return m_map.insert({ key, Node{ value }});
}

template<typename key_type, typename value_type, typename weight_type>
std::pair<typename graph::Graph<key_type, value_type, weight_type>::iterator, bool>
graph::Graph<key_type, value_type, weight_type>::insert_node(const key_type& key) {
    return m_map.insert({ key, Node{}});
}

template<typename key_type, typename value_type, typename weight_type>
std::pair<typename graph::Graph<key_type, value_type, weight_type>::iterator, bool>
graph::Graph<key_type, value_type, weight_type>::insert_or_assign_node(const key_type& key, const value_type& value) {
    return m_map.insert_or_assign(key, Node{ value });
}

template<typename key_type, typename value_type, typename weight_type>
std::pair<typename graph::Graph<key_type, value_type, weight_type>::Node::iterator, bool>
graph::Graph<key_type, value_type, weight_type>::insert_edge(const std::pair<key_type, key_type>& p,
                                                             const weight_type& weight) {
    auto it_from = find(p.first);
    auto it_to = find(p.second);
    if (it_from == end() || it_to == end())
        throw GraphException("There is no key");
    return it_from->second.edge().insert({ p.second, weight });
}

template<typename key_type, typename value_type, typename weight_type>
std::pair<typename graph::Graph<key_type, value_type, weight_type>::Node::iterator, bool>
graph::Graph<key_type, value_type, weight_type>::insert_or_assign_edge(const std::pair<key_type, key_type>& p,
                                                                       const weight_type& weight) {
    auto it_from = find(p.first);
    auto it_to = find(p.second);
    if (it_from == end() || it_to == end())
        throw GraphException("There is no key");
    return it_from->second.edge().insert_or_assign(p.second, weight);
}

template<typename key_type, typename value_type, typename weight_type>
size_t graph::Graph<key_type, value_type, weight_type>::degree_in(const key_type& key) const {
    auto it = find(key);
    if (it == end())
        throw GraphException("There is no key");
    size_t degree_out_count = 0;
    for (auto const& pair: *this) {
        auto node = pair.second;
        auto edge_map = node.edge();
        if (edge_map.find(key) != edge_map.end()) ++degree_out_count;
    }
    return degree_out_count;
}

template<typename key_type, typename value_type, typename weight_type>
size_t graph::Graph<key_type, value_type, weight_type>::degree_out(const key_type& key) const {
    auto it = find(key);
    if (it == end())
        throw GraphException("There is no key");
    return it->second.size();
}


template<typename key_type, typename value_type, typename weight_type>
bool graph::Graph<key_type, value_type, weight_type>::loop(const key_type& key) const {
    auto it = find(key);
    if (it == end())
        throw GraphException("There is no key");
    auto node = find(key)->second;
    auto edge_map = node.edge();
    for (auto const& pair: edge_map)
        if (pair.first == key)
            return true;
    return false;
}

template<typename key_type, typename value_type, typename weight_type>
const typename graph::Graph<key_type, value_type, weight_type>::Node&
graph::Graph<key_type, value_type, weight_type>::at(const key_type& key) const {
    auto it = find(key);
    if (it == end())
        throw GraphException("There is no key");
    return it->second;
}

template<typename key_type, typename value_type, typename weight_type>
typename graph::Graph<key_type, value_type, weight_type>::Node&
graph::Graph<key_type, value_type, weight_type>::at(const key_type& key) {
    auto it = find(key);
    if (it == end())
        throw GraphException("There is no key");
    return it->second;
}

//template<typename key_type, typename value_type, typename weight_type>
//typename graph::Graph<key_type, value_type, weight_type>::Node&
//graph::Graph<key_type, value_type, weight_type>::at(const key_type& key) {
//    return const_cast<Graph<key_type, value_type, weight_type>::Node&>(
//            const_cast<const Graph<key_type, value_type, weight_type>*>(this)->at(key)
//    );
//}