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
    Player joueur1 = board.getPlayers()[0];
    vector<Vec3<int>> throwingPositions = joueur1.throwBall(58,0.5,0);

    vector<Vec3<int>> throwingPositions2 = board.getPlayers()[1].throwBall(58,0.5,0);

    Player lucas = Player("Lucas",10);
    cout << "Test position cups avec 10 cups ______________________________" << endl;
    for (int i=0; i<lucas.getCups().size(); i++){
        //cout << "Cups position: (" << lucas.getCups()[i].getPosition().x << "," << lucas.getCups()[i].getPosition().y << ")" << endl;
    }

    /*cout << "Test remove de la cup: ID = 2 ______________________________" << endl;
    for (int i=0; i<lucas.getCups().size(); i++){
        cout << "Cups ID: " << lucas.getCups()[i].getID() << endl;
    }

    lucas.removeCup(2);

    for (int i=0; i<lucas.getCups().size(); i++){
        cout << "Cups ID: " << lucas.getCups()[i].getID() << endl;
    }*/


    vector<Vec3<int>> ballTrajectory;
    ballTrajectory = lucas.throwBall (0.5, M_PI/2.2, 100, 400);
    for (int i=0; i<=240; i++){
        //cout << "x : " << ballTrajectory[i].getX() << "|y : " << ballTrajectory[i].getY() << "|z : " << ballTrajectory[i].getZ() << endl;
    }


    return 0;
}