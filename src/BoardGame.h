//
// Created by lucas on 4/3/18.
//

#ifndef BEERPONGPROJECT_BOARDGAME_H
#define BEERPONGPROJECT_BOARDGAME_H
#include "Ball.h"
#include "Player.h"
#include <string>
#include <vector>

using namespace std;

class BoardGame {
private:
    int sizeX, sizeY, sizeZ;
    Player player1, computer;
    Ball PingPong;
public:
    /** Constructeurs
     */
    BoardGame(int sizeX, int sizeY, int sizeZ) : sizeX(sizeX), sizeY(sizeY), sizeZ(sizeZ) {}

    BoardGame(int sizeX, int sizeY, int sizeZ, string player1, string computer) : sizeX(sizeX), sizeY(sizeY), sizeZ(sizeZ) {
        BoardGame::player1 = Player(player1);
        BoardGame::computer = Player(computer);
    }

    BoardGame(int sizeX, int sizeY, int sizeZ, string player1, string computer, int nbrOfCups) : sizeX(sizeX), sizeY(sizeY), sizeZ(sizeZ) {
        BoardGame::player1 = Player(player1, nbrOfCups);
        BoardGame::computer = Player(computer, nbrOfCups);
    }

    /** Getters
     */
    int getSizeX() const {
        return sizeX;
    }

    int getSizeY() const {
        return sizeY;
    }

    int getSizeZ() const {
        return sizeZ;
    }

    const Player &getPlayer1() const {
        return player1;
    }

    const Player &getComputer() const {
        return computer;
    }

    const Ball &getPingPong() const {
        return PingPong;
    }

    /** Setters
     */
    void setSizeX(int sizeX) {
        BoardGame::sizeX = sizeX;
    }

    void setSizeY(int sizeY) {
        BoardGame::sizeY = sizeY;
    }

    void setSizeZ(int sizeZ) {
        BoardGame::sizeZ = sizeZ;
    }

    void setPlayer1(const Player &player1) {
        BoardGame::player1 = player1;
    }

    void setComputer(const Player &computer) {
        BoardGame::computer = computer;
    }

    void setPingPong(const Ball &PingPong) {
        BoardGame::PingPong = PingPong;
    }

        /** fonction start game, juste pour tester
         *
         */
    void startGame();


    void lancer
};



#endif //BEERPONGPROJECT_BOARDGAME_H
