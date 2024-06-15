#include "Move.hpp"
#include "Board.hpp"
#include <string>
#include <cassert>

Move::Move(const std::string& move)
{
    if (move.length() == 4)
    {
        begin = Position::parse(move[0], move[1]);
        end = Position::parse(move[2], move[3]);
    }
}
