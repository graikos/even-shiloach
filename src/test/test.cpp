#include <iostream>
#include <ctime>
#include "graph.hpp"
#include <cassert>
#include <vector>
#include "algo.hpp"
#include "dyn_graph.hpp"
#include <boost/random/mersenne_twister.hpp>
#include <boost/graph/random.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/make_connected.hpp>
#include "edge_set.hpp"
#include "gen.hpp"
#include "util.hpp"
#define MAX_RANDOM_VERTICES 8000
#define MAX_RANDOM_EDGES 16000

using namespace boost;

void test_ring(mt19937 &mt)
{
    Graph G;
    std::vector<Edge> edge_handles;
    auto num_of_vertices = mt() % MAX_RANDOM_VERTICES;
    std::cout << "Testing ring with " << num_of_vertices << " vertices... ";
    gen::generate_ring(G, num_of_vertices, edge_handles);

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
    assert(num_edges(G) == 0);
    std::cout << "Success" << std::endl;
}

void test_line(mt19937 &mt)
{
    Graph G;
    std::vector<Edge> edge_handles;
    auto num_of_vertices = mt() % MAX_RANDOM_VERTICES;
    std::cout << "Testing line with " << num_of_vertices << " vertices... ";
    gen::generate_line(G, num_of_vertices, edge_handles);
    DynGraph DG(G);

    for (auto it = edge_handles.begin(); it != edge_handles.end(); ++it)
    {
        DG.dyn_remove_edge(*it);
        assert(!DG.query_is_connected(source(*it, G), target(*it, G)) && "Every subsequent edge removal should break a component.");
    }
    assert(num_edges(G) == 0);
    std::cout << "Success" << std::endl;
}

void test_random_no_assert(mt19937 &mt)
{
    Graph G;
    std::vector<Edge> edge_handles;
    auto num_of_vertices = mt() % MAX_RANDOM_VERTICES;
    auto num_of_edges = mt() % MAX_RANDOM_EDGES;
    std::cout << "Testing random no-fail with " << num_of_vertices << " vertices and " << num_of_edges << " edges... ";
    gen::generate_random(G, num_of_vertices, num_of_edges, edge_handles, mt);
    DynGraph DG(G);

    for (auto it = edge_handles.begin(); it != edge_handles.end(); ++it)
    {
        DG.dyn_remove_edge(*it);
        DG.query_is_connected(source(*it, G), target(*it, G));
    }
    assert(num_edges(G) == 0);
    std::cout << "Success" << std::endl;
}

void test_random_connected(mt19937 &mt)
{
    Graph G;
    std::vector<Edge> edge_handles;
    auto num_of_vertices = mt() % MAX_RANDOM_VERTICES;
    auto num_of_edges = mt() % MAX_RANDOM_EDGES;
    gen::generate_random(G, num_of_vertices, num_of_edges, edge_handles, mt);
    make_connected(G);
    DynGraph DG(G);
    std::cout << "Testing random connected with " << num_vertices(G) << " vertices and " << num_edges(G) << " edges... ";

    std::vector<Edge> st_vec;
    my::dfs_tree(G, 0, st_vec);
    EdgeSet st_set;
    for (auto it = st_vec.begin(); it != st_vec.end(); ++it)
    {
        st_set.add_edge(source(*it, G), target(*it, G));
    }
    for (auto it = edge_handles.begin(); it != edge_handles.end(); ++it)
    {
        if (st_set.contains(source(*it, G), target(*it, G)))
        {
            // skip over spanning tree edges for now
            continue;
        }

        DG.dyn_remove_edge(*it);
        assert(DG.query_is_connected(source(*it, G), target(*it, G)) && "Non spanning tree edges should not break component.");
    }

    for (auto it = st_vec.begin(); it != st_vec.end(); ++it)
    {
        DG.dyn_remove_edge(*it);
        assert(!DG.query_is_connected(source(*it, G), target(*it, G)) && "Every spanning tree edge should now break a component.");
    }

    assert(num_edges(G) == 0);
    std::cout << "Success" << std::endl;
}

void test_fully_connected(mt19937 &mt)
{
    Graph G;
    std::vector<Edge> edge_handles;
    auto num_of_vertices = mt() % 1500;
    gen::generate_fully_connected(G, num_of_vertices, edge_handles);
    DynGraph DG(G);
    std::cout << "Testing fully connected with " << num_vertices(G) << " vertices... ";

    std::vector<Edge> st_vec;
    my::dfs_tree(G, 0, st_vec);
    EdgeSet st_set;
    for (auto it = st_vec.begin(); it != st_vec.end(); ++it)
    {
        st_set.add_edge(source(*it, G), target(*it, G));
    }
    for (auto it = edge_handles.begin(); it != edge_handles.end(); ++it)
    {
        if (st_set.contains(source(*it, G), target(*it, G)))
        {
            // skip over spanning tree edges for now
            continue;
        }

        DG.dyn_remove_edge(*it);
        assert(DG.query_is_connected(source(*it, G), target(*it, G)) && "Non spanning tree edges should not break component.");
    }

    for (auto it = st_vec.begin(); it != st_vec.end(); ++it)
    {
        DG.dyn_remove_edge(*it);
        assert(!DG.query_is_connected(source(*it, G), target(*it, G)) && "Every spanning tree edge should now break a component.");
    }

    assert(num_edges(G) == 0);
    std::cout << "Success" << std::endl;
}

int main()
{
    mt19937 mt(time(0));
    test_ring(mt);
    test_line(mt);
    test_random_no_assert(mt);
    test_random_connected(mt);
    test_fully_connected(mt);
    return 0;
}
