#include "Move.hpp"
#include "Board.hpp"
#include <string>
#include <cassert>

static int parse_position(char x, char y)
{
    x = tolower(x);

    int position_x = x - 'a';
    int position_y = y - '0';

    if (position_x < 0 || position_x >= Board::Width) return -1;
    if (position_y < 0 || position_y >= Board::Height) return -1;

    return position_x + position_y * Board::Width;
}

Move::Move(const std::string& move)
{
    if (move.length() == 4)
    {
        begin = parse_position(move[0], move[1]);
        end = parse_position(move[2], move[3]);
    }
    else
    {
        begin = end = -1;
    }
}

static bool is_valid_position(int position)
{
    return 0 <= position && position < Board::Width * Board::Height;
}

bool Move::is_valid() const
{
    return is_valid_position(begin) && is_valid_position(end);
}