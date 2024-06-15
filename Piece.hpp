#pragma once
#include <string>

class Piece 
{
public:
    Piece() : data(0) {}
    explicit Piece(char fen);

    bool is_black() const { return data >= 8; }
    std::string to_string() const;
    
    static Piece pawn(bool black) { return Piece(1, black); }
    static Piece cannon(bool black) { return Piece(2, black); }
    static Piece rook(bool black) { return Piece(3, black); }
    static Piece horse(bool black) { return Piece(4, black); }
    static Piece elephant(bool black) { return Piece(5, black); }
    static Piece advisor(bool black) { return Piece(6, black); }
    static Piece king(bool black) { return Piece(7, black); }

private:
    Piece(int data, bool black) : data(data + (black ? 8 : 0)) {}

    int data;
};