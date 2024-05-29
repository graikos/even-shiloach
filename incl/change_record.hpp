#ifndef CHANGE_RECORD_HPP
#define CHANGE_RECORD_HPP
#include "graph.hpp"
#include "edge_set.hpp"

enum class ChangeRecordType
{
    LevelBump,
    Insert,
    Remove,
    AlphaBetaMove,
    RestoreBeta,
    BetaGammaMove,
    GammaEmptyMove,
};

class ChangeRecord
{
public:
    ChangeRecordType type;
    Vertex v;        // considered the primary vertex concerned when dealing with edge sets
    Vertex u;        // ignored when not Insert or Remove
    int primary_set; // 0: alpha, 1: beta, 2: gamma
    EdgeSet old_set; // ignored when not AlphaBetaMove

    ChangeRecord(ChangeRecordType type, Vertex v, Vertex u, int primary_set);

    ChangeRecord(ChangeRecord &&other);
    ChangeRecord &operator=(ChangeRecord &&other);

    ~ChangeRecord();
};

#endif