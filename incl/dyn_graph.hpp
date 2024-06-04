#ifndef DYN_GRAPH_HPP
#define DYN_GRAPH_HPP

#include "graph.hpp"
#include "edge_set.hpp"
#include <stack>
#include "algo.hpp"
#include "change_record.hpp"

class DynGraph
{
public:
    std::vector<int> _levels;
    std::vector<int> _components;
    std::vector<EdgeSet> alpha; // TODO: move to private
    std::vector<EdgeSet> beta;  // TODO: move to private
    std::vector<EdgeSet> gamma; // TODO: move to private

    DynGraph(Graph &G);
    DynGraph(Graph &G, Vertex r);
    void init(bool random_root);
    void print();
    void dyn_remove_edge(Edge e);
    bool query_is_connected(Vertex v, Vertex u);
    bool query_is_connected(Edge e);

private:
    Graph &_G;
    Vertex _r;
    int _component_max_idx;
    std::stack<ChangeRecord> _change_history;

    void _rewind();
};

#endif
