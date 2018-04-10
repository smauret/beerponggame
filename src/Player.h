//
// Created by lucas on 4/3/18.
//

#pragma once
#include "Cup.h"
#include <string>
#include <vector>
#include "Vec3.h"

using namespace std;

class Player {
private:
    string name;
    vector<Cup> cups;
    int cupsLeft;
    Vec2i tableSize;

public:
    /** Constructeurs
     */
    Player(const string &name) : name(name) {}

    Player(const string &name, int nbOfCups) : name(name), cupsLeft(nbOfCups) {
        for (int i=0; i<cupsLeft; i++){
            cups.push_back(Cup(i));
        }
        tableSize = Vec2i(240,60);
        //cout << Player::name << " créé ! :) :) :)" << endl;
    }


    Player(const string &name, int nbOfCups, Vec2i tableSize) : name(name), cupsLeft(nbOfCups), tableSize(tableSize) {
         for (int i=0; i<cupsLeft; i++){
             cups.push_back(Cup(i));
         }
         //cout << Player::name << " créé ! :) :) :)" << endl;
    }


    Player() {}

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
    const string &getName() const {
        return name;
    }

    const vector<Cup> &getCups() const {
        return cups;
    }

    int getNbOfCups() const {
        return cupsLeft;
    }

    int getCupsLeft() const {
        return cupsLeft;
    }

    const Vec2i &getTableSize() const {
        return tableSize;
    }

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

};


