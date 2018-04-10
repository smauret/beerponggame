//
// Created by lucas on 4/3/18.
//

#ifndef BEERPONGPROJECT_CUP_H
#define BEERPONGPROJECT_CUP_H


#include <iostream>
#include "Vec2.h"

class Cup {
private:
    int ID;
    Vec2i position; //position du centre de la cup
    bool onTable;
    int rayon; //en cm
    int hauteur; //en cm
public:

    /** getters
     *
     */
    int getID() const {
        return ID;
    }

    const Vec2i &getPosition() const {
        return position;
    }

    bool isOnTable() const {
        return onTable;
    }

    int getRayon() const {
        return rayon;
    }

    int getHauteur() const {
        return hauteur;
    }

    /** setters
     *
     */
    void setID(int ID) {
        Cup::ID = ID;
    }

    void setPosition(const Vec2i &position) {
        Cup::position = position;
    }

    void setOnTable(bool onTable) {
        Cup::onTable = onTable;
    }

    void setRayon(int rayon) {
        Cup::rayon = rayon;
    }

    void setHauteur(int hauteur) {
        Cup::hauteur = hauteur;
    }

public:
    Cup() {}

    Cup(int ID, const Vec2i &position, int hauteur, int rayon) : ID(ID), position(position), hauteur(hauteur), rayon(rayon) {}

    Cup(int ID, const Vec2i &position) : ID(ID), position(position) {hauteur = 12; rayon = 10;}

    Cup(int ID) : ID(ID) {/*std::cout << "Cups " << Cup::ID << " crée ! :)" << std::endl;*/}

};


#endif //BEERPONGPROJECT_CUP_H
