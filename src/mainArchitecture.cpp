#include <iostream>

#include "Table.h"
using namespace std;


int main() {
    vector<string> playerNames;
    playerNames.emplace_back("Lucas");
    playerNames.emplace_back("Najwa");
    Table board = Table(playerNames, 5, Vec2i(240,61));
    //for (const auto &it : board.getPlayers()) {
    //    cout << it.getName() << endl;
    //}

    Player lucas = Player("Lucas",10);
    cout << "Test position cups avec 10 cups ______________________________" << endl;
    vector<Vec3<int>> ballTrajectory;
    ballTrajectory = lucas.throwBall (0.5, M_PI/2.2, 100, 400);
    for (int i=0; i<=240; i++){
        //cout << "x : " << ballTrajectory[i].getX() << "|y : " << ballTrajectory[i].getY() << "|z : " << ballTrajectory[i].getZ() << endl;
    }

    cout << "Cups id:";
    for (auto &cup : lucas.getCups()) {
        cout << ", " << cup.getID();
    }
    cout << "." << endl;
    cout << "test" << endl;



    return 0;
}