//
// Created by lucas on 4/3/18.
//

#ifndef BEERPONGPROJECT_BOARDGAME_H
#define BEERPONGPROJECT_BOARDGAME_H
#include "Ball.h"
#include "Player.h"
#include <string>
#include <utility>
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
    explicit Table(vector<Player> players) : players(std::move(players)) {}

    explicit Table(const vector<string> &playersName) {
        for (const auto &it : playersName) {
            players.emplace_back(it);
        }
    }

    Table(const vector<string> &playersName, int nbOfCups) {
        for (const auto &it : playersName) {
            players.emplace_back(it,nbOfCups);
        }
    }

    /** default to 6 cups per player
     *
     * @param playersName
     * @param tableSize
     */
    Table(const vector<string> &playersName, Vec2i tableSize) {
        for (const auto &it : playersName) {
            players.emplace_back(it, 6, tableSize);
        }
    }

    Table(const vector<string> &playersName, int nbOfCups, const Vec2i &tableSize) {
        for (const auto &it : playersName) {
            players.emplace_back(it,nbOfCups,tableSize);
        }
    }

    /** Getters
     */
    vector<Player> getPlayers() const {
        return players;
    }

    Ball getPingPong() const {
        return PingPong;
    }

    /** Setters
     */
    void setPlayers(const vector<Player> &players) {
        Table::players = players;
    }

    void setPingPong(const Ball &PingPong) {
        Table::PingPong = PingPong;
    }

};



#endif //BEERPONGPROJECT_BOARDGAME_H
