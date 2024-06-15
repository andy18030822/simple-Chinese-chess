#pragma once
#include "Position.hpp"

#include <string>

class Move
{
public:
    Move() {}
    Move(const std::string& move);

    bool is_valid() const { return begin.is_valid() && end.is_valid(); }
    Position get_begin() const { return begin; }
    Position get_end() const { return end; }

    std::string to_string() const { return begin.to_string() + end.to_string(); }

    bool operator==(Move other) const { return begin == other.begin && end == other.end; }

    Position begin;
    Position end;
};

namespace std
{
    template<>
    class hash<Move>
    {
    public:
        size_t operator()(Move move) const noexcept
        {
            static_assert(sizeof(Move) == sizeof(uint32_t));
            return *reinterpret_cast<uint32_t*>(&move);
        }
    };
}