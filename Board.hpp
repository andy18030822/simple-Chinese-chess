#pragma once
#include <vector>
#include <string>
#include "Piece.hpp"
#include "Move.hpp"

class Board
{
public:
    Board();
    Board(const std::string& fen_string);

    void print_board();

    static constexpr int Width = 9;
    static constexpr int Height = 10;
    void make_move(Move move);

private:
    std::vector<Piece> pieces{Width * Height};
    bool black_turn = false;
};