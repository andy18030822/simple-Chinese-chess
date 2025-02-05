#include <iostream>
#include "Piece.hpp"
#include "Board.hpp"
#include "Move.hpp"
#include <string>
#include <unordered_set>

int main()
{
	Board board;
	Move move;

	while (true)
	{
		board.print_board();

		std::vector<Move> moves = board.get_legal_moves();

		std::unordered_set<Move> set(moves.begin(), moves.end());
		std::cout << set.size() << " available moves" << std::endl;
		std::cout << "evaluation: " << board.evaluate() << std::endl;

		if (board.get_black_turn())
		{
			int score = 0;
			int depth = 0;
			Move best_move = board.find_best_move(score, depth);

			if (not best_move.is_valid())
			{
				std::cout << "you win :(" << std::endl;
				break;
			}

			std::cout << "found " << score << " after searching depth " << depth << std::endl;
			
			board.make_move(best_move);
			continue;
		}

		while (true)
		{
			std::string input;
			std::cout << "input move: ";
			std::cin >> input;

			if (input == "undo")
			{
				if (board.undo_move() && board.undo_move()) std::cout << "success" << std::endl;
				else std::cout << "no more moves to undo" << std::endl;
				break;
			}

			move = Move(input);

			if (not move.is_valid())
			{
				std::cout << "bad format" << std::endl;
				continue;
			}

			if (set.find(move) == set.end())
			{
				std::cout << "illegal move" << std::endl;
				continue;
			}

			board.make_move(move);
			break;
		}
	}
	
    return 0;
}