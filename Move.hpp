#pragma once
#include <string>

class Move
{
public:
    Move() : begin(-1), end(-1) {}
    Move(const std::string& move);

    bool is_valid() const;
    int get_begin() const { return begin; }
    int get_end() const { return end; }


private:
    int begin;
    int end;
};