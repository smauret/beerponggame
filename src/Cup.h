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
    Vec2i position;
    bool isActive;
public:
    Cup() {}

    Cup(int ID, const Vec2i &position) : ID(ID), position(position) {}

    Cup(int ID) : ID(ID) {std::cout << "Cups " << Cup::ID << " crée ! :)" << std::endl;}

};


#endif //BEERPONGPROJECT_CUP_H
