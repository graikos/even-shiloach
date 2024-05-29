#include "gen.hpp"
#include <boost/graph/random.hpp>
#include <cassert>

using namespace boost;

void gen::generate_ring(Graph &G, int n, std::vector<Edge> &edge_handles)
{
    assert(n > 0);
    add_vertex(G);
    Edge e;
    bool c;
    for (int i = 1; i < n; ++i)
    {
        tie(e, c) = add_edge(i, i - 1, G);
        edge_handles.push_back(e);
    }
    tie(e, c) = add_edge(0, n - 1, G);
    edge_handles.push_back(e);
}

void gen::generate_random(Graph &G, int n, int m, std::vector<Edge> &edge_handles, mt19937 &mt)
{
    assert(n > 0);
    boost::generate_random_graph(G, n, m, mt, false, false);
    EdgeIterator ei, eiend;
    for (tie(ei, eiend) = edges(G); ei != eiend; ++ei)
    {
        edge_handles.push_back(*ei);
    }
}

void gen::generate_line(Graph &G, int n, std::vector<Edge> &edge_handles)
{
    assert(n > 0);
    add_vertex(G);
    Edge e;
    bool c;
    for (int i = 1; i < n; ++i)
    {
        tie(e, c) = add_edge(i, i - 1, G);
        edge_handles.push_back(e);
    }
}