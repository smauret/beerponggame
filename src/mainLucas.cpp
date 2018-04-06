#include <iostream>

#include "BoardGame.h"
using namespace std;


int main() {
    BoardGame board = BoardGame(60,240,100, "Lucas", "Najwa", 5);
    board.startGame();
    vector<Vec3<int>> throwingPositions = board.throwBall(board.getPlayer1(), 58,0.5,0);
    for (int i = 0; i < throwingPositions.size(); i++){
        cout << "(x,y,z) = (" << throwingPositions[i].getX() << ", " << throwingPositions[i].getY() << ", " << throwingPositions[i].getZ() << ")" << endl;
    }
    return 0;
}