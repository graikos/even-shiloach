#include "edge_set.hpp"
#include <iostream>

void EdgeSet::add_edge(Vertex u, Vertex v)
{
    Vertex mn = (u < v) ? u : v;
    Vertex mx = (u >= v) ? u : v;
    _internal_set.insert(std::make_pair(mn, mx));
}

bool EdgeSet::contains(Vertex u, Vertex v)
{
    Vertex mn = (u < v) ? u : v;
    Vertex mx = (u >= v) ? u : v;
    auto k = _internal_set.find(std::make_pair(mn, mx));
    return k != _internal_set.end();
}

bool EdgeSet::remove_edge(Vertex u, Vertex v)
{
    Vertex mn = (u < v) ? u : v;
    Vertex mx = (u >= v) ? u : v;

    auto k = _internal_set.find(std::make_pair(mn, mx));
    if (k == _internal_set.end())
    {
        return false;
    }

    if (!contains(u, v))
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

void EdgeSet::clear()
{
    return _internal_set.clear();
}

EdgeSet::EdgeSet(EdgeSet &&other) : _internal_set(std::move(other._internal_set))
{
}

EdgeSet &EdgeSet::operator=(EdgeSet &&other)
{
    // don't move to self
    if (this != &other)
    {
        _internal_set = std::move(other._internal_set);
    }
    return *this;
}

EdgeSetIterator EdgeSet::begin()
{
    return _internal_set.begin();
}

EdgeSetIterator EdgeSet::end()
{
    return _internal_set.end();
}

Vertex EdgeSet::other_end(EdgeSetIterator it, Vertex v)
{
    return (it->first == v) ? it->second : it->first;
}

void EdgeSet::print()
{
    for (auto k = _internal_set.begin(); k != _internal_set.end(); ++k)
    {
        std::cout << "(" << k->first << "," << k->second << ")" << "\t";
    }
    std::cout << std::endl;
}