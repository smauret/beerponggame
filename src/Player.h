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
        tableSize = Vec2i(240,61);
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



    /** Fonction lancé de balle pour un joueur humain
     *
     * @param power la puissance du lancé, entre 0 et 1
     * @param angle l'angle du tire, entre 0 et 1
     * @param translation la translation latéral du joueur, entre 0 et 1
     * @return le vecteur qui représente l'ensemble des positions prises par la balle lors du lancé, la coordonnée y représente la profondeur du la table
     */
    vector<Vec3<int>> throwBall(float power, float angle, float translation);



    /** Fonction lancé de balle pour un joueur computeur
     *
     * @return Le vecteur qui represente l'ensemble des positions prises par la balle lors du lancé
     */
    // on peu calculer comme dans le cas joueur humain, mais il faudrait remplir le vecteur dans l'autre sens je pense (la balle est lancée depuis l'autre coté de la table)
    vector<Vec3<int>> throwBall();


    vector<Vec3<int>> throwBall (float alpha, float beta, float h, float v0);
    bool scoreCup(float &a, float &b, float &c, vector<Vec3<int>> &ballTrajectory);

    /** retourne la liste des positions des cups en fonction du nombre de cups avec lequel on jous
     *
     * @param nbOfCups le nombre de cups
     * @return les positions des cups au début de la partie
     */
    vector<Vec2i> cupsPositions(int nbOfCups);


    /** Movve the cup to a new position
     *
     * @param cupID ID of the cup to moove
     * @param newPosition The position where to moove the cup
     * @return true = worked, false = did not work (other cup at the same position for example)
     */
    bool mooveCup(int cupID, Vec2i newPosition);


};


