#include "Position.hpp"
#include "Board.hpp"

bool Position::is_valid() const
{
    return 0 <= x && x < Board::Width && 0 <= y && y < Board::Height;
}

int Position::get_index() const
{
    assert(is_valid());
    return x + y * Board::Width;
}


