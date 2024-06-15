#include <iostream>
#include "Piece.hpp"
#include "Board.hpp"



// 空   = 0
// 卒兵 = 1  pP
// 砲炮 = 2  cC
// 車车 = 3  rR
// 馬马 = 4  hH
// 象相 = 5  eE
// 仕士 = 6  aA
// 将帅 = 7  kK
// 
// 
// 
// 




int main()
{

	Board board("rheakaehr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RHEAKAEHR w");
	board.print_board();

	

	
    return 0;
}