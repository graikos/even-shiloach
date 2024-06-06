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

#define ITERATIONS 50

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
                Vertex src = source(e, G);
                Vertex trgt = target(e, G);
                auto t1 = std::chrono::high_resolution_clock::now();
                // now remove the edge
                DG.dyn_remove_edge(e);
                auto t2 = std::chrono::high_resolution_clock::now();
                // save time elapsed to dynamically restructure
                res[c][q][0] += std::chrono::duration<double, std::milli>(t2 - t1).count();
                bool dfs_found = false;
                t1 = std::chrono::high_resolution_clock::now();
                dfs_found = my::dfs_scan(G, src, trgt);
                t2 = std::chrono::high_resolution_clock::now();
                res[c][q][1] += std::chrono::duration<double, std::milli>(t2 - t1).count();
                // make sure correct results are returned
                assert((DG.query_is_connected(src, trgt) == dfs_found));
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
                Vertex src = source(e, G);
                Vertex trgt = target(e, G);
                auto t1 = std::chrono::high_resolution_clock::now();
                // now remove the edge
                DG.dyn_remove_edge(e);
                auto t2 = std::chrono::high_resolution_clock::now();
                // save time elapsed to dynamically restructure
                res[c][q][0] += std::chrono::duration<double, std::milli>(t2 - t1).count();
                bool dfs_found = false;
                t1 = std::chrono::high_resolution_clock::now();
                dfs_found = my::dfs_scan(G, src, trgt);
                t2 = std::chrono::high_resolution_clock::now();
                res[c][q][1] += std::chrono::duration<double, std::milli>(t2 - t1).count();
                // make sure correct results are returned
                assert((DG.query_is_connected(src, trgt) == dfs_found));
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
                Vertex src = source(e, G);
                Vertex trgt = target(e, G);
                auto t1 = std::chrono::high_resolution_clock::now();
                // now remove the edge
                DG.dyn_remove_edge(e);
                auto t2 = std::chrono::high_resolution_clock::now();
                // save time elapsed to dynamically restructure
                res[c][q][0] += std::chrono::duration<double, std::milli>(t2 - t1).count();
                bool dfs_found = false;
                t1 = std::chrono::high_resolution_clock::now();
                dfs_found = my::dfs_scan(G, src, trgt);
                t2 = std::chrono::high_resolution_clock::now();
                res[c][q][1] += std::chrono::duration<double, std::milli>(t2 - t1).count();
                // make sure correct results are returned
                assert((DG.query_is_connected(src, trgt) == dfs_found));
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

        std::cout << cases[c].first << " " << cases[c].second << std::endl;

        // for the line graph benchmark, all edges will be removed
        auto query_num = cases[c].second - 1;
        // will keep avg time for each query
        res[c] = std::vector<std::vector<double>>(query_num, std::vector<double>(2, 0.0));

        auto seed = time(0) + c;
        std::cout << "seed: " << seed << std::endl;

        for (int iter = 0; iter < ITERATIONS; ++iter)
        {
            // each case will be one random graph
            // initializing a new mersenne twister RNG for every iteration with same seed to get same random graph
            mt19937 mt(seed);
            Graph G;
            std::vector<Edge> edge_handles;
            gen::generate_random(G, cases[c].first, cases[c].second, edge_handles, mt);
            DynGraph DG(G);

            mt19937 edge_mt(seed + 1072558);

            for (int q = 0; q < query_num; ++q)
            {
                std::chrono::duration<double, std::milli> dur(0);
                Edge e = random_edge(G, edge_mt);
                Vertex src = source(e, G);
                Vertex trgt = target(e, G);
                auto t1 = std::chrono::high_resolution_clock::now();
                // now remove the edge
                DG.dyn_remove_edge(e);
                auto t2 = std::chrono::high_resolution_clock::now();
                // save time elapsed to dynamically restructure
                res[c][q][0] += std::chrono::duration<double, std::milli>(t2 - t1).count();
                bool dfs_found = false;
                t1 = std::chrono::high_resolution_clock::now();
                dfs_found = my::dfs_scan(G, src, trgt);
                t2 = std::chrono::high_resolution_clock::now();
                res[c][q][1] += std::chrono::duration<double, std::milli>(t2 - t1).count();
                // make sure correct results are returned
                assert((DG.query_is_connected(src, trgt) == dfs_found));
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
                Vertex src = source(e, G);
                Vertex trgt = target(e, G);
                auto t1 = std::chrono::high_resolution_clock::now();
                // now remove the edge
                DG.dyn_remove_edge(e);
                auto t2 = std::chrono::high_resolution_clock::now();
                // save time elapsed to dynamically restructure
                res[c][q][0] += std::chrono::duration<double, std::milli>(t2 - t1).count();
                bool dfs_found = false;
                t1 = std::chrono::high_resolution_clock::now();
                dfs_found = my::dfs_scan(G, src, trgt);
                t2 = std::chrono::high_resolution_clock::now();
                res[c][q][1] += std::chrono::duration<double, std::milli>(t2 - t1).count();
                // make sure correct results are returned
                assert((DG.query_is_connected(src, trgt) == dfs_found));
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
    std::vector<std::pair<int, int>> random_cases = {
        {500, 500},
        {500, 2000},
        {1500, 3000},
        {3000, 10000},
        {10000, 500},
    };
    auto res_random = bench_random_q_queries(random_cases);
    save_bench_to_file("../results/bench_random_q_queries", res_random, random_cases);

    auto line_log_cases = powers_of_two(7, 256);
    auto res_line_log = bench_line_q_random_queries(line_log_cases);
    save_bench_to_file("../results/bench_line_q_queries", res_line_log, line_log_cases);

    auto line_random_cases = powers_of_two(4, 2048);
    auto res_line_random = bench_line_q_random_queries(line_random_cases);
    save_bench_to_file("../results/bench_line_q_random_queries", res_line_random, line_random_cases);

    std::vector<int> fully_conn_cases = {50, 100, 250};
    auto res_fully_conn = bench_fully_connected_q_queries(fully_conn_cases);
    save_bench_to_file("../results/bench_fully_connected_q_queries", res_fully_conn, fully_conn_cases);

    std::vector<int> worst_a_cases = powers_of_two(8, 256);
    auto res_worst_a = bench_worst_process_a(worst_a_cases);
    save_worst_case_to_file("../results/worst_case_process_a_bench", res_worst_a, worst_a_cases);

    std::vector<int> worst_b_cases = powers_of_two(6, 256);
    auto res_worst_b = bench_worst_process_b(worst_b_cases);
    save_worst_case_to_file("../results/worst_case_process_b_bench", res_worst_b, worst_b_cases);

    std::vector<int> bench_ring_cases = {256, 2048, 16384};
    auto res_ring = bench_ring_q_random_queries(bench_ring_cases);
    save_bench_to_file("../results/bench_ring_q_queries", res_ring, bench_ring_cases);

    return 0;
}
