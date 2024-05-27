#ifndef ALGO_HPP
#define ALGO_HPP

#include "graph.hpp"
#include "edge_set.hpp"
#include <stack>
#include <queue>
#include <list>
#include <unordered_set>
#include <unordered_map>
#include <vector>

namespace my
{
    // bfs performs the BFS algorithm and keeps track of the levels, starting with a root level of "levels_offset". It
    // also marks the component discovered and keeps track of edge sets (unordered_map) for each vertex to the previous (alpha), current(beta),
    // and next (gamma) levels.
    void bfs(const Graph &G, Vertex s, std::vector<int> &levels, std::vector<int> &comp, int comp_val,
             std::vector<EdgeSet> &alpha,
             std::vector<EdgeSet> &beta,
             std::vector<EdgeSet> &gamma,
             int levels_offset = 0);

    void dfs(const Graph &G, Vertex s, std::vector<int> &comp, int comp_val);
    bool dfs_scan(const Graph &G, Vertex s, Vertex t);

    enum class StepScanState
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
        // target_mode being true means scan will stop when a specified target is reached. Meanwhile, the connected component is kept
        // in case target is not found. If target is found, the contents of the component should be disregarded.
        bool target_mode;
        // state tracks the current state of the execution to determine next step
        StepScanState state;
        // list of vertices in this connected component.
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

    enum class StepDetectBreakState
    {
        Uninitialized,
        FirstBranch,
        SecondBranch,
        Finished
    };

    // StepDetectBreak implements a step-by-step version of the "parallel" process to check whether by removing an edge (u,v) a connected component breaks.
    // If it does, the smallest of the two newly created components is saved as "small_component". The caller can use that to update the component name.
    // If it does not, the "coomponent_breaks" variable is false and no small_component is set.
    // This is labeled as Process A in the paper.
    class StepDetectBreak
    {
    public:
        bool component_breaks;
        StepDetectBreakState state;
        std::list<Vertex> small_component;

        StepDetectBreak(const Graph &G, Vertex u, Vertex v);

        void advance();

    private:
        const Graph &_G;
        StepScanDFS sdfs1;
        StepScanDFS sdfs2;
    };

    enum class StepDetectNotBreakState
    {
        Uninitialized,
        InitialCheckLevels,
        InitialDifferentLevels,
        InitLevelAvalanche,
        AvalancheStep1_2_3,
        AvalancheStep4,
        AvalancheStep5,
        AvalancheStep6,
        AvalancheStep7,
        AvalancheStep8,
        Finished
    };

    class StepDetectNotBreak
    {
    public:
        bool component_breaks;
        StepDetectNotBreakState state;

        StepDetectNotBreak() = default;
        StepDetectNotBreak(std::vector<int> &levels,
                           std::vector<EdgeSet> &alpha,
                           std::vector<EdgeSet> &beta,
                           std::vector<EdgeSet> &gamma,
                           std::stack<int> &changes_stack,
                           Vertex u, Vertex v);

        void advance();

    private:
        std::stack<int> &_changes_stack; // TODO: change int

        std::queue<Vertex> _Q;

        Vertex _u;
        Vertex _v;

        std::vector<int> &_levels;
        std::vector<EdgeSet> &_alpha;
        std::vector<EdgeSet> &_beta;
        std::vector<EdgeSet> &_gamma;

        Vertex _current_w;
        EdgeSetIterator _current_esi;

        void _init();
    };

}

#endif