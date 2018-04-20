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

vector<Vec3<int>> Player::throwBall(float power, float angle, float lateralOffset){
    double Vx, Vy, Vz; // les vitesses d'origine selon x y et z
    double convertAngle; // l'angle de lance converti entre -Pi/4 et Pi/4
    int convertOffset;  // le décalage de lance convertie dans la table (entre 0 et sizeX)
    int x0, y0, z0; // les coordonnées du lancé apres decalage

    /*power et angle entre 0 et 1 => les convertir
     * angle entre -45 et 45
     *  angle = angle * 90 -45
     *
     * power: a determiner
     *
     * Position: determiner x0, y0 et z0
     *
     * Offset: 0 = (0,0,0), 1
     */
    convertAngle = angle * M_PI/2 - M_PI/4;

    Vx = power * sin(convertAngle) / sqrt(2);
    Vy = power * cos(convertAngle) / sqrt(2);
    Vz = power / sqrt(2);
    /*cout << "angle = " << convertAngle << endl;
    cout << "Vx = " << Vx << endl;
    cout << "Vy = " << Vy << endl;*/


    y0 = 30; //le bras du joueur 30 cm
    x0 = (int)floor(lateralOffset * tableSize.x);
    z0 = 90; // le joueur lance la balle a 90cm de la table (grand)

    /** calcul des positions succesives de la balle
     */

    int x, y, z;
    vector<Vec3<int>> throwingPositions;
    for (int i = 0; i<tableSize.y; i++){
        y = i;
        x = (int)floor(Vx * (y - y0) / Vy) + x0;
        z = (int)floor(-9.81*y*y/(Vy*Vy)) + y + z0;
        //cout << "(x,y,z) = (" << x << ", " << y << ", " << z << ")" << endl;
        throwingPositions.emplace_back(x,y,z);
    }
    return throwingPositions;
}


vector<Vec3<int>> Player::throwBall(){

}

vector<Vec3<int>> Player::throwBall (float alpha, float beta, float h, float v0){
    vector<Vec3<int>> ballTrajectory;
    float g = 981; //cm.s-2
    float a = -g/(2*cos(alpha)*cos(alpha)*v0*v0);
    float b = tan(alpha);
    float c = h - cups[0].getHeight();
    float d = cos(beta)/cos(alpha);
    int x,z;

    for (int y=0; y<=tableSize.x; y++){
        z = (int)round(a * y * y + b * y + h);
        x = (int)round(y * d);
        ballTrajectory.emplace_back(x,y,z);
        cout << "x = " << x << " | y = " << y << " | z = " << z << endl;
    }

    Player::scoreCup(a, b, c, ballTrajectory);

    return ballTrajectory;
}

bool Player::scoreCup(float &a, float &b, float &c, vector<Vec3<int>> &ballTrajectory){
    bool score = false;
    float delta = b * b - 4 * a * c;
    int y1 = (int)round((-b -sqrtf(delta)) / (2 * a));
    int y2 = (int)round((-b +sqrtf(delta)) / (2 * a));
    int ySolution;
    if(y1 > 0 && y1 > y2)
        ySolution = y1;
    else
        ySolution = y2;

    int xSolution = ballTrajectory[ySolution].getX();
    int radius = cups[0].getRadius();

    for (auto itCup = cups.begin(); itCup != cups.end(); itCup++){
        bool onTable = itCup->isOnTable();
        if (onTable == false)
            score = false;
        else{
            Vec2i posCup = itCup->getPosition();
            float d = 3;
            cout << d;
        }
    }



    return score;
}

vector<Vec2i> Player::cupsPositions(int nbOfCups) {
    vector<Vec2i> positions;
    if (tableSize.x == 240 && tableSize.y == 61){
        if (nbOfCups == 6){
            positions.emplace_back(22,10);
            positions.emplace_back(31,10);
            positions.emplace_back(40,10);
            positions.emplace_back(26,19);
            positions.emplace_back(35,19);
            positions.emplace_back(31,28);
        }
        if (nbOfCups == 10){
            positions.emplace_back(18,10);
            positions.emplace_back(27,10);
            positions.emplace_back(38,10);
            positions.emplace_back(47,10);
            positions.emplace_back(22,19);
            positions.emplace_back(31,19);
            positions.emplace_back(40,19);
            positions.emplace_back(27,28);
            positions.emplace_back(39,28);
            positions.emplace_back(31,37);
        }
    }
    return positions;
}

bool Player::mooveCup(int cupID, Vec2i newPosition) {

    return false;
}






