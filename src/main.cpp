#include <iostream>
#include <cassert>
#include <ctime>
#include <queue>
#include "graph.hpp"
#include <vector>
#include "algo.hpp"
#include "dyn_graph.hpp"
#include <boost/random/mersenne_twister.hpp>
#include <boost/graph/random.hpp>
#include "gen.hpp"
#include "util.hpp"

#include <chrono>
#include <ctime>
#include <ratio>

#define ITERATIONS 100
#define NUM_OF_RANDOM 25

using namespace boost;

std::vector<std::vector<std::vector<double>>> bench_line_q_queries(std::vector<int> &cases)
{
    // Cases X Queries X 2
    std::vector<std::vector<std::vector<double>>> res(cases.size());

    for (int c = 0; c < cases.size(); ++c)
    {

        // assert that vertices are a power of two
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

std::vector<std::vector<std::vector<double>>> bench_line_q_random_queries(std::vector<int> &cases)
{
    // Cases X Queries X 2
    std::vector<std::vector<std::vector<double>>> res(cases.size());

    for (int c = 0; c < cases.size(); ++c)
    {

        // assert that vertices are a power of two
        // this will get the worst case complexity
        std::cout << cases[c] << std::endl;
        assert(cases[c] > 0);

        // for the line graph benchmark, all edges will be removed
        auto query_num = cases[c] - 1;
        // will keep avg time for each query
        res[c] = std::vector<std::vector<double>>(query_num, std::vector<double>(2, 0.0));

        mt19937 mt(time(0));

        for (int iter = 0; iter < ITERATIONS; ++iter)
        {
            Graph G;
            std::vector<Edge> edge_handles;
            gen::generate_line(G, cases[c], edge_handles);
            DynGraph DG(G);

            for (int q = 0; q < query_num; ++q)
            {

                std::chrono::duration<double, std::milli> dur(0);
                Edge e = random_edge(G, mt);
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

std::vector<std::vector<std::vector<double>>> bench_ring_q_random_queries(std::vector<int> &cases)
{
    // Cases X Queries X 2
    std::vector<std::vector<std::vector<double>>> res(cases.size());

    for (int c = 0; c < cases.size(); ++c)
    {

        // assert that vertices are a number of two
        // this will get the worst case complexity
        std::cout << cases[c] << std::endl;
        assert(cases[c] > 0);

        // for the line graph benchmark, all edges will be removed
        auto query_num = cases[c] - 1;
        // will keep avg time for each query
        res[c] = std::vector<std::vector<double>>(query_num, std::vector<double>(2, 0.0));

        mt19937 mt(time(0));

        for (int iter = 0; iter < ITERATIONS; ++iter)
        {
            Graph G;
            std::vector<Edge> edge_handles;
            gen::generate_ring(G, cases[c], edge_handles);
            DynGraph DG(G);

            for (int q = 0; q < query_num; ++q)
            {

                std::chrono::duration<double, std::milli> dur(0);
                Edge e = random_edge(G, mt);
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

std::vector<std::vector<std::vector<double>>> bench_random_q_queries(std::vector<std::pair<int, int>> &cases)
{
    // Cases X Queries X 2
    std::vector<std::vector<std::vector<double>>> res(cases.size());

    for (int c = 0; c < cases.size(); ++c)
    {

        // this will get the worst case complexity
        std::cout << cases[c].first << " " << cases[c].second << std::endl;

        // for the line graph benchmark, all edges will be removed
        auto query_num = cases[c].second - 1;
        // will keep avg time for each query
        res[c] = std::vector<std::vector<double>>(query_num, std::vector<double>(2, 0.0));

        mt19937 mt(time(0));

        for (int iter = 0; iter < ITERATIONS; ++iter)
        {
            Graph G;
            std::vector<Edge> edge_handles;
            gen::generate_random(G, cases[c].first, cases[c].second, edge_handles, mt);
            DynGraph DG(G);

            for (int q = 0; q < query_num; ++q)
            {
                std::chrono::duration<double, std::milli> dur(0);
                Edge e = random_edge(G, mt);
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

std::vector<std::vector<std::vector<double>>> bench_fully_connected_q_queries(std::vector<int> &cases)
{
    // Cases X Queries X 2
    std::vector<std::vector<std::vector<double>>> res(cases.size());

    for (int c = 0; c < cases.size(); ++c)
    {

        std::cout << cases[c] << std::endl;

        // for the line graph benchmark, all edges will be removed
        int query_num = cases[c] * (cases[c] - 1) / 2;
        // will keep avg time for each query
        res[c] = std::vector<std::vector<double>>(query_num, std::vector<double>(2, 0.0));

        mt19937 mt(time(0));

        for (int iter = 0; iter < ITERATIONS; ++iter)
        {
            Graph G;
            std::vector<Edge> edge_handles;
            gen::generate_fully_connected(G, cases[c], edge_handles);
            DynGraph DG(G);

            for (int q = 0; q < query_num; ++q)
            {
                std::chrono::duration<double, std::milli> dur(0);
                Edge e = random_edge(G, mt);
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

std::vector<double> bench_worst_process_a(std::vector<int> &cases)
{
    std::vector<double> res(cases.size(), 0.0);

    for (int c = 0; c < cases.size(); ++c)
    {

        // assert that vertices are a power of two
        // this will get the worst case complexity
        std::cout << cases[c] << std::endl;
        assert((cases[c] > 0) && ((cases[c] & (cases[c] - 1)) == 0));

        // for the line graph benchmark, all edges will be removed
        auto query_num = cases[c] - 1;

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
                
                // save total time for current case
                res[c] += std::chrono::duration<double, std::milli>(t2 - t1).count();
            }
        }

        // take avg over iterations
        res[c] /= ITERATIONS;
    }

    return res;
}

std::vector<double> bench_worst_process_b(std::vector<int> &cases)
{
    std::vector<double> res(cases.size(), 0.0);

    for (int c = 0; c < cases.size(); ++c)
    {

        std::cout << cases[c] << std::endl;
        assert(cases[c] > 0);

        for (int iter = 0; iter < ITERATIONS; ++iter)
        {
            // construct new ring graph for every iteration
            Graph G;
            std::vector<Edge> edge_handles;
            gen::generate_ring(G, cases[c], edge_handles);
            DynGraph DG(G);

            // will only remove one edge to cause worst-case restructuring with process B
            std::chrono::duration<double, std::milli> dur(0);
            // remove the edge before the random root to cause worst case scenario
            Edge e = edge(DG.get_root(), (DG.get_root() - 1) % cases[c], G).first;
            auto t1 = std::chrono::high_resolution_clock::now();
            // now remove the edge
            DG.dyn_remove_edge(e);
            auto t2 = std::chrono::high_resolution_clock::now();
            
            // save total time for current case
            res[c] += std::chrono::duration<double, std::milli>(t2 - t1).count();
        }

        // take avg over iterations
        res[c] /= ITERATIONS;
    }

    return res;
}


int main()
{
    // std::vector<int> cases = {128, 256, 512, 1024, 2048, };
    std::vector<int> cases = powers_of_two(10, 4);
    // auto res = bench_line_q_queries(cases);
    // auto res = bench_line_q_random_queries(cases);
    // auto res = bench_random_q_queries(cases);
    // auto res = bench_fully_connected_q_queries(cases);
    // auto res = bench_ring_q_random_queries(cases);
    auto res = bench_worst_process_b(cases);
    save_worst_case_to_file("../results/worst_case_process_b_bench", res, cases);
    return 0;
}
