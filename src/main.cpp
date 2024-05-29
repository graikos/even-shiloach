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
    gen::generate_ring(G, 8, edge_handles);


    DynGraph DG(G);
    DG.print();

    printVector(DG._levels);
    std::cout << std::endl;
    printVector(DG._components);
    std::cout << std::endl;
    std::cout << "Edges" << std::endl;
    printVector(edge_handles);

    DG.dyn_remove_edge(edge_handles[0]);
    printVector(DG._levels);
    std::cout << std::endl;
    printVector(DG._components);
    std::cout << std::endl;
    DG.dyn_remove_edge(edge_handles[1]);
    printVector(DG._components);
    std::cout << "Levels: " << std::endl;
    printVector(DG._levels);
    std::cout << std::endl;
    DG.dyn_remove_edge(edge_handles[3]);
    printVector(DG._components);
    // DG.dyn_remove_edge(edge_handles[2]);
    // printVector(DG._components);

    std::cout << "exit" << std::endl;


    return 0;
}
