#ifndef MY_UTIL_HPP
#define MY_UTIL_HPP

#include "graph.hpp"
#include <vector>
#include <list>

using namespace boost;

void printGraph(Graph &G)
{
    VertexIterator vi, viend;
    for (tie(vi, viend) = vertices(G); vi != viend; ++vi)
    {
        std::cout << "Vertex " << *vi << " : ";
        OutEdgeIterator oei, oeiend;
        for (tie(oei, oeiend) = out_edges(*vi, G); oei != oeiend; ++oei)
        {
            std::cout << "Edge: " << *oei << " ";
        }
        std::cout << std::endl;
    }
}

template <typename T>
void printVector(const std::vector<T> &vec)
{
    for (auto it = vec.begin(); it != vec.end(); ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}

template <typename T>
void printList(const std::list<T> &list)
{
    for (auto it = list.begin(); it != list.end(); ++it)
    {
        std::cout << *it << std::endl;
    }
}

Edge add_edge(Vertex u, Vertex v, Graph &G)
{
    Edge e;
    bool c;
    tie(e, c) = boost::add_edge(u, v, G);
    return e;
}

#endif
