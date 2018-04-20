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
    }

    int cupScored = scoreCup(a, b, c, ballTrajectory);

    if (cupScored > -1) {
        removeCup(cupScored);
    }

    return ballTrajectory;
}

int Player::scoreCup(float &a, float &b, float &c, vector<Vec3<int>> &ballTrajectory){
    int score = -1;
    float delta = b * b - 4 * a * c;
    int y1, y2;
    y1 = (int)round((-b -sqrtf(delta)) / (2 * a));
    y2 = (int)round((-b +sqrtf(delta)) / (2 * a));
    int ySolution;
    if(y1 > 0 && y1 > y2)
        ySolution = y1;
    else
        ySolution = y2;

    int xSolution = ballTrajectory[ySolution].getX();
    int radius = cups[0].getRadius();

    for (auto &cup : cups) {
        //cout << "x sol = " << xSolution << "  y sol = " << ySolution << " cup x = " << itCup->getPosition().x << " cup y = " << itCup->getPosition().y <<endl;
        bool onTable = cup.isOnTable();
        if (cup.isOnTable()) {
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






