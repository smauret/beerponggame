//ECE-6122: Advanced Programming Techniques
//Beer Pong Game
//Najwa Harif, Sarah Mauret, Hugo Blech, Lucas Chaulan, Elliot Hallais

#ifndef BEERPONGPROJECT_CUP_H
#define BEERPONGPROJECT_CUP_H


#include <iostream>
#include "src/Vec2.h"
#include <vector>

class Cup {
private:
    int ID;
    Vec2i position; //position du centre de la cup
    bool onTable;
    float radius; //en cm
    int height; //en cm
public:

    /** constructeurs
 *
 */
    Cup() {onTable = true;}

    Cup(int ID, const Vec2i &position, int height, int radius) : ID(ID), position(position), height(height), radius(radius) {onTable = true;}

    Cup(int ID, const Vec2i &position) : ID(ID), position(position) {height = 12; radius = 4.5; onTable = true;}

    Cup(int ID) : ID(ID) {/*std::cout << "Cups " << Cup::ID << " crée ! :)" << std::endl;*/onTable = true;}

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

    int getRadius() const {
        return radius;
    }

    int getHeight() const {
        return height;
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

    void setRadius(int radius) {
        Cup::radius = radius;
    }

    void setHeight(int height) {
        Cup::height = height;
    }

};

#endif //BEERPONGPROJECT_CUP_H
