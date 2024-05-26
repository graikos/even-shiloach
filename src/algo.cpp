#include "algo.hpp"
#include <queue>
#include <iostream>

using namespace boost;

void my::bfs(const Graph &G, Vertex s, std::vector<int> &levels, std::vector<int> &comp, int comp_val, int levels_offset)
{
    std::queue<Vertex> q;
    levels[s] = levels_offset;
    comp[s] = comp_val;
    q.push(s);

    while (!q.empty())
    {
        Vertex u = q.front();
        q.pop();
        OutEdgeIterator ei, eiend;
        std::cout << "before for" << std::endl;
        for (tie(ei, eiend) = out_edges(u, G); ei != eiend; ++ei)
        {
            std::cout << *ei << " " << *eiend << std::endl;
            Vertex v = target(*ei, G);
            if (levels[v] < 0)
            {
                q.push(v);
                levels[v] = levels[u] + 1;
                comp[v] = comp_val;
            }
        }
    }
}