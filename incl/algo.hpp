#ifndef ALGO_HPP
#define ALGO_HPP

#include "graph.hpp"
#include <stack>
#include <list>
#include <unordered_set>

namespace my
{
    void bfs(const Graph &G, Vertex s, std::vector<int> &levels, std::vector<int> &comp, int comp_val, int levels_offset = 0);
    void dfs(const Graph &G, Vertex s, std::vector<int> &comp, int comp_val);
    bool dfs_scan(const Graph &G, Vertex s, Vertex t);

    enum StepScanState
    {
        Uninitialized,
        Examine_new_vertex,
        Examine_new_edge,
        Finished
    };

    // StepScanDFS implements a step-by-step DFS scanning algorithm to check whether a vertex t is reachable from a vertex s.
    class StepScanDFS
    {
    public:
        // result will hold the final return value of the algorithm execution once it reaches a finished state
        bool result;
        // target_mode being true means scan will stop when a specified target is reached. In this case, no track of
        // connected component is kept
        bool target_mode;
        // state tracks the current state of the execution to determine next step
        StepScanState state;
        // list of vertices in this connected component. Only applicable if algorithm is not running in target mode
        std::list<Vertex> component;

        // scans until vertex t is found, does not keep track of component
        StepScanDFS(const Graph &G, Vertex s, Vertex t);
        // scans to find whole component
        StepScanDFS(const Graph &G, Vertex s);
        // advance advances the execution by one step, depending on the current state
        void advance();

    private:
        const Graph &_G;
        std::stack<Vertex> _stack;
        // using a set instead of a vector here to avoid the O(N) initialization, which changes the complexity of subsequent algorithms.
        // The complexity analysis of algorithms such as circuit_free_update_components assumes that a DFS scan running for a small component
        // will only pay for the vertices it has, and not the N vertices of the graph.
        std::unordered_set<Vertex> _visited;
        Vertex _s;
        Vertex _t;
        // holds current edge iterator, if we're in an edge examination state
        OutEdgeIterator _current_ei;
        OutEdgeIterator _eiend;
        // holds current vertex being examined
        Vertex _current_v;

        // init initializes the algorithm temporary data and kicks off the execution
        void _init();
    };

    void circuit_free_update_components(const Graph &G, Vertex u, Vertex v, std::vector<int> &comps, int new_comp_val);

    class StepDetectBreak
    {
        StepDetectBreak(const Graph &G, Vertex u, Vertex v);

    private:
        const Graph &_G;
    };

}

#endif