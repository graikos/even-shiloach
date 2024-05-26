#ifndef ALGO_HPP
#define ALGO_HPP

#include "graph.hpp"

namespace my
{
    void bfs(const Graph &G, Vertex s, std::vector<int> &levels, std::vector<int> &comp, int comp_val, int levels_offset = 0);
}

#endif