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

#endif