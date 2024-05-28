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

int main()
{

    // Graph G;
    // add_vertex(G);
    // add_vertex(G);
    // add_vertex(G);
    // add_vertex(G);
    // add_vertex(G);
    // add_vertex(G);
    // add_edge(0, 1, G);
    // add_edge(0, 2, G);
    // add_edge(2, 3, G);
    // add_edge(2, 4, G);
    // add_edge(4, 5, G);
    // add_vertex(G);
    // add_vertex(G);
    // add_edge(7, 6, G);

    Graph G;
    add_vertex(G);
    add_vertex(G);
    add_vertex(G);
    add_vertex(G);
    Edge e;
    bool created;
    tie(e, created) = add_edge(0, 1, G);
    add_edge(1, 2, G);
    add_edge(2, 3, G);
    add_edge(3, 0, G);

    add_vertex(G);
    add_vertex(G);
    tie(e, created) = add_edge(4, 5, G);

    DynGraph DG(G);
    DG.print();

    printVector(DG._levels);
    std::cout << std::endl;
    printVector(DG._components);

    std::stack<my::ChangeRecord> stacky;

    std::cout << "Artificial Edges: " << std::endl;
    for (auto k = DG._artificial_edges.begin(); k != DG._artificial_edges.end(); ++k)
    {
        std::cout << k->first << " " << k->second << "\t";
    }
    std::cout << std::endl;

    std::cout << "Printing alpha: " << std::endl;
    for (auto i = 0; i < DG.alpha.size(); ++i)
    {
        std::cout << "Vertex: " << i << std::endl;
        DG.alpha[i].print();
    }

    std::cout << "Printing beta: " << std::endl;
    for (auto i = 0; i < DG.beta.size(); ++i)
    {
        std::cout << "Vertex: " << i << std::endl;
        DG.beta[i].print();
    }

    std::cout << "Printing gamma: " << std::endl;
    for (auto i = 0; i < DG.gamma.size(); ++i)
    {
        std::cout << "Vertex: " << i << std::endl;
        DG.gamma[i].print();
    }

    remove_edge(e, G);
    my::StepDetectNotBreak sdnb(DG._levels, DG.alpha, DG.beta, DG.gamma, stacky, source(e, G), target(e, G));

    std::cout << "===============" << std::endl;

    while (sdnb.state != my::StepDetectNotBreakState::Finished)
    {
        sdnb.advance();
    }

    std::cout << "component breaks?: " << sdnb.component_breaks << std::endl;
    std::cout << "===============" << std::endl;

    std::cout << "levels: " << std::endl;
    printVector(DG._levels);

    // my::StepScanDFS sdfs(G, 6, 7);
    // while (sdfs.state != my::StepScanState::Finished)
    // {
    //     sdfs.advance();
    // }
    // std::cout << "result: " << sdfs.result << std::endl;
    // printList(sdfs.component);

    // my::StepDetectBreak sd(G, 5, 7);

    // while (sd.state != my::StepDetectBreakState::Finished)
    // {
    //     sd.advance();
    // }
    // std::cout << "Component breaks? " <<  sd.component_breaks << std::endl;
    // printList(sd.small_component);

    return 0;
}