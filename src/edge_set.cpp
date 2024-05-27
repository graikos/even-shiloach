#include "edge_set.hpp"
#include <iostream>

void EdgeSet::add_edge(Vertex u, Vertex v)
{
    Vertex mn = (u < v) ? u : v;
    Vertex mx = (u >= v) ? u : v;
    _internal_set.insert(std::make_pair(mn, mx));
}

bool EdgeSet::remove_edge(Vertex u, Vertex v)
{
    Vertex mn = (u < v) ? u : v;
    Vertex mx = (u >= v) ? u : v;

    auto k = _internal_set.find(std::make_pair(mn,mx));
    if (k == _internal_set.end())
    {
        return false;
    }

    _internal_set.erase(k);
    return true;
}

bool EdgeSet::empty()
{
    return _internal_set.empty();
}

EdgeSet &EdgeSet::operator=(EdgeSet &&other)
{
    // don't move to self
    if (this == &other)
    {
        return *this;
    }
    _internal_set.clear();
    // transfer the internal map
    _internal_set = std::move(other._internal_set);
    // clear the map of the other edge set, essentially leaving it empty again
    other._internal_set.clear();
    return *this;
}

void EdgeSet::print()
{
    for (auto k = _internal_set.begin(); k != _internal_set.end(); ++k)
    {
        std::cout << "("<< k->first << "," << k->second << ")" << "\t";
    }
    std::cout << std::endl;
}