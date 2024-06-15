#include "Board.hpp"
#include <iostream>
#include <map>

Board::Board(const std::string& fen_string)
{
    int read = 0;
    int x = 0;
    int y = 9;

    for (char fen : fen_string)
    {
        read++;
        if (fen==' ') break;

        if (fen=='/')
        {
            x = 0;
            y--;
        }
        else if ('1' <= fen && fen <= '9')
        {
            int gap = fen - '0';
            x += gap;
        }
        else
        {
            int index = x + y * Width;
            pieces[index] = Piece(fen);
            x++;
        }
    }

    black_turn = (fen_string[read] == 'b');
}

void Board::print_board()
{
    if(black_turn)
    {
        std::cout << "black turn" << std::endl;
    }
    else
    {
        std::cout << "red turn" << std::endl;
    }

    for (int y = 9; y >= 0; y--)
    {
        std::cout << y << " ";
        for (int x = 0; x < 9; x++)
        {
            int index = x + y * Width;
            std::cout << pieces[index].to_string();
        }
        
        std::cout<<std::endl;
    }
    std::cout << "  A B C D E F G H I" << std::endl;
}
