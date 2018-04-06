#include <iostream>

#include "BoardGame.h"
using namespace std;


int main() {
    BoardGame board = BoardGame(1,2,3, "Lucas", "Najwa", 5);
    board.startGame();
    board.throwBall(board.getPlayer1(), 0,0,0);
    return 0;
}