#include <iostream>
#include <cassert>
#include <ctime>
#include <queue>
#include "graph.hpp"
#include <vector>
#include "algo.hpp"
#include "dyn_graph.hpp"
#include <boost/random/mersenne_twister.hpp>
#include "gen.hpp"
#include "util.hpp"

#include <chrono>
#include <ctime>
#include <ratio>

#define ITERATIONS 100

using namespace boost;

std::vector<std::vector<std::vector<double>>> bench_line_q_queries(std::vector<int> &cases)
{
    // Cases X Queries X 2
    std::vector<std::vector<std::vector<double>>> res(cases.size());

    for (int c = 0; c < cases.size(); ++c)
    {

        // assert that vertices are a number of two
        // this will get the worst case complexity
        std::cout << cases[c] << std::endl;
        assert((cases[c] > 0) && ((cases[c] & (cases[c] - 1)) == 0));

        // for the line graph benchmark, all edges will be removed
        auto query_num = cases[c] - 1;
        // will keep avg time for each query
        res[c] = std::vector<std::vector<double>>(query_num, std::vector<double>(2, 0.0));

        for (int iter = 0; iter < ITERATIONS; ++iter)
        {
            Graph G;
            std::vector<Edge> edge_handles;
            gen::generate_line(G, cases[c], edge_handles);
            DynGraph DG(G);

            // this will halve the line every time, making it
            // the worst case for process A
            std::queue<Vertex> vq;
            int level = cases[c] / 4;
            int limit = 1;
            int limit_counter = 0;
            vq.push(cases[c] / 2);
            for (int q = 0; q < query_num; ++q)
            {
                int current = vq.front();
                vq.pop();

                vq.push(current - level);
                vq.push(current + level);

                ++limit_counter;
                if (limit_counter == limit)
                {
                    level /= 2;
                    limit *= 2;
                    limit_counter = 0;
                }

                std::chrono::duration<double, std::milli> dur(0);
                Edge e = edge(current, current - 1, G).first;
                auto t1 = std::chrono::high_resolution_clock::now();
                // now remove the edge
                DG.dyn_remove_edge(e);
                auto t2 = std::chrono::high_resolution_clock::now();
                // save time elapsed to dynamically restructure
                res[c][q][0] += std::chrono::duration<double, std::milli>(t2 - t1).count();
                bool dfs_found = false;
                t1 = std::chrono::high_resolution_clock::now();
                dfs_found = my::dfs_scan(G, source(e, G), target(e, G));
                t2 = std::chrono::high_resolution_clock::now();
                res[c][q][1] += std::chrono::duration<double, std::milli>(t2 - t1).count();
                // make sure correct results are returned
                assert((DG.query_is_connected(source(e, G), target(e, G)) == dfs_found));
            }
        }

        // take avg per query over iterations
        for (int q = 0; q < query_num; ++q)
        {
            res[c][q][0] /= ITERATIONS;
            res[c][q][1] /= ITERATIONS;
        }
    }

    return res;
}

int main()
{
    std::vector<int> cases = {128};
    auto res = bench_line_q_queries(cases);
    save_bench_to_file("../results/line_bench", res, cases);
    return 0;
}
