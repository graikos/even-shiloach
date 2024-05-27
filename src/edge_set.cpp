#include "edge_set.hpp"
#include <iostream>

void EdgeSet::add_edge(Vertex u, Vertex v)
{
    _internal_map[(u < v) ? u : v] = (u >= v) ? u : v;
}

bool EdgeSet::remove_edge(Vertex u, Vertex v)
{
    auto k = _internal_map.find((u < v) ? u : v);
    if (k == _internal_map.end())
    {
        return false;
    }

    _internal_map.erase(k);
    return true;
}

bool EdgeSet::empty()
{
    return _internal_map.empty();
}

EdgeSet &EdgeSet::operator=(EdgeSet &&other)
{
    // don't move to self
    if (this == &other)
    {
        return *this;
    }
    _internal_map.clear();
    // transfer the internal map
    _internal_map = std::move(other._internal_map);
    // clear the map of the other edge set, essentially leaving it empty again
    other._internal_map.clear();
    return *this;
}

void EdgeSet::print()
{
    for (auto k = _internal_map.begin(); k != _internal_map.end(); ++k)
    {
        std::cout << "("<< k->first << "," << k->second << ")" << "\t";
    }
    std::cout << std::endl;
}