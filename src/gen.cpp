#include "gen.hpp"
#include <boost/graph/random.hpp>
#include <cassert>

using namespace boost;

void gen::generate_ring(Graph &G, int n, std::vector<Edge> &edge_handles)
{
    assert(n > 0);
    add_vertex(G);
    for (int i = 1; i < n; ++i)
    {
        edge_handles.push_back(add_edge(i, i - 1, G).first);
    }
    edge_handles.push_back(add_edge(0, n - 1, G).first);
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
    for (int i = 1; i < n; ++i)
    {
        edge_handles.push_back(add_edge(i, i - 1, G).first);
    }
}

void gen::generate_fully_connected(Graph &G, int n, std::vector<Edge> &edge_handles)
{
    assert(n > 0);
    for (int i = 0; i < n; ++i)
    {
        add_vertex(G);
    }
    for (int i = 0; i < n; ++i)
    {
        for (int j = i + 1; j < n; ++j)
        {
            edge_handles.push_back(add_edge(i, j, G).first);
        }
    }
}