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
    double alpha, beta, h, v0, startX;
    alpha = 0.5;
    beta = static_cast<double>(M_PI / 2);
    h=0.0;
    v0=0;
    startX = 0;

    return throwBall(alpha, beta, h, v0, startX, 0, reinterpret_cast<int &>(startX));

}

vector<Vec3<int>> Player::throwBall(double alpha, double beta, double h, double v0, double startX, int startY,
                                    int &cupScored){
    vector<Vec3<int>> ballTrajectory;
    double g = 981; //cm.s-2
    double a = -g/(2*cos(alpha)*cos(alpha)*v0*v0);
    double b = tan(alpha);
    double c = h - cups[0].getHeight();
    double d = cos(beta)/cos(alpha);
    int x,z;

    for (int y=0; y<tableSize.y; y++){
        z = (int)round(a * y * y + b * y + h);
        x = (int)round(y * d + startX);
        ballTrajectory.emplace_back(x,y + startY,z);
        //cout << "x = " << ballTrajectory[y].getX() << " | y = " << ballTrajectory[y].getY() << " | z = " << ballTrajectory[y].getZ() << endl;
        if (z < 11 || (y+startY)>239) {
            // stop when the ball is lower than the height of a cup: we don't need more information on the trajectory
            break;
        }
    }

    cupScored = scoreCup(a, b, c, ballTrajectory);

    if (cupScored > -1) {
        removeCup(cupScored);
    }

    return ballTrajectory;
}


int Player::scoreCup(double &a, double &b, double &c, vector<Vec3<int>> &ballTrajectory){
    // only test the last element in ballTrajectory due to its construction
    int score = -1;

    int xSolution = ballTrajectory.back().getX();
    int ySolution = ballTrajectory.back().getY();
    int radius = cups[0].getRadius();
    int radius2 = radius*radius;

    for (auto &cup : cups) {
        if (cup.isOnTable()) {
            // The cup has not been scored yet
            Vec2i posCup = cup.getPosition();
            double d = (xSolution - posCup.x)*(xSolution - posCup.x)+(ySolution - posCup.y)*(ySolution - posCup.y);
            cout << "Cup id: " << cup.getID() << " d = " << d << endl;
            if (d < radius2){
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


void Player::get_z_graphics(vector<Vec3<int>> &ballTrajectory, vector<Vec3<int>> &graphicsTrajectory) {
    if (ballTrajectory.size() == graphicsTrajectory.size()) {
        int size_cm = 240; // depth in pixel of the table
        int size_pixels = 768; // height i pixel of the table
        double factor_a = 1;
        double factor_b = (size_pixels-1)/log(size_cm + 1);
        // function has the shape: f(x) = factor_a + factor_b + log(x+1)
        for (int i = 0; i<ballTrajectory.size(); i++) {
            graphicsTrajectory[i].setZ( (int)floor(factor_a + factor_b * log(1 + ballTrajectory[i].getY())) );
            //cout << "yA = " << ballTrajectory[i].getY() << " | zG = " << graphicsTrajectory[i].getZ() << endl;
        }
    } else {
        cout << "Trajectoire de la balle en cm: " << ballTrajectory.size() << "| vecteur pour la trajectoire en graphique : " << graphicsTrajectory.size() << endl;
    }
}

// supposed to be called after the cm_to_pixel_depth function
void Player::get_x_graphics(vector<Vec3<int>> &ballTrajectory, vector<Vec3<int>> &graphicsTrajectory){
    if (ballTrajectory.size() == graphicsTrajectory.size()) {
        int width_pixel_max = 1024; // width of the table max, also witdh of the window
        int witdh_pixel_middle = (int)floor(width_pixel_max/2);
        int width_pixel_min = 512;
        int heigth_pixel_table = 768;
        int width_cm = 60;


        for (int i = 0; i<ballTrajectory.size(); i++) {
            // get zG depending on yA
            int zG = graphicsTrajectory[i].getZ();
            // Get width in pixel depending on zG
            //auto pixel_width = (int)floor(width_pixel_min*(heigth_pixel_table - zG)/heigth_pixel_table);
            double alpha = (double)(heigth_pixel_table - zG)/heigth_pixel_table;
            auto pixel_width = (int)floor(alpha*(width_pixel_max-width_pixel_min) + width_pixel_min);
            // Pixel position x starting from the left of the table
            double x_shift = pixel_width*ballTrajectory[i].getX()/width_cm; // maybe <0 or bigger that the table (ball out of the table) => handle
            // We don't k ow where the left of the table start, lets start from the middle
            graphicsTrajectory[i].setX((int)floor(witdh_pixel_middle - (pixel_width/2) + x_shift));
            //cout << "x = " << ballTrajectory[i].getX() << " | zG = " << graphicsTrajectory[i].getZ() << " | alpha = " << alpha << " | largeur pixel = "<< pixel_width << " | décalage en x = " << x_shift << " | xGraphic = " << graphicsTrajectory[i].getX() << endl;
        }
        get_ball_size(ballTrajectory, graphicsTrajectory);
    } else {
        cout << "Trajectoire de la balle en cm: " << ballTrajectory.size() << "| vecteur pour la trajectoire en graphique : " << graphicsTrajectory.size() << endl;
    }
}

void Player::get_ball_size(vector<Vec3<int>> &ballTrajectory, vector<Vec3<int>> &graphicsTrajectory) {
    int ball_size_cm = 4; // diametre of the ball
    auto ball_size_end_cm = (int)floor(ball_size_cm/2);
    int width_table_cm = 60;
    int width_pixel_table_max = 1024;
    auto width_pixel_ball_max = (int)floor((double)ball_size_cm/width_table_cm*width_pixel_table_max);
    auto width_pixel_ball_min = (int)floor(width_pixel_ball_max/2); // numlérateur = ratio entre haut de la table et bas de la table
    int height_tabe_pixel = 768; // window height / 2

    for (auto &i : graphicsTrajectory) {
        // get zG depending on YA
        int zG = i.getZ();
        // Get width in pixel depending on zG
        double alpha = (double)(height_tabe_pixel - zG)/height_tabe_pixel;
        auto pixel_width = (int)floor(alpha*(width_pixel_ball_max-width_pixel_ball_min) + width_pixel_ball_min);
        i.setY(pixel_width);
        //cout << "zG = " << zG << " | alpha = " << alpha << " | ball size = " << pixel_width << " | minimum pixel size " << width_pixel_ball_max << endl;
    }
    include_zArchi_graphics(ballTrajectory, graphicsTrajectory);
}

void Player::include_zArchi_graphics(vector<Vec3<int>> &ballTrajectory, vector<Vec3<int>> &graphicsTrajectory){
    int width_table_cm = 60;
    int width_pixel_table_max = 1024;
    // at the bottom of the screen
    auto cm_to_pixel_max = (int)floor(1.0/width_table_cm*width_pixel_table_max);
    double ration_top_bottom = 1.0/2;
    // at the top of the screen
    auto cm_to_pixel_min = (int)floor(cm_to_pixel_max*ration_top_bottom); // numlérateur = ratio entre haut de la table et bas de la table
    int height_tabe_pixel = 768; // window height / 2

    for (int i=0; i<ballTrajectory.size(); i++) {
        // get zG depending on YA (to know the reduction in cm)
        int zG = graphicsTrajectory[i].getZ();
        // Get cm in pixel depending on zG
        double alpha = (double)(height_tabe_pixel - zG)/height_tabe_pixel;
        auto cm_to_pixel = (int)floor(alpha*(cm_to_pixel_max-cm_to_pixel_min) + cm_to_pixel_min);
        // reverse zG
        graphicsTrajectory[i].setZ(height_tabe_pixel - graphicsTrajectory[i].getZ());
        cout << "zG = " << graphicsTrajectory[i].getZ() << " | cm to pixel = " << cm_to_pixel;
        // include zArchitecture, "-" in the formula because the zG start from the upper left corner
        // pour le moment déconne: prendre en compte les vrai valeur pour al taille de la table, et voir comment faire pour que ça soit bien
        int new_zG = graphicsTrajectory[i].getZ() - (int)floor(ballTrajectory[i].getZ()*cm_to_pixel*0.5);
        //graphicsTrajectory[i].setZ(new_zG);

        cout << " | zA = " << ballTrajectory[i].getZ() << " | zG after = " << graphicsTrajectory[i].getZ() << endl;


    }
}
