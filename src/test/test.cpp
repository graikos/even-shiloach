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
#define MAX_RANDOM_NODES 15000

using namespace boost;

void test_ring(mt19937 &mt)
{
    Graph G;
    std::vector<Edge> edge_handles;
    gen::generate_ring(G, 8500, edge_handles);
    std::cout << "just made it" << std::endl;

    DynGraph DG(G);
    // pick random first edge to remove
    Edge first = random_edge(G, mt);
    Vertex firstv = source(first, G);
    Vertex firstu = target(first, G);

    // TODO: possible bad_alloc because of too many changes being saved in the stack
    // especially for the first one that needs to pass all of them
    // TODO: maybe stop recording once we know that process A has stopped so no rewinding
    // is gonna take place
    std::cout << "Removing first" << std::endl;
    // remove random first edge
    DG.dyn_remove_edge(first);
    std::cout << "After remove first" << std::endl;

    // first edge should not break connected component
    assert(DG.query_is_connected(first) && "First edge removed broke component.");

    EdgeIterator ei, eiend;
    for (auto it = edge_handles.begin(); it != edge_handles.end(); ++it)
    {
        if ((source(*it, G) == firstv && target(*it, G) == firstu) || source(*it, G) == firstu && target(*it, G) == firstv)
        {
            // skip the already removed one
            continue;
        }
        DG.dyn_remove_edge(*it);
        assert(!DG.query_is_connected(source(*it, G), target(*it, G)));
    }
}

int main()
{
    mt19937 mt(time(0));
    std::cout << "Hello test" << std::endl;
    test_ring(mt);
    return 0;
}
