//
// Created by lucas on 4/3/18.
//

#include "BoardGame.h"

using namespace std;


void BoardGame::startGame() {
    cout << "\nDébut de la partie !" << endl;
    cout << "Victoire écrasante de Lucas, Najwa est en PLS dans les toilettes !!" << endl;
}

vector<Vec3<int>> BoardGame::throwBall(Player joueur, float power, float angle, float lateralOffset){
    double Vx, Vy, Vz; // les vitesses d'origine selon x y et z
    double convertAngle; // l'angle de lance converti entre -Pi/4 et Pi/4
    int convertOffset;  // le décalage de lance convertie dans la table (entre 0 et sizeX)
    int x0, y0, z0; // les coordonnées du lancé apres decalage

    /**power et angle entre 0 et 1 => les convertir
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
    cout << "angle = " << convertAngle << endl;
    cout << "Vx = " << Vx << endl;
    cout << "Vy = " << Vy << endl;


    y0 = 30; //le bras du joueur 30 cm
    x0 = (int)floor(lateralOffset * sizeX);
    z0 = 90; // le joueur lance la balle a 90cm de la table (grand)

    /** calcul des positions succesives de la balle
     */

    int x, y, z;
    vector<Vec3<int>> throwingPositions;
    for (int i = 0; i<sizeY; i++){
        y = i;
        x = (int)floor(Vx * (y - y0) / Vy) + x0;
        z = (int)floor(-9.81*y*y/(Vy*Vy)) + y + z0;
        //cout << "(x,y,z) = (" << x << ", " << y << ", " << z << ")" << endl;
        throwingPositions.push_back(Vec3<int>(x,y,z));
    }
    return throwingPositions;
}










