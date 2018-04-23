//
// Created by lucas on 4/3/18.
//

#pragma once
#include "Cup.h"
#include <string>
#include <utility>
#include <vector>
#include "Vec3.h"

using namespace std;

class Player {
private:
    string name;
    vector<Cup> cups;
    int cupsLeft{};
    Vec2i tableSize;

public:
    /** Constructeurs
     */
    explicit Player(string name) : name(std::move(name)) {}

    Player(string name, int nbOfCups) : name(std::move(name)), cupsLeft(nbOfCups) {
        tableSize = Vec2i(61,240);
        vector<Vec2i> positionCups = cupsPositions(nbOfCups);
        for (int i=0; i<cupsLeft; i++){
            cups.emplace_back(i,positionCups[i]);
        }
    }


    Player(string name, int nbOfCups, const Vec2i &tableSize) : name(std::move(name)), cupsLeft(nbOfCups), tableSize(tableSize) {
        for (int i=0; i<cupsLeft; i++){
            cups.emplace_back(i);
        }
        //cout << Player::name << " créé ! :) :) :)" << endl;
    }


    Player() = default;

    /** Setters
     */
    void setName(const string &name) {
        Player::name = name;
    }

    void setCups(const vector<Cup> &cups) {
        Player::cups = cups;
    }

    void setNbOfCups(int nbOfCups) {
        Player::cupsLeft = nbOfCups;
    }

    void setCupsLeft(int cupsLeft) {
        Player::cupsLeft = cupsLeft;
    }

    void setTableSize(const Vec2i &tableSize) {
        Player::tableSize = tableSize;
    }

    /** Getters
    */
    string &getName() {
        return name;
    }

    vector<Cup> &getCups() {
        return cups;
    }

    int getNbOfCups() {
        return cupsLeft;
    }

    int getCupsLeft() {
        return cupsLeft;
    }

    Vec2i &getTableSize() {
        return tableSize;
    }


    /** Iterators */
    typedef vector<Cup>::iterator iterator;
    typedef vector<Cup>::const_iterator const_iterator;
    iterator begin() { return cups.begin(); }
    iterator end() { return cups.end(); }

    /** Methods
     *
     */

    /** Remove the cup from the game when it's scoded*
     *
     * @param id id of the cup to remove
     */
    void removeCup(int id);


    /** Throw the ball for a computer
     *
     * @return all the positions taken by the ball
     */
    vector<Vec3<int>> throwBall();


    /** Throw the ball for a human player
     *
     * @param alpha angle between shot and depth (with y)
     * @param beta lateral angle (in (xz) plan)
     * @param h hight of the ball at throwing
     * @param v0 fast of the throw
     * @return the trajectory vector that contain all the ball positions
     */
    vector<Vec3<int>> throwBall (float alpha, float beta, float h, float v0, float startX);

    /** Indicate if a ball scored a cup, return othe id of the cup to remove, -1 if no cup should be remooved
     *
     * @param a
     * @param b
     * @param c
     * @param ballTrajectory
     * @return -1 = no score, int > 0 = score cup of id indicated
     */
    int scoreCup(float &a, float &b, float &c, vector<Vec3<int>> &ballTrajectory);

    /** Give all the positions the cups should take at the beginning of the game, depending on the nulber of cups
     *
     * @param nbOfCups number of cups
     * @return starting positions
     */
    vector<Vec2i> cupsPositions(int nbOfCups);


    /** Movve the cup to a new position
     *
     * @param cupID ID of the cup to moove
     * @param newPosition The position where to moove the cup
     * @return true = worked, false = did not work (other cup at the same position for example)
     */
    bool mooveCup(int cupID, Vec2i newPosition);

    /** Find the correct zGraphic values depending on the yArchitecture values, write them in a vector of 2d points for graphics purpose
     *
     * @param ballTrajectory the vector containing the position of the ball over the time, in cm (Architecture)
     * @param graphicsTrajectory The vector of 2D points for the graphic trajectory, write only x values in it
     */
    void get_z_graphics(vector<Vec3<int>> &ballTrajectory, vector<Vec3<int>> &graphicsTrajectory);

    /** Get xGraphic value for graphics from the architecture trajectory
     *
     * @param ballTrajectory The vector containing the position of the ball over the time, in cm (Architecture)
     * @param graphicsTrajectory The vector of 2D points for the graphic trajectory, write only x values in it
     */
    void get_x_graphics(vector<Vec3<int>> &ballTrajectory, vector<Vec3<int>> &graphicsTrajectory);

    void inverse_z_graphics(vector<Vec3<int>> &graphicsTrajectory);
};