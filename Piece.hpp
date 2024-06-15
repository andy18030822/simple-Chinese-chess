#pragma once
#include <string>
#include <cassert>

// 空   = 0
// 卒兵 = 1  pP
// 砲炮 = 2  cC
// 車车 = 3  rR
// 馬马 = 4  hH
// 象相 = 5  eE
// 仕士 = 6  aA
// 将帅 = 7  kK

class Piece 
{
public:
    Piece() : data(0) {}
    explicit Piece(char fen);

    bool is_empty() const { return data == 0; }
    
    bool is_black() const 
    {    
        assert(not is_empty());
        return data >= 8;
    }

    bool equals_ignore_color(Piece piece) const;

    std::string to_string() const;
    
    static Piece pawn(bool black = false) { return Piece(1, black); }
    static Piece cannon(bool black = false) { return Piece(2, black); }
    static Piece rook(bool black = false) { return Piece(3, black); }
    static Piece horse(bool black = false) { return Piece(4, black); }
    static Piece elephant(bool black = false) { return Piece(5, black); }
    static Piece advisor(bool black = false) { return Piece(6, black); }
    static Piece king(bool black = false) { return Piece(7, black); }

private:
    Piece(int data, bool black) : data(data + (black ? 8 : 0)) {}

    int data;
};