#ifndef EDGE_SET_HPP
#define EDGE_SET_HPP

#include "graph.hpp"
#include <unordered_map>

class EdgeSet
{
public:
    EdgeSet() = default;

    EdgeSet &operator=(EdgeSet &&other);

    void add_edge(Vertex u, Vertex v);
    bool remove_edge(Vertex u, Vertex v);
    bool empty();

    void print();

private:
    std::unordered_map<Vertex, Vertex> _internal_map;
};

#endif