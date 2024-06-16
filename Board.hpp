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

    void print_board() const;
    void make_move(Move move);
    bool undo_move();
    bool get_black_turn() { return black_turn; }

    std::vector<Move> get_moves() const;
    std::vector<Move> get_legal_moves();
    Position find_king(bool opponent) const;
    uint64_t perft(int depth);

    int evaluate() const;
    Move find_best_move();
    int find_highest_score(int depth);

    static constexpr int Width = 9;
    static constexpr int Height = 10;

private:
    void try_take(Move move, std::vector<Move>& moves) const
    {
        Piece piece = pieces[move.end.get_index()];
        bool can_take = piece.is_empty() || piece.is_black() != black_turn;
        if (can_take) moves.push_back(move);
    }

    void fill_pawn_moves(Position position, std::vector<Move>& moves) const;
    void fill_cannon_moves(Position position, std::vector<Move>& moves) const;
    void fill_rook_moves(Position position, std::vector<Move>& moves) const;
    void fill_horse_moves(Position position, std::vector<Move>& moves) const;
    void fill_elephant_moves(Position position, std::vector<Move>& moves) const;
    void fill_advisor_moves(Position position, std::vector<Move>& moves) const;
    void fill_king_moves(Position position, std::vector<Move>& moves) const;

    std::vector<Piece> pieces{Width * Height};
    std::vector<std::pair<Move, Piece>> move_history;
    bool black_turn = false;
};