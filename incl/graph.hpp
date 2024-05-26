#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <boost/graph/adjacency_list.hpp>
#include <unordered_map>
#include <vector>

/* Graph (https://www.boost.org/doc/libs/1_61_0/libs/graph/doc/graph_concepts.html) */
typedef boost::adjacency_list<boost::listS, boost::vecS, boost::undirectedS, boost::no_property, boost::no_property> Graph;

/* Vertex descriptor */
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
/* Edge descriptor */
typedef boost::graph_traits<Graph>::edge_descriptor Edge;

/* Iterators */
typedef boost::graph_traits<Graph>::vertex_iterator VertexIterator;
typedef boost::graph_traits<Graph>::edge_iterator EdgeIterator;
typedef boost::graph_traits<Graph>::out_edge_iterator OutEdgeIterator;
typedef boost::graph_traits<Graph>::in_edge_iterator InEdgeIterator;

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

namespace my
{
    void bfs(const Graph &G, Vertex s, std::vector<int> &levels, std::vector<int> &comp, int comp_val, int levels_offset = 0);
}

#endif