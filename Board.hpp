#pragma once
#include <vector>
#include "Piece.hpp"

class Board
{
public:
    void print_board();
    Board(const std::string& fen_string);

    static constexpr int Width = 9;
    static constexpr int Height = 10;

private:
    std::vector<Piece> pieces{Width * Height};
    bool black_turn = false;
};