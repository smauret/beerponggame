#include <iostream>

#include "BoardGame.h"
using namespace std;


int main() {
    BoardGame board = BoardGame(1,2,3, "Lucas", "Najwa", 5);
    board.startGame();
    return 0;
}