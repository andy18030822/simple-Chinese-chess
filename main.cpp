#include <iostream>
#include "Piece.hpp"
#include "Board.hpp"
#include "Move.hpp"
#include <string>

int main()
{
	Board board;
	Move move;

	while (true)
	{
		board.print_board();

		do
		{
			std::string input_move;
			std::cout << "input move: ";
			std::cin >> input_move;

			move = Move(input_move);

			if (not move.is_valid())
			{
				std::cout << "bad format" << std::endl;
			}
		}
		while (not move.is_valid());

		board.make_move(move);
	}
	
    return 0;
}