#include "graph.hpp"
#include <ctime>
#include <queue>
#include <iostream>
#include <boost/random/mersenne_twister.hpp>
#include "dyn_graph.hpp"
#include "algo.hpp"

using namespace boost;

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
            // the root of this new component will be artificially connected to the random root
            // will be on level 1 and an artificial edge will be added
            my::bfs(_G, s, _levels, _components, ++_component_max_idx, 1);
            _add_artificial_edge(_r, i);
        }
    }
}

void DynGraph::_add_artificial_edge(Vertex v, Vertex u)
{
    // artificial edges will be kept in a hash map, with key the min of the two and value the max of the two
    std::cout << "Adding" << std::endl;
    _artificial_edges[(v < u) ? v : u] = (v >= u) ? v : u;
}

void DynGraph::_remove_artificial_edge(Vertex v, Vertex u)
{
    auto k = _artificial_edges.find((v < u) ? v : u);
    if (k != _artificial_edges.end())
    {
        _artificial_edges.erase(k);
    }
}

bool DynGraph::_check_if_artificially_connected(Vertex v, Vertex u)
{
    try
    {
        _artificial_edges.at((v < u) ? v : u);
        return true;
    }
    catch (const std::out_of_range &e)
    {
        return false;
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