#include <iostream>
#include "graph.hpp"
#include <vector>
#include "algo.hpp"
#include "dyn_graph.hpp"

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
        std::cout << *it << std::endl;
    }
}

template <typename T>
void printList(const std::list<T> &list)
{
    for (auto it = list.begin(); it != list.end(); ++it)
    {
        std::cout << *it << std::endl;
    }
}

int main()
{

    Graph G;
    add_vertex(G);
    add_vertex(G);
    add_vertex(G);
    add_vertex(G);
    add_vertex(G);
    add_vertex(G);
    add_edge(0, 1, G);
    add_edge(0, 2, G);
    add_edge(2, 3, G);
    add_edge(2, 4, G);
    add_edge(4, 5, G);
    add_vertex(G);
    add_vertex(G);
    add_edge(7, 6, G);

    // DynGraph DG(G);
    // DG.print();

    // printVector(DG._levels);
    // std::cout << std::endl;
    // printVector(DG._components);

    // for (auto k = DG._artificial_edges.begin(); k != DG._artificial_edges.end(); ++k)
    // {
    //     std::cout << k->first << " " << k->second << std::endl;
    // }

    // my::StepScanDFS sdfs(G, 6, 7);
    // while (sdfs.state != my::StepScanState::Finished)
    // {
    //     sdfs.advance();
    // }
    // std::cout << "result: " << sdfs.result << std::endl;
    // printList(sdfs.component);

    my::StepDetectBreak sd(G, 5, 7);

    while (sd.state != my::StepDetectBreakState::Finished)
    {
        sd.advance();
    }
    std::cout << "Component breaks? " <<  sd.component_breaks << std::endl;
    printList(sd.small_component);

    return 0;
}