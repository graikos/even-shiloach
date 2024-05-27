#include "algo.hpp"
#include <queue>
#include <stack>
#include <iostream>

using namespace boost;

void my::bfs(const Graph &G, Vertex s, std::vector<int> &levels, std::vector<int> &comp, int comp_val,
             std::vector<EdgeSet> &alpha,
             std::vector<EdgeSet> &beta,
             std::vector<EdgeSet> &gamma,
             int levels_offset)
{
    std::queue<Vertex> q;
    levels[s] = levels_offset;
    comp[s] = comp_val;
    q.push(s);

    while (!q.empty())
    {
        Vertex u = q.front();
        q.pop();
        OutEdgeIterator ei, eiend;
        for (tie(ei, eiend) = out_edges(u, G); ei != eiend; ++ei)
        {
            Vertex v = target(*ei, G);
            if (levels[v] < 0)
            {
                q.push(v);
                levels[v] = levels[u] + 1;
                comp[v] = comp_val;
                // here we know that the edge (u,v) will be in alpha(v) and gamma(u)
                alpha[v].add_edge(u, v);
                gamma[u].add_edge(u, v);
                continue;
            }

            // here we know that the vertex has already been discovered
            // it remains to check what's their level relation
            // due to BGL out_edges, which will show both (u,v) and (v,u), the insertions below will run two times
            // for each edge, but the set will correctly insert them only once
            if (levels[u] < levels[v])
            {
                alpha[v].add_edge(u, v);
                gamma[u].add_edge(u, v);
            }
            else if (levels[u] == levels[v])
            {
                beta[v].add_edge(u, v);
                beta[u].add_edge(u, v);
            }
            else
            {
                alpha[u].add_edge(u, v);
                gamma[v].add_edge(u, v);
            }
        }
    }
}

void my::dfs(const Graph &G, Vertex s, std::vector<int> &comp, int comp_val)
{
    std::stack<Vertex> stack;
    comp[s] = comp_val;
    stack.push(s);

    while (!stack.empty())
    {
        Vertex u = stack.top();
        stack.pop();
        OutEdgeIterator ei, eiend;
        for (tie(ei, eiend) = out_edges(u, G); ei != eiend; ++ei)
        {
            Vertex v = target(*ei, G);
            if (comp[v] < 0)
            {
                stack.push(v);
                comp[v] = comp_val;
            }
        }
    }
}

bool my::dfs_scan(const Graph &G, Vertex s, Vertex t)
{
    std::stack<Vertex> stack;
    std::vector<bool> visited(num_vertices(G), false);

    stack.push(s);
    visited[s] = true;
    if (s == t)
        return true;

    while (!stack.empty())
    {
        Vertex u = stack.top();
        stack.pop();
        OutEdgeIterator ei, eiend;
        for (tie(ei, eiend) = out_edges(u, G); ei != eiend; ++ei)
        {
            Vertex v = target(*ei, G);

            if (!visited[v])
            {
                if (v == t)
                {
                    // scan complete, found vertex
                    return true;
                }
                stack.push(v);
                visited[v] = true;
            }
        }
    }
    // scan complete, not found
    return false;
}

my::StepScanDFS::StepScanDFS(const Graph &G, Vertex s, Vertex t) : _G(G), result(false), state(my::StepScanState::Uninitialized), _s(s), _t(t), target_mode(true)
{
    // trivial check
    if (_s == _t)
    {
        result = true;
        state = my::StepScanState::Finished;
        component.push_back(_s); // connected component contains only _s
        return;
    }
    _init();
}

my::StepScanDFS::StepScanDFS(const Graph &G, Vertex s) : _G(G), result(false), state(my::StepScanState::Uninitialized), _s(s), target_mode(false)
{
    _init();
}

void my::StepScanDFS::_init()
{

    _visited = std::unordered_set<Vertex>();

    _stack.push(_s);
    _visited.insert(_s);

    // NOTE: even in target mode, component will be kept.
    // In case target is not found, we will at least have the connected component needed for process A
    component = std::list<Vertex>();
    component.push_back(_s);

    state = my::StepScanState::Examine_new_vertex;
}

void my::StepScanDFS::advance()
{
    switch (state)
    {
    case my::StepScanState::Uninitialized:
        _init();
        return;
    case my::StepScanState::Examine_new_vertex:
        if (_stack.empty())
        {
            // finished scan, default result (false) returned
            state = my::StepScanState::Finished;
            return;
        }
        // pop stack to get new vertex to examine
        _current_v = _stack.top();
        _stack.pop();

        // initialize edge iterators
        tie(_current_ei, _eiend) = out_edges(_current_v, _G);
        if (_current_ei == _eiend)
        {
            // no edges, next advance should examine new vertex again
            return;
        }

        // next advance will examine edges
        state = my::StepScanState::Examine_new_edge;
        return;

    case my::StepScanState::Examine_new_edge:
    {

        if (_current_ei == _eiend)
        {
            // all edges of current vertex have been examined, next examine new vertex
            state = my::StepScanState::Examine_new_vertex;
            return;
        }

        Vertex v = target(*_current_ei, _G);

        if (_visited.find(v) == _visited.end())
        {
            component.push_back(v);
            if (target_mode && v == _t)
            {
                // scan complete, found vertex
                state = my::StepScanState::Finished;
                result = true;

                return;
            }
            _stack.push(v);
            _visited.insert(v);
        }

        ++_current_ei;

        return;
    }

    case my::StepScanState::Finished:
    default:
        return;
    }
}

void my::circuit_free_update_components(const Graph &G, Vertex u, Vertex v, std::vector<int> &comps, int new_comp_val)
{
    // initialize a step DFS from both ends specified
    // not in target mode, since we know for a fact that a circuit free connected component breaks for every edge deletion
    StepScanDFS sdfs1(G, u);
    StepScanDFS sdfs2(G, v);

    while (sdfs1.state != StepScanState::Finished && sdfs2.state != StepScanState::Finished)
    {
        sdfs1.advance();
        sdfs2.advance();
    }

    // one of the two terminated
    // find which one and update the components
    const auto &finished_component = (sdfs1.state == StepScanState::Finished) ? sdfs1.component : sdfs2.component;
    for (auto it = finished_component.begin(); it != finished_component.end(); ++it)
    {
        comps[*it] = new_comp_val;
    }
}

my::StepDetectBreak::StepDetectBreak(const Graph &G, Vertex u, Vertex v) : state(StepDetectBreakState::FirstBranch), component_breaks(false), _G(G), sdfs1(G, u, v), sdfs2(G, v, u)
{
}

void my::StepDetectBreak::advance()
{
    switch (state)
    {
    case StepDetectBreakState::FirstBranch:
        // advance first branch search by one step
        if (sdfs1.state == StepScanState::Finished)
        {
            if (!sdfs1.result)
            {
                // if it has not found the other end, this branch has found the small component, return so we can change
                small_component = sdfs1.component;
                component_breaks = true;
            }
            // NOTE: if it did find the other edge, the component_breaks will be the initial value (false)
            state = StepDetectBreakState::Finished;
            return;
        }

        sdfs1.advance();

        // switch to the other branch
        state = StepDetectBreakState::SecondBranch;
        return;

    case StepDetectBreakState::SecondBranch:
        // similar to previous
        if (sdfs2.state == StepScanState::Finished)
        {
            if (!sdfs2.result)
            {
                small_component = sdfs2.component;
                component_breaks = true;
            }
            state = StepDetectBreakState::Finished;
            return;
        }

        sdfs2.advance();

        // switch back to first branch
        state = StepDetectBreakState::FirstBranch;
        return;

    case StepDetectBreakState::Uninitialized:
    default:
        return;
    }
}