//
// Created by lucas on 4/3/18.
//

#pragma once
#include "Cup.h"
#include <string>
#include <vector>

using namespace std;

class Player {
public:
    /** Constructeurs
     */
    Player(const string &name) : name(name) {}

    Player(const string &name, int nbOfCups) : name(name), nbrOfCups(nbOfCups) {
        for (int i=0; i<nbrOfCups; i++){
            cups.push_back(Cup(i));
        }
        cout << Player::name << " créé ! :) :) :)" << endl;
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
        Player::nbrOfCups = nbOfCups;
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
        return nbrOfCups;
    }

private:
    string name;
    vector<Cup> cups;
    int nbrOfCups;


};


