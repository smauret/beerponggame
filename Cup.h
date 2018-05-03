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
    Vec2f position; //position du centre de la cup
    bool onTable;
    float radius; //en cm
    float height; //en cm
public:

    /** constructeurs
 *
 */
    Cup() {onTable = true;}

    Cup(int ID, const Vec2f &position, float height, float radius) : ID(ID), position(position), height(height), radius(radius) {onTable = true;}

    Cup(int ID, const Vec2f &position) : ID(ID), position(position) {height = 12; radius = 4.5; onTable = true;}

    Cup(int ID) : ID(ID) {/*std::cout << "Cups " << Cup::ID << " crée ! :)" << std::endl;*/onTable = true;}

    /** getters
     *
     */
    int getID() const {
        return ID;
    }

    const Vec2f &getPosition() const {
        return position;
    }

    bool isOnTable() const {
        return onTable;
    }

    float getRadius() const {
        return radius;
    }

    float getHeight() const {
        return height;
    }

    /** setters
     *
     */
    void setID(int ID) {
        this->ID = ID;
    }

    void setPosition(const Vec2f &position) {
        this->position = position;
    }

    void setOnTable(bool onTable) {
        this->onTable = onTable;
    }

    void setRadius(float radius) {
        this->radius = radius;
    }

    void setHeight(float height) {
        this->height = height;
    }

};

#endif //BEERPONGPROJECT_CUP_H
