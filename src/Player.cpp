//
// Created by lucas on 4/3/18.
//

#include "Player.h"

void Player::removeCup(int id) {
    int i = 0;
    while ((i < cups.size()) && (cups[i].getID() != id)) i++;

    cups.erase(cups.begin() + i);
    cupsLeft--;
}


vector<Vec3<int>> Player::throwBall(){
    float alpha, beta, h, v0, startX;
    alpha = 0.5;
    beta = static_cast<float>(M_PI / 2);
    h=0.0;
    v0=0;
    startX = 0;

    return throwBall(alpha, beta, h, v0, startX);

}

vector<Vec3<int>> Player::throwBall (float alpha, float beta, float h, float v0, float startX){
    vector<Vec3<int>> ballTrajectory;
    float g = 981; //cm.s-2
    float a = -g/(2*cos(alpha)*cos(alpha)*v0*v0);
    float b = tan(alpha);
    float c = h - cups[0].getHeight();
    float d = cos(beta)/cos(alpha);
    int x,z;

    for (int y=0; y<=tableSize.y; y++){
        z = (int)round(a * y * y + b * y + h);
        x = (int)round(y * d + startX);
        ballTrajectory.emplace_back(x,y,z);
        cout << "x = " << ballTrajectory[y].getX() << " | y = " << ballTrajectory[y].getY() << " | z = " << ballTrajectory[y].getZ() << endl;
        if (z < 11) {
            // stop when the ball is lower than the height of a cup: we don't need more information on the trajectory
            break;
        }
    }

    int cupScored = scoreCup(a, b, c, ballTrajectory);

    if (cupScored > -1) {
        removeCup(cupScored);
    }

    return ballTrajectory;
}

int Player::scoreCup(float &a, float &b, float &c, vector<Vec3<int>> &ballTrajectory){
    // only test the last element in ballTrajectory due to its construction
    int score = -1;

    int xSolution = ballTrajectory.back().getX();
    int ySolution = ballTrajectory.back().getY();
    int radius = cups[0].getRadius();

    for (auto &cup : cups) {
        if (cup.isOnTable()) {
            // The cup has not been scored yet
            Vec2i posCup = cup.getPosition();
            float d = sqrtf((xSolution - posCup.x)*(xSolution - posCup.x)+(ySolution - posCup.y)*(ySolution - posCup.y));
            cout << "Cup id: " << cup.getID() << " d = " << d << endl;
            if (d < radius){
                score = cup.getID();
                break;
            }
        }

    }

    return score;
}

vector<Vec2i> Player::cupsPositions(int nbOfCups) {
    vector<Vec2i> positions;
    if (tableSize.y == 240 && tableSize.x == 61){
        if (nbOfCups == 6){
            positions.emplace_back(22,tableSize.y-10);
            positions.emplace_back(31,tableSize.y-10);
            positions.emplace_back(40,tableSize.y-10);
            positions.emplace_back(26,tableSize.y-19);
            positions.emplace_back(35,tableSize.y-19);
            positions.emplace_back(31,tableSize.y-28);
        }
        if (nbOfCups == 10){
            positions.emplace_back(18,tableSize.y-10);
            positions.emplace_back(27,tableSize.y-10);
            positions.emplace_back(38,tableSize.y-10);
            positions.emplace_back(47,tableSize.y-10);
            positions.emplace_back(22,tableSize.y-19);
            positions.emplace_back(31,tableSize.y-19);
            positions.emplace_back(40,tableSize.y-19);
            positions.emplace_back(27,tableSize.y-28);
            positions.emplace_back(39,tableSize.y-28);
            positions.emplace_back(31,tableSize.y-37);
        }
    }
    return positions;
}

bool Player::mooveCup(int cupID, Vec2i newPosition) {

    return false;
}






