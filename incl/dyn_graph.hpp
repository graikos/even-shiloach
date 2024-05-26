#ifndef DYN_GRAPH_HPP
#define DYN_GRAPH_HPP

#include "graph.hpp"


class DynGraph
{
public:

    std::vector<int> _levels;
    std::vector<int> _components;
    std::unordered_map<Vertex, Vertex> _artificial_edges; // TODO: remove

    DynGraph(Graph &G);
    void init();
    void print();

private:
    Graph &_G;
    Vertex _r;
    // std::unordered_map<Vertex, Vertex> _artificial_edges;
    int _component_max_idx;

    void _add_artificial_edge(Vertex v, Vertex u);
    void _remove_artificial_edge(Vertex v, Vertex u);
    bool _check_if_artificially_connected(Vertex v, Vertex u);
};

#endif