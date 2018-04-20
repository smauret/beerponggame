//
// Created by lucas on 4/3/18.
//

#ifndef BEERPONGPROJECT_BALL_H
#define BEERPONGPROJECT_BALL_H


class Ball {
public:

    Ball() {}

    void setSize(int size) {
        Ball::size = size;
    }

    int getSize() const {
        return size;
    }

private:
    int size; // size in cm of the ball

};


#endif //BEERPONGPROJECT_BALL_H
