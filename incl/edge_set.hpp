#ifndef EDGE_SET_HPP
#define EDGE_SET_HPP

#include "graph.hpp"
#include <unordered_set>
#include <utility>
#include <boost/functional/hash.hpp>

struct vertex_pair_hash
{
    std::size_t operator()(const std::pair<Vertex, Vertex> &p) const
    {
        std::size_t seed = 0;
        // NOTE: (x,y) will hash to a different value than (y,x);
        boost::hash_combine(seed, p.first);
        boost::hash_combine(seed, p.second);
        return seed;
    }
};

typedef std::unordered_set<std::pair<Vertex, Vertex>, vertex_pair_hash>::iterator EdgeSetIterator;

class EdgeSet
{
public:
    EdgeSet() = default;

    EdgeSet &operator=(EdgeSet &&other);
    EdgeSet(EdgeSet &&other);

    void add_edge(Vertex u, Vertex v);
    bool remove_edge(Vertex u, Vertex v);
    bool empty();
    Vertex other_end(EdgeSetIterator it, Vertex v);
    EdgeSetIterator begin();
    EdgeSetIterator end();
    void print();

private:
    std::unordered_set<std::pair<Vertex, Vertex>, vertex_pair_hash> _internal_set;
};

#endif