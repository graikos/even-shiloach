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

my::StepDetectNotBreak::StepDetectNotBreak(std::vector<int> &levels, std::vector<EdgeSet> &alpha, std::vector<EdgeSet> &beta,
                                           std::vector<EdgeSet> &gamma, std::stack<ChangeRecord> &changes_stack,
                                           Vertex u, Vertex v) : _levels(levels), _alpha(alpha), _beta(beta), _gamma(gamma), _u(u), _v(v), component_breaks(false), _changes_stack(changes_stack)
{
    _init();
}

void my::StepDetectNotBreak::_init()
{
    state = StepDetectNotBreakState::InitialCheckLevels;
}

/*
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
*/

void my::StepDetectNotBreak::advance()
{
    switch (state)
    {
    case StepDetectNotBreakState::InitialCheckLevels:
        if (_levels[_u] == _levels[_v])
        {
            std::cout << "Endpoints were at the same level." << std::endl;
            // same level means component does not break
            // remove edge from beta sets of each
            _beta[_u].remove_edge(_u, _v);
            _beta[_v].remove_edge(_u, _v);

            state = StepDetectNotBreakState::Finished;
            component_breaks = false;
            return;
        }
        std::cout << "Moving to initial different levels." << std::endl;
        state = StepDetectNotBreakState::InitialDifferentLevels;
        return;

    case StepDetectNotBreakState::InitialDifferentLevels:

        if (_levels[_v] < _levels[_u])
        {
            // make _u be the one with the smaller level of the two
            // to avoid slips and checks further along
            std::swap(_v, _u);
        }

        _gamma[_u].remove_edge(_u, _v);
        _alpha[_v].remove_edge(_u, _v);

        if (!_alpha[_v].empty())
        {
            std::cout << "alpha(v) not empty; component did not break" << std::endl;
            // components have not changed
            state = StepDetectNotBreakState::Finished;
            component_breaks = false;
            return;
        }

        state = StepDetectNotBreakState::InitLevelAvalanche;
        return;

    case StepDetectNotBreakState::InitLevelAvalanche:

        std::cout << "Init avalanche" << std::endl;
        _Q.push(_v);
        state = StepDetectNotBreakState::AvalancheStep1_2_3;
        return;

    case StepDetectNotBreakState::AvalancheStep1_2_3:

        // check if process should stop because of empty queue
        if (_Q.empty())
        {
            std::cout << "queue is empty; finished" << std::endl;
            component_breaks = false;
            state = StepDetectNotBreakState::Finished;
            return;
        }

        // pop queue
        _current_w = _Q.front();
        _Q.pop();
        std::cout << "Popped: " << _current_w << std::endl;

        // increase popped vertex level
        ++_levels[_current_w];
        // add change to stack
        _changes_stack.push(my::ChangeRecord{my::ChangeRecordType::LevelBump, _current_w, _current_w, 0, EdgeSet{}});

        _current_esi = _beta[_current_w].begin();

        // proceed to avalanche
        state = StepDetectNotBreakState::AvalancheStep4;
        return;

    case StepDetectNotBreakState::AvalancheStep4:
    {
        if (_current_esi == _beta[_current_w].end())
        {
            std::cout << "Step 4: all edges examined" << std::endl;
            // iterated through all of beta(w) edges, move to next step
            state = StepDetectNotBreakState::AvalancheStep5;
            return;
        }

        // for the current examined edge in b(w), get its other end (w')
        Vertex w_prime = _beta[_current_w].other_end(_current_esi, _current_w);
        // remove current edge from beta(w') and insert into gamma(w')
        _beta[w_prime].remove_edge(_current_w, w_prime);
        _gamma[w_prime].add_edge(_current_w, w_prime);

        _changes_stack.push(my::ChangeRecord{my::ChangeRecordType::Remove, w_prime, _current_w, 1, EdgeSet{}});
        _changes_stack.push(my::ChangeRecord{my::ChangeRecordType::Insert, w_prime, _current_w, 2, EdgeSet{}});

        std::cout << "Step 4: Examined edge: (" << _current_w << "," << w_prime << ")" << std::endl;

        // continue with next edge in beta(w)
        ++_current_esi;
        return;
    }

    case StepDetectNotBreakState::AvalancheStep5:

        std::cout << "Step 5" << std::endl;

        // push change to stack to save alpha(w) before changing it
        // alpha(w) will be moved to the old EdgeSet change record field
        _changes_stack.push(my::ChangeRecord{my::ChangeRecordType::AlphaBetaMove, _current_w, _current_w, 0, std::move(_alpha[_current_w])});

        // transfer beta(w) to alpha(w), beta(w) is now empty
        _alpha[_current_w] = std::move(_beta[_current_w]);


        // initialize edge iterator for gamma(w)
        _current_esi = _gamma[_current_w].begin();
        state = StepDetectNotBreakState::AvalancheStep6;

        return;

    case StepDetectNotBreakState::AvalancheStep6:
    {
        if (_current_esi == _gamma[_current_w].end())
        {
            std::cout << "Step 6: examined all edges" << std::endl;
            // iterated through all of gamma(w) edges
            state = StepDetectNotBreakState::AvalancheStep7;
            return;
        }

        Vertex w_prime = _gamma[_current_w].other_end(_current_esi, _current_w);
        _alpha[w_prime].remove_edge(w_prime, _current_w);
        _beta[w_prime].add_edge(w_prime, _current_w);

        _changes_stack.push(my::ChangeRecord{my::ChangeRecordType::Remove, w_prime, _current_w, 0, EdgeSet{}});
        _changes_stack.push(my::ChangeRecord{my::ChangeRecordType::Insert, w_prime, _current_w, 1, EdgeSet{}});

        std::cout << "Step 6: examined edge: (" << _current_w << "," << w_prime << ")" << std::endl;

        if (_alpha[w_prime].empty())
        {
            std::cout << "alpha(w') found emtpy; pushing w_prime to queue" << std::endl;
            _Q.push(w_prime);
        }

        ++_current_esi;
        return;
    }

    case StepDetectNotBreakState::AvalancheStep7:

        // transfer gamma(w) to beta(w)
        // gamma(w) is emptied inside the move
        _beta[_current_w] = std::move(_gamma[_current_w]);
        // no need to keep track of old EdgeSet for the change record for this move
        _changes_stack.push(my::ChangeRecord{my::ChangeRecordType::BetaGammaMove, _current_w, _current_w, 0, EdgeSet{}});
        // also add the emptying of gamma(w) to the changes
        _changes_stack.push(my::ChangeRecord{my::ChangeRecordType::GammaEmptyMove, _current_w, _current_w, 0, EdgeSet{}});
        std::cout << "Step 7" << std::endl;

        state = StepDetectNotBreakState::AvalancheStep8;

        return;

    case StepDetectNotBreakState::AvalancheStep8:

        std::cout << "Step 8" << std::endl;
        if (_alpha[_current_w].empty())
        {
            std::cout << "Step 8: alpha(w) is still empty, pushing again" << std::endl;
            // alpha(w) is still empty, push to queue again
            _Q.push(_current_w);
        }

        state = StepDetectNotBreakState::AvalancheStep1_2_3;

        return;

    case StepDetectNotBreakState::Uninitialized:
        _init();
        return;
    case StepDetectNotBreakState::Finished:
    default:
        return;
    }
}