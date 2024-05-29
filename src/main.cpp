#include <iostream>
#include "graph.hpp"
#include <vector>
#include "algo.hpp"
#include "dyn_graph.hpp"
#include "gen.hpp"

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
    tie(e, c) = boost::add_edge(u,v,G);
    return e;
}

int main()
{
    Graph G;
    std::vector<Edge> edge_handles;
    gen::generate_line(G, 10, edge_handles);


    DynGraph DG(G);
    DG.dyn_remove_edge(edge_handles[4]);
    std::cout << "Levels:" << std::endl;
    printVector(DG._levels);
    std::cout << "Components:" << std::endl;
    printVector(DG._components);

    return 0;
}
