#include "change_record.hpp"
#include <iostream>

using namespace boost;

ChangeRecord::ChangeRecord(ChangeRecordType type, Vertex v, Vertex u, int primary_set)
    : type(type), v(v), u(u), primary_set(primary_set)
{
}

ChangeRecord::ChangeRecord(ChangeRecord &&other)
    : type(other.type), v(other.v), u(other.u), primary_set(other.primary_set), old_set(std::move(other.old_set))
{
}

ChangeRecord &ChangeRecord::operator=(ChangeRecord &&other)
{
    if (this != &other)
    {
        type = other.type;
        v = other.v;
        u = other.u;
        primary_set = other.primary_set;
        old_set = std::move(other.old_set);
    }
    return *this;
}

ChangeRecord::~ChangeRecord()
{
    std::cout << "ChangeRecord destuctor of vertex: " << v << std::endl;
    std::cout << "ChangeRecord destuctor of type: " << ((type == ChangeRecordType::AlphaBetaMove) ? "ALPHABETA" : "other") << std::endl;
}