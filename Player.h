//ECE-6122: Advanced Programming Techniques
//Beer Pong Game
//Najwa Harif, Sarah Mauret, Hugo Blech, Lucas Chaulan, Elliot Hallais

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
    int totalThrows;
    vector<Cup> cups;
    int cupsLeft{};
    Vec2i tableSize;
    bool winner;

    int window_width_pixel;
    int window_height_pixel;
    double ratio_min_max;
    int table_width_max_cm;
    int table_width_min_cm;
    int table_width_max_pixel;
    int table_width_min_pixel;
    int table_length_cm;
    int table_length_pixel_zAxis;
    int ball_size_max_cm;
    int ball_size_min_cm;
    int ball_size_max_pixel;
    int ball_size_min_pixel;


public:
    /** Constructeurs
     */
    explicit Player(string name) : name(std::move(name)) {}

    Player(string name, int nbOfCups) : name(std::move(name)), cupsLeft(nbOfCups) {
        totalThrows=0;
        winner=false;
        tableSize = Vec2i(61, 240);
        vector<Vec2i> positionCups = cupsPositions(nbOfCups);
        for (int i = 0; i < cupsLeft; i++) {
            Cup c (i, positionCups[i]);
            cups.emplace_back(c);
            //cups.emplace_back(i, positionCups[i]);
        }
    }


    Player(string name, int nbOfCups, const Vec2i &tableSize) : name(std::move(name)), cupsLeft(nbOfCups),
                                                                tableSize(tableSize) {
        totalThrows=0;
        for (int i = 0; i < cupsLeft; i++) {
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

    void setTotalThrows(const int &number) {
        Player::totalThrows = number;
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

    int getTotalThrows() {
        return totalThrows;
    }

    vector<Cup> &getCups() {
        return cups;
    }

    Cup &getCup(int id){
        return cups[id];
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

    /** Operator */
    bool operator==(Player otherPlayer){
        return name == otherPlayer.getName();
    }

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
    vector<Vec3<int>> throwBall(int &cupScored);


    /** Throw the ball for a human player
     *
     * @param alpha angle between shot and depth (with y)
     * @param beta lateral angle (in (xz) plan)
     * @param h hight of the ball at throwing
     * @param v0 fast of the throw
     * @param startX the initial condition in x for the trajectory
     * @param startY the initial condition in y for the trajectory
     * @param cupScored to stock the cup scored, -1 = no cup scored, otherwhise it's the ID of the cup scored
     * @return the trajectory vector that contain all the ball positions
     */
    vector<Vec3<int>> throwBall(double alpha, double beta, double h, double v0, double startX, int startY,
                                int &cupScored);

    /** Indicate if a ball scored a cup, return othe id of the cup to remove, -1 if no cup should be remooved
     *
     * @param a
     * @param b
     * @param c
     * @param ballTrajectory
     * @return -1 = no score, int > 0 = score cup of id indicated
     */
    int scoreCup(vector<Vec3<int>> &ballTrajectory);

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

    /** update winner tu true
    */
    void win();

    /** Return whereas or no if the player has win the game
     *
     * @return
     */
    bool hasWon();

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

    /** include the high of the ball in the graphics trajectory
     *
     * @param ballTrajectory The vector containing the position of the ball over the time, in cm (Architecture)
     * @param graphicsTrajectory The vector of 2D points for the graphic trajectory, write only x values in it
     */
    void include_zArchi_graphics(vector<Vec3<int>> &ballTrajectory, vector<Vec3<int>> &graphicsTrajectory);

    /** Get ball size value for graphics from the architecture trajectory
     *
     * @param ballTrajectory The vector containing the position of the ball over the time, in cm (Architecture)
     * @param graphicsTrajectory The vector of 2D points for the graphic trajectory, write only x values in it
     */
    void get_ball_size(vector<Vec3<int>> &ballTrajectory, vector<Vec3<int>> &graphicsTrajectory);

    /** Convert the architecture trajectory (stored in cm) into graphics position for plotting purpose (in pixel).
     *
     * @param ballTrajectory The trajectory of the ball in the architecture (in cm)
     * @param graphicsTrajectory The trajectory that will be plotted, vector is updated with the correct values (in pixels)
     * @param cupScored updated with: -1 if no cup is scored, otherwise with the ID of the cup scored
     */
    void get_xzSize_graphics(vector<Vec3<int>> &ballTrajectory, vector<Vec3<int>> &graphicsTrajectory, int &cupScored);
};