#include <iostream>
#include <ctime>
#include "graph.hpp"
#include <vector>
#include "algo.hpp"
#include "dyn_graph.hpp"
#include <boost/random/mersenne_twister.hpp>
#include "gen.hpp"
#include "util.hpp"

using namespace boost;

int main()
{
    mt19937 mt(25);
    Graph G;
    std::vector<Edge> edge_handles;
    // gen::generate_line(G, 10, edge_handles);
    gen::generate_random(G, 10, 5, edge_handles, mt);


    DynGraph DG(G);

    std::cout << "Original levels" << std::endl;
    printVector(DG._levels);
    std::cout << "Original components" << std::endl;
    printVector(DG._components);

    Edge tbr;
    tbr = edge_handles[4];

    std::cout << "removing: " << tbr << std::endl;
    DG.dyn_remove_edge(tbr);
    std::cout << DG.query_is_connected(tbr) << std::endl;
    std::cout << "Levels:" << std::endl;
    printVector(DG._levels);
    std::cout << "Components:" << std::endl;
    printVector(DG._components);

    return 0;
}
