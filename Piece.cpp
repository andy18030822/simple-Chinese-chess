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
    "  ", 
    "\033[0;31m兵\033[0m", "\033[0;31m炮\033[0m", 
    "\033[0;31m车\033[0m", "\033[0;31m马\033[0m", 
    "\033[0;31m相\033[0m", "\033[0;31m士\033[0m", 
    "\033[0;31m帅\033[0m",
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

bool Piece::equals_ignore_color(Piece piece) const 
{
    return (data & 0b111) == (piece.data & 0b111);
}
