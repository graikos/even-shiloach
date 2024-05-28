#ifndef DYN_GRAPH_HPP
#define DYN_GRAPH_HPP

#include "graph.hpp"
#include "edge_set.hpp"
#include <stack>
#include "algo.hpp"


class DynGraph
{
public:
    std::vector<int> _levels;
    std::vector<int> _components;
    std::unordered_map<Vertex, Vertex> _artificial_edges; // TODO: move to private
    std::vector<EdgeSet> alpha;                           // TODO: move to private
    std::vector<EdgeSet> beta;                            // TODO: move to private
    std::vector<EdgeSet> gamma;                           // TODO: move to private

    DynGraph(Graph &G);
    void init();
    void print();

private:
    Graph &_G;
    Vertex _r;
    // std::unordered_map<Vertex, Vertex> _artificial_edges;
    int _component_max_idx;
    std::stack<my::ChangeRecord> _change_history;

    void _add_artificial_edge(Vertex v, Vertex u);
    void _remove_artificial_edge(Vertex v, Vertex u);
    void _rewind();
    bool _check_if_artificially_connected(Vertex v, Vertex u);
};

#endif