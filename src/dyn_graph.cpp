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
    init(true);
}
DynGraph::DynGraph(Graph &G, Vertex r) : _G(G), _component_max_idx(0), _r(r)
{
    init(false);
}

void DynGraph::init(bool random_root)
{

    _levels = std::vector<int>(num_vertices(_G), -1);
    _components = std::vector<int>(num_vertices(_G), -1);
    // initialze edge sets for each vertex
    alpha = std::vector<EdgeSet>(num_vertices(_G));
    beta = std::vector<EdgeSet>(num_vertices(_G));
    gamma = std::vector<EdgeSet>(num_vertices(_G));

    if (random_root)
    {
        // pick random vertex as root
        mt19937 mt(std::time(0));
        _r = vertex(mt() % num_vertices(_G), _G);
    }

    // perform initial BFS from root r
    my::bfs(_G, _r, _levels, _components, _component_max_idx, alpha, beta, gamma);

    for (std::size_t i = 0; i < _levels.size(); ++i)
    {
        Vertex s = vertex(i, _G);
        // if current vertex is not part of an existing connected component, it has not been discovered
        if (_levels[i] < 0)
        {
            // the root of this new component will be artificially connected to the random root
            // will be on level 1
            my::bfs(_G, s, _levels, _components, ++_component_max_idx, alpha, beta, gamma, 1);
        }
    }
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
        // WARNING: move here is necessary to avoid popping and destroying the reference while we use it below
        ChangeRecord record = std::move(_change_history.top());
        _change_history.pop();

        switch (record.type)
        {
        case ChangeRecordType::LevelBump:

            // undo the level increase of v
            --_levels[record.v];
            break;

        case ChangeRecordType::Insert:
            (*(s[record.primary_set]))[record.v].remove_edge(record.v, record.u);
            break;

        case ChangeRecordType::Remove:
            (*(s[record.primary_set]))[record.v].add_edge(record.v, record.u);
            break;

        case ChangeRecordType::AlphaBetaMove:

            // undo the alpha(w) <- beta(w)
            alpha[record.v] = std::move(record.old_set);

            break;

        case ChangeRecordType::RestoreBeta:
            // in that case, this will move alpha again, which will have already been moved

            // this is restoring beta(w), which is invalidate with the move inside step 5
            // in step 5 beta(w) is moved to alpha(w). During rewinding, if we only rewind
            // alpha to its old set, beta(w) will be left invalid, with a possibility of not being
            // overwritten, if execution does not make it to step 7. It is guaranteed due to the order of the
            // stack that the next entry will restore alpha(w), so no problem with this move

            // NOTE: IF the execution stops after step 7, the following problem will occur:
            // beta(w) will be restored in the betagammamove restore. Then this restoree will be called,
            // which will be the same as teh betagammamove restore. And this will overwrite beta[v] with the
            // already-moved alpha, so beta will be undefined again. This is why BetaGammaMove will be removed.
            // In step 6, beta(w) is not used, so a rewind of step 5 is guaranteed to run after rewinding step 7, so this will run
            // too and beta will eventually be restored

            beta[record.v] = std::move(alpha[record.v]);

            break;

        case ChangeRecordType::GammaEmptyMove:

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

    reorg_after_remove(v, u);
}

void DynGraph::reorg_after_remove(Vertex v, Vertex u)
{
    // initialize the "parallel" processes
    my::StepDetectBreak procA(_G, u, v);
    my::StepDetectNotBreak procB(_levels, alpha, beta, gamma, _change_history, u, v);

    bool record_changes = true;

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
        else
        {
            if (procA.component_breaks)
            {
                // here process A has detected a component breaking
                // we need to update components, rewind process B changes
                ++_component_max_idx;
                for (auto it = procA.small_component.begin(); it != procA.small_component.end(); ++it)
                {
                    _components[*it] = _component_max_idx;
                }
                _rewind();

                break;
            }
            else
            {
                // process A has finished and detected that no component breaks
                // we have to let process B continue until it detects that, so that the BFS structure remains
                // however, we can stop recording changes to save space
                record_changes = false;
            }
        }

        procB.advance(record_changes);
    }
    // after each run, empty change history
    // NOTE: this takes O(N) to delete all elements, only adds a constant to the total run complexity
    if (!_change_history.empty())
    {
        _change_history = std::stack<ChangeRecord>();
    }
}

bool DynGraph::query_is_connected(Vertex v, Vertex u)
{
    return _components[v] == _components[u];
}

bool DynGraph::query_is_connected(Edge e)
{
    return _components[source(e, _G)] == _components[target(e, _G)];
}


Vertex DynGraph::get_root()
{
    return _r;
}