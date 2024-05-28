#include "gen.hpp"

using namespace boost;

void generate_ring(Graph &G, int n, std::vector<Edge> &edge_handles)
{
    add_vertex(G);
    Edge e;
    bool c;
    for (int i = 1; i < n; ++i)
    {
        tie(e, c) = add_edge(i, i - 1, G);
        edge_handles.push_back(e);
    }
    tie(e, c) = add_edge(0, n - 1, G);
}