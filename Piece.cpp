#include "Piece.hpp"
#include <vector>
#include <map>

static const std::map<char, Piece> FenStringToPiece
{
    {'P', Piece::pawn(false)},
    {'C', Piece::cannon(false)},
    {'R', Piece::rook(false)},
    {'H', Piece::horse(false)},
    {'E', Piece::elephant(false)},
    {'A', Piece::advisor(false)},
    {'K', Piece::king(false)},
    {'p', Piece::pawn(true)},
    {'c', Piece::cannon(true)},
    {'r', Piece::rook(true)},
    {'h', Piece::horse(true)},
    {'e', Piece::elephant(true)},
    {'a', Piece::advisor(true)},
    {'k', Piece::king(true)},
};

static const std::vector<std::string> PieceToString =
{
    "  ", "兵", "炮", "车", "马", "相", "士", "帅",
    "  ", "卒", "砲", "車", "馬", "象", "仕", "将"
};

std::string Piece::to_string() const
{
    return PieceToString[data];
}

Piece::Piece(char fen)
{
    *this = FenStringToPiece.at(fen);
}
