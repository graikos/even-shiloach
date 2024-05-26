#include "algo.hpp"
#include <queue>
#include <stack>
#include <iostream>

using namespace boost;

void my::bfs(const Graph &G, Vertex s, std::vector<int> &levels, std::vector<int> &comp, int comp_val, int levels_offset)
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
    if (!target_mode)
    {
        component = std::list<Vertex>();
        component.push_back(_s);
    }
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
        std::cout << "popped: " << _current_v << std::endl;

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

        std::cout << "examining edge: " << *_current_ei << std::endl;

        Vertex v = target(*_current_ei, _G);

        if (_visited.find(v) == _visited.end())
        {
            if (target_mode && v == _t)
            {
                // scan complete, found vertex
                state = my::StepScanState::Finished;
                result = true;
                return;
            }
            _stack.push(v);
            _visited.insert(v);

            if (!target_mode)
            {
                component.push_back(v);
            }
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
