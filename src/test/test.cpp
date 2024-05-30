#include <iostream>
#include <ctime>
#include "graph.hpp"
#include <cassert>
#include <vector>
#include "algo.hpp"
#include "dyn_graph.hpp"
#include <boost/random/mersenne_twister.hpp>
#include <boost/graph/random.hpp>
#include "gen.hpp"
#include "util.hpp"
#define MAX_RANDOM_NODES 8000

using namespace boost;

void test_ring(mt19937 &mt)
{
    Graph G;
    std::vector<Edge> edge_handles;
    auto num_of_nodes = mt() % MAX_RANDOM_NODES;
    std::cout << "Testing ring with " << num_of_nodes << " nodes." << std::endl;
    gen::generate_ring(G, num_of_nodes, edge_handles);

    DynGraph DG(G);
    // pick random first edge to remove
    Edge first = random_edge(G, mt);
    Vertex firstv = source(first, G);
    Vertex firstu = target(first, G);

    // remove random first edge
    DG.dyn_remove_edge(first);

    // first edge should not break connected component
    assert(DG.query_is_connected(first) && "First edge removed broke component.");

    for (auto it = edge_handles.begin(); it != edge_handles.end(); ++it)
    {
        if ((source(*it, G) == firstv && target(*it, G) == firstu) || source(*it, G) == firstu && target(*it, G) == firstv)
        {
            // skip the already removed one
            continue;
        }
        DG.dyn_remove_edge(*it);
        assert(!DG.query_is_connected(source(*it, G), target(*it, G)) && "Every subsequent edge removal should break a component.");
    }
}

void test_line(mt19937 &mt)
{
    Graph G;
    std::vector<Edge> edge_handles;
    auto num_of_nodes = mt() % MAX_RANDOM_NODES;
    std::cout << "Testing line with " << num_of_nodes << " nodes." << std::endl;
    gen::generate_line(G, num_of_nodes, edge_handles);
    DynGraph DG(G);

    for (auto it = edge_handles.begin(); it != edge_handles.end(); ++it)
    {
        DG.dyn_remove_edge(*it);
        assert(!DG.query_is_connected(source(*it, G), target(*it, G)) && "Every subsequent edge removal should break a component.");
    }
}

int main()
{
    mt19937 mt(time(0));
    test_ring(mt);
    test_line(mt);
    return 0;
}
