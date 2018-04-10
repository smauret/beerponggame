//
// Created by lucas on 4/3/18.
//

#ifndef BEERPONGPROJECT_BOARDGAME_H
#define BEERPONGPROJECT_BOARDGAME_H
#include "Ball.h"
#include "Player.h"
#include <string>
#include <vector>
#include "Vec3.h"
#include <math.h>


using namespace std;

class Table {
private:
    vector<Player> players;
    Ball PingPong;
public:
    /** Constructeurs
     */


    /** Getters
     */
    const vector<Player> getPlayers() const {
        return players;
    }

    const Ball getPingPong() const {
        return PingPong;
    }

    /** Setters
     */
    void setPlayers(const vector<Player> &players);

    void setPingPong(const Ball &PingPong);

    /** fonction start game, juste pour tester
     *
     */
    void startGame();



};



#endif //BEERPONGPROJECT_BOARDGAME_H
