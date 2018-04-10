#include <iostream>

#include "Table.h"
using namespace std;


int main() {
    Table board = Table(60,240,100, "Lucas", "Najwa", 5);
    vector<Vec3<int>> throwingPositions = board.throwBall(board.getPlayer1(), 58,0.5,0);
    /*for (int i = 0; i < throwingPositions.size(); i++){
        cout << "(x,y,z) = (" << throwingPositions[i].getX() << ", " << throwingPositions[i].getY() << ", " << throwingPositions[i].getZ() << ")" << endl;
    }*/

    Player lucas = Player("Lucas",10);
    for (int i=0; i<lucas.getCups().size(); i++){
        cout << "Cups ID: " << lucas.getCups()[i].getID() << endl;
    }

    lucas.removeCup(5);

    for (int i=0; i<lucas.getCups().size(); i++){
        cout << "Cups ID: " << lucas.getCups()[i].getID() << endl;
    }
    return 0;
}