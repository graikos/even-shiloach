#include "graph.hpp"
#include <ctime>
#include <queue>
#include <iostream>
#include <boost/random/mersenne_twister.hpp>

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

DynGraph::DynGraph(Graph &G) : _G(G), _component_max_idx(0)
{
    init();
}

void DynGraph::init()
{

    _levels = std::vector<int>(num_vertices(_G), -1);
    _components = std::vector<int>(num_vertices(_G), -1);

    // pick random vertex as root
    mt19937 mt(std::time(0));
    _r = vertex(mt() % num_vertices(_G), _G);
    std::cout << "Picked as root: " << _r << std::endl;

    // perform initial BFS from root r
    my::bfs(_G, _r, _levels, _components, _component_max_idx);

    for (std::size_t i = 0; i < _levels.size(); ++i)
    {
        Vertex s = vertex(i, _G);
        // if current vertex is not part of an existing connected component, it has not been discovered
        if (_levels[i] < 0)
        {
            my::bfs(_G, s, _levels, _components, ++_component_max_idx, 1);
        }
    }
}

void DynGraph::print()
{
    VertexIterator vi, viend;
    for (tie(vi, viend) = vertices(_G); vi != viend; ++vi)
    {
        std::cout << "Vertex " << *vi << " : ";
        OutEdgeIterator oei, oeiend;
        for (tie(oei, oeiend) = out_edges(*vi, _G); oei != oeiend; ++oei)
        {
            std::cout << "Edge: " << *oei << " ";
        }
        std::cout << std::endl;
    }
}