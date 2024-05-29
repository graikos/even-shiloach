#include "graph.hpp"
#include <vector>
#include <boost/random/mersenne_twister.hpp>

namespace gen
{
    void generate_ring(Graph &G, int n, std::vector<Edge> &edge_handles);
    void generate_random(Graph &G, int n, int m, std::vector<Edge> &edge_handles, boost::mt19937 &mt);
}