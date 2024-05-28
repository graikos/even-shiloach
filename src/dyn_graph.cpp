#include "graph.hpp"
#include <ctime>
#include <queue>
#include <iostream>
#include <boost/random/mersenne_twister.hpp>
#include "dyn_graph.hpp"
#include "algo.hpp"
#include "edge_set.hpp"

using namespace boost;

DynGraph::DynGraph(Graph &G) : _G(G), _component_max_idx(0)
{
    init();
}

void DynGraph::init()
{

    _levels = std::vector<int>(num_vertices(_G), -1);
    _components = std::vector<int>(num_vertices(_G), -1);
    // initialze edge sets for each vertex
    alpha = std::vector<EdgeSet>(num_vertices(_G));
    beta = std::vector<EdgeSet>(num_vertices(_G));
    gamma = std::vector<EdgeSet>(num_vertices(_G));

    // pick random vertex as root
    mt19937 mt(std::time(0));
    _r = vertex(mt() % num_vertices(_G), _G);
    std::cout << "Picked as root: " << _r << std::endl;

    // perform initial BFS from root r
    my::bfs(_G, _r, _levels, _components, _component_max_idx, alpha, beta, gamma);

    for (std::size_t i = 0; i < _levels.size(); ++i)
    {
        Vertex s = vertex(i, _G);
        // if current vertex is not part of an existing connected component, it has not been discovered
        if (_levels[i] < 0)
        {
            // the root of this new component will be artificially connected to the random root
            // will be on level 1 and an artificial edge will be added
            my::bfs(_G, s, _levels, _components, ++_component_max_idx, alpha, beta, gamma, 1);
            _add_artificial_edge(_r, i);
            // also add artifical edges to appropriate edge sets
            alpha[i].add_edge(_r, i);
            gamma[_r].add_edge(_r, i);
        }
    }
}

void DynGraph::_add_artificial_edge(Vertex v, Vertex u)
{
    // artificial edges will be kept in a hash map, with key the min of the two and value the max of the two
    std::cout << "Adding" << std::endl;
    _artificial_edges.add_edge(u, v);
}

void DynGraph::_remove_artificial_edge(Vertex v, Vertex u)
{
    _artificial_edges.remove_edge(v, u);
}

bool DynGraph::_check_if_artificially_connected(Vertex v, Vertex u)
{
    return _artificial_edges.contains(v, u);
}

void DynGraph::print()
{
    VertexIterator vi, viend;
    for (tie(vi, viend) = vertices(_G); vi != viend; ++vi)
    {
        std::cout << "Vertex " << *vi << " : ";
        OutEdgeIterator oei, oeiend;
        for (tie(oei, oeiend) = out_edges(*vi, _G); oei != oeiend; ++oei)
        {
            std::cout << "Edge: " << *oei << " ";
        }
        std::cout << std::endl;
    }
}

void DynGraph::_rewind()
{

    std::vector<std::vector<EdgeSet> *> s = {&alpha, &beta, &gamma};

    while (!_change_history.empty())
    {
        auto &record = _change_history.top();
        _change_history.pop();

        switch (record.type)
        {
        case my::ChangeRecordType::LevelBump:

            // undo the level increase of v
            --_levels[record.v];
            break;

        case my::ChangeRecordType::Insert:
            (*(s[record.primary_set]))[record.v].remove_edge(record.v, record.u);
            break;

        case my::ChangeRecordType::Remove:
            (*(s[record.primary_set]))[record.v].add_edge(record.v, record.u);
            break;

        case my::ChangeRecordType::AlphaBetaMove:

            // undo the alpha(w) <- beta(w)
            // the old alpha is kept
            alpha[record.v] = std::move(record.old_set);

            break;

        case my::ChangeRecordType::BetaGammaMove:

            // undo the beta(w) <- gamma(w)
            // to do that, use the current alpha(w)
            // This will work because we use a stack, meaning we traverse the avalanche steps in reverse
            // This means that we have not yet changed alpha(w) back to the old one, meaning at this step
            // alpha(w) holds the old beta(w) still.
            // Also, there is the issue of moving alpha(w) to beta(w), thus invalidating the current alpha(w),
            // running the risk of leaving the alpha(w) set as undefined in the new version of the dyn_graph.
            // This CANNOT happen because there is nothing between step 7 to step 5 that could break the execution of
            // process B, thus leaving alpha(w) unchanged, which will happen at step 5.
            // Also, in step 6 no changes are made to alpha(w).
            // Essentially, if we find ourselves in this step, rewinding the beta(w) <- gamma(w) move, we will certainly
            // rewind the alpha(w) <- beta(w) move too, thus setting the momentarily undefined alpha(w) back to its
            // old version.

            beta[record.v] = std::move(alpha[record.v]);

            break;

        case my::ChangeRecordType::GammaEmptyMove:

            // similary, this takes place before the previous rewind, so
            // beta(w) will still have the old value of gamma(w)

            gamma[record.v] = std::move(beta[record.v]);
            break;

        default:
            continue;
        }
    }
}

void DynGraph::dyn_remove_edge(Edge e)
{

    Vertex u = source(e, _G);
    Vertex v = target(e, _G);

    // remove the edge from the graph, the edge is removed from the appropriate EdgeSets inside process B
    remove_edge(e, _G);

    // initialize the "parallel" processes
    my::StepDetectBreak procA(_G, u, v);
    my::StepDetectNotBreak procB(_levels, alpha, beta, gamma, _change_history, u, v);

    // Execution halts if:
    // 1) Process B halts. Result: no component breaks
    // 2) Process A halts and has detected component break. Result: component breaks
    // In other cases, if A has finished but no component breaks, and B is still running, A is skipped
    while (procB.state != my::StepDetectNotBreakState::Finished)
    {
        if (procA.state != my::StepDetectBreakState::Finished)
        {
            procA.advance();
        }
        if (procA.state == my::StepDetectBreakState::Finished && procA.component_breaks)
        {
            // here process A has detected a component breaking
            // we need to update components, rewind process B changes, and add artifical edge
            ++_component_max_idx;
            for (auto it = procA.small_component.begin(); it != procA.small_component.end(); ++it)
            {
                _components[*it] = _component_max_idx;
            }
            _rewind();

            // now add artificial edge
            _artificial_edges.add_edge(u, v);

            // follow convention that u is the one with the smaller level
            if (_levels[v] < _levels[u])
            {
                std::swap(v, u);
            }

            // add artificial edge to edge sets again
            // TODO: think about this
            gamma[u].add_edge(u, v);
            alpha[v].add_edge(u, v);

            break;
        }
        procB.advance();
    }
}

bool DynGraph::query_is_connected(Vertex v, Vertex u)
{
    return _components[v] == _components[u];
}