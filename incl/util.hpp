#ifndef MY_UTIL_HPP
#define MY_UTIL_HPP

#include "graph.hpp"
#include <fstream>
#include <iostream>
#include <vector>
#include <list>
// #include <boost/graph/graphviz.hpp>

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

Edge add_edge(Vertex u, Vertex v, Graph &G)
{
    Edge e;
    bool c;
    tie(e, c) = boost::add_edge(u, v, G);
    return e;
}

void save_bench_to_file(const std::string &filename, const std::vector<std::vector<std::vector<double>>> &vec,
                        const std::vector<int> &cases)
{
    for (auto c = 0; c < cases.size(); ++c)
    {
        std::ofstream file;
        std::stringstream sstm;
        sstm << filename << "_" << cases[c] << ".dat";
        file.open(sstm.str(), std::ios::out | std::ios::trunc);
        for (auto q = vec[c].begin(); q != vec[c].end(); ++q)
        {
            file << q->at(0) << "," << q->at(1) << std::endl;
        }
        file.close();
    }
}

void save_bench_to_file(const std::string &filename, const std::vector<std::vector<std::vector<double>>> &vec,
                        const std::vector<std::pair<int, int>> &cases)
{
    for (auto c = 0; c < cases.size(); ++c)
    {
        std::ofstream file;
        std::stringstream sstm;
        sstm << filename << "_" << cases[c].first << "_" << cases[c].second << ".dat";
        file.open(sstm.str(), std::ios::out | std::ios::trunc);
        for (auto q = vec[c].begin(); q != vec[c].end(); ++q)
        {
            file << q->at(0) << "," << q->at(1) << std::endl;
        }
        file.close();
    }
}

void save_worst_case_to_file(const std::string &filename, const std::vector<double> &vec,
                             const std::vector<int> &cases)
{
    std::ofstream file;
    std::stringstream sstm;
    sstm << filename << ".dat";
    file.open(sstm.str(), std::ios::out | std::ios::trunc);
    for (auto c = cases.begin(); c!= cases.end(); ++c)
    {
        file << *c << ",";
    }
    file << std::endl;
    for (auto q = vec.begin(); q != vec.end(); ++q)
    {
        file << *q << std::endl;
    }
    file.close();
}

std::vector<int> powers_of_two(int num, int start_with=1)
{
    std::vector<int> p(num);
    int a = start_with;
    for (int i =0; i < num; ++i)
    {
        p[i] = a;
        a *= 2;
    }
    return p;
}

// void save_graphviz_file(const std::string &filename, const Graph &G)
// {
//     std::ofstream file;
//     file.open(filename, std::ios::out | std::ios::trunc);
//     write_graphviz(file, G);
//     file.close();
// }

#endif
