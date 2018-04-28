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

    for (int y=0; y<tableSize.y+60; y++){
        z = (int)round(a * y * y + b * y + h);
        x = (int)round(y * d + startX);
        ballTrajectory.emplace_back(x,y + startY,z);
        //cout << "x = " << ballTrajectory[y].getX() << " | y = " << ballTrajectory[y].getY() << " | z = " << ballTrajectory[y].getZ() << endl;
        if ((z < 1 && (y+startY)<tableSize.y && x>0 && x<tableSize.x+1) || (z < -50 && (y+startY)>tableSize.y)) {
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
    float radius = cups[0].getRadius();
    float radius2 = radius*radius;
    //cout << "radius square = " << radius2 << "  radius = " << radius << "  x = " << xSolution << "  y = " << ySolution << endl;
    for (auto &cup : cups) {
        if (cup.isOnTable()) {
            // The cup has not been scored yet
            Vec2i posCup = cup.getPosition();
            double d = (xSolution - posCup.x)*(xSolution - posCup.x)+(ySolution - posCup.y)*(ySolution - posCup.y);
            //cout << "Cup id: " << cup.getID() << " d = " << sqrt(d) << " d square= " << d << " Cups x: " << cup.getPosition().x << " Cups y: " << cup.getPosition().y << endl;
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
            positions.emplace_back(30,tableSize.y-28);
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

void Player::get_xzSize_graphics(vector<Vec3<int>> &ballTrajectory, vector<Vec3<int>> &graphicsTrajectory) {

    if (ballTrajectory.size() == graphicsTrajectory.size()) {
        window_width_pixel = 1024;
        auto window_witdh_pixel_middle = (int)floor(window_width_pixel/2);
        window_height_pixel = 768;
        ratio_min_max = 1.0/2;
        table_width_max_cm = 60;
        table_width_min_cm = (int)floor((double)table_width_max_cm*ratio_min_max);
        table_width_max_pixel = 889;
        table_width_min_pixel = (int)floor((double)table_width_max_pixel*ratio_min_max);
        table_length_cm = 240;
        table_length_pixel_zAxis = 461;
        ball_size_max_cm = 8;
        ball_size_min_cm = (int)floor(ball_size_max_cm*ratio_min_max);
        ball_size_max_pixel = (int)floor((double)ball_size_max_cm/table_width_max_cm*table_width_max_pixel);;
        ball_size_min_pixel = (int)floor(ball_size_max_pixel/4.0);//(int)floor(ball_size_max_pixel*ratio_min_max);
        auto cm_to_pixel_max = (int)floor(1.0/table_width_max_cm*table_width_max_pixel);
        auto cm_to_pixel_min = (int)floor(cm_to_pixel_max*ratio_min_max);


        double factor_a = 1;
        double factor_b = (table_length_pixel_zAxis-1)/log(table_length_cm + 1);
        double factor_c = table_length_pixel_zAxis/table_length_cm;
        // function has the shape: f(x) = factor_a + factor_b + log(x+1)
        for (int i = 0; i<ballTrajectory.size(); i++) {
            // Calculate zG depending on yA
            auto zG = (int)floor(factor_a + factor_b * log(1 + ballTrajectory[i].getY()));
            //auto zGBall = (int)floor(factor_a + factor_c * ballTrajectory[i].getY()); // for ball zisize variation
            //zGBall = zG;


            // Calculate the xG-Position of the ball depending on the its zG-Position
            // Get width in pixel depending on zG
            double alpha = (double)(table_length_pixel_zAxis - zG)/table_length_pixel_zAxis;
            double alphaBall = (double)(table_length_pixel_zAxis - zG)/table_length_pixel_zAxis;
            auto pixel_width = (int)floor(alpha*(table_width_max_pixel-table_width_min_pixel) + table_width_min_pixel);
            // Pixel position x starting from the left of the table
            double x_shift = pixel_width*ballTrajectory[i].getX()/table_width_max_cm; // maybe <0 or bigger that the table (ball out of the table) => handle
            // We don't k ow where the left of the table start, lets start from the middle
            auto xG = (int)floor(window_witdh_pixel_middle - (pixel_width/2) + x_shift);
            graphicsTrajectory[i].setX(xG);

            // Calculate the ball size
            pixel_width = (int)floor(alphaBall*(ball_size_max_pixel-ball_size_min_pixel) + ball_size_min_pixel);
            graphicsTrajectory[i].setY(pixel_width);


            // récuperer les tailles de cups
            /*int zCup1 = (int)floor(factor_a + factor_b * log(1 + 230));
            int zCup2 = (int)floor(factor_a + factor_b * log(1 + 221));
            int zCup3 = (int)floor(factor_a + factor_b * log(1 + 212));
            cout << "Z cup d fond " << zCup1 << " | Z cup du milieu " << zCup2 << " | Z cup de devant " << zCup3 << endl;
            double alphaCup1 = (double)(table_length_pixel_zAxis - zCup1)/table_length_pixel_zAxis;
            double alphaCup2 = (double)(table_length_pixel_zAxis - zCup2)/table_length_pixel_zAxis;
            double alphaCup3 = (double)(table_length_pixel_zAxis - zCup3)/table_length_pixel_zAxis;
            //cout << "alpha cup1 " << alphaCup1 << " | alpha cup2 " << alphaCup2 << " | alpha cup3 " << alphaCup3 << endl;

            auto cm_to_pixelCup1 = (int)floor(alphaCup1*(cm_to_pixel_max-cm_to_pixel_min) + cm_to_pixel_min);
            auto cm_to_pixelCup2 = (int)floor(alphaCup2*(cm_to_pixel_max-cm_to_pixel_min) + cm_to_pixel_min);
            auto cm_to_pixelCup3 = (int)floor(alphaCup3*(cm_to_pixel_max-cm_to_pixel_min) + cm_to_pixel_min);
            cout << "hauteur: Cup du fond : " << 12*cm_to_pixelCup1  << " | Cup du milieu : " << 12*cm_to_pixelCup2 << " | Cup de devant : " << 12*cm_to_pixelCup3 << endl;
            cout << "largeur: Cup du fond : " << 9*cm_to_pixelCup1  << " | Cup du milieu : " << 9*cm_to_pixelCup2 << " | Cup de devant : " << 9*cm_to_pixelCup3 << endl;
*/
            // choper x pixel des cups
            /*cout << " Position x de la cup:";
            for (auto &cup : cups) {
                auto zCup = (int)floor(factor_a + factor_b * log(1 + cup.getPosition().y));
                double alphaCup = (double)(table_length_pixel_zAxis - zCup)/table_length_pixel_zAxis;
                auto cm_to_pixelCup = (int)floor(alphaCup*(cm_to_pixel_max-cm_to_pixel_min) + cm_to_pixel_min);
                int height_pixel = cup.getHeight()*cm_to_pixelCup;
                int width_pixel = cup.getRadius()*2*cm_to_pixelCup;

                auto pixel_width_cup = (int)floor(alphaCup*(table_width_max_pixel-table_width_min_pixel) + table_width_min_pixel);


                double x_shift_cup = pixel_width_cup*cup.getPosition().x/table_width_max_cm;
                auto xG_Cup = (int)floor(window_witdh_pixel_middle - (pixel_width_cup/2) + x_shift_cup + width_pixel/2);
                cout << " | " << cup.getID() << ": (" << xG_Cup << "," << zCup << ")";
            }
            cout << endl;*/





            // include zA
            // Get cm in pixel depending on zG
            auto cm_to_pixel = (int)floor(alpha*(cm_to_pixel_max-cm_to_pixel_min) + cm_to_pixel_min);
            ////cout << "zG = " << graphicsTrajectory[i].getZ() << " | cm to pixel = " << cm_to_pixel;

            // reverse zG
            // include zArchitecture, "-" in the formula because the zG start from the upper left corner
            // pour le moment déconne: prendre en compte les vrai valeur pour al taille de la table, et voir comment faire pour que ça soit bien
            int new_zG = window_height_pixel -(zG + (int)floor((ballTrajectory[i].getZ()*cm_to_pixel)*0.3));
            //int new_zG = window_height_pixel -(zG);
            ////cout << " | zA = " << ballTrajectory[i].getZ() << " | zG after = " << graphicsTrajectory[i].getZ() << endl;
            graphicsTrajectory[i].setZ(new_zG);
            //cout << "xA : " << ballTrajectory[i].getX() << " | xG : " << graphicsTrajectory[i].getX() << " | yA : " << ballTrajectory[i].getY() << " | alpha : " << alpha << ballTrajectory[i].getY() << " | yG : " << graphicsTrajectory[i].getY()<< " | zA : " << ballTrajectory[i].getZ() << " | Pixel to cm : " << cm_to_pixel << " | zG before : " << zG << " | zG : " << graphicsTrajectory[i].getZ() << endl << endl;

            // cout << " | length table pixel = " << table_length_pixel_zAxis << " | zG = " << zG << " | alpha = " << alpha << " | ball size = " << pixel_width << " | minimum ball pixel size = " << ball_size_min_pixel << endl;

            if (i == ballTrajectory.size()-1){
                cout << "ball arrival : " << "x= " << ballTrajectory[i].getX() << "   y= " << ballTrajectory[i].getY() <<"   z= " << ballTrajectory[i].getZ() << endl;
            }
        }
    } else {
        cout << "Trajectoire de la balle en cm: " << ballTrajectory.size() << "| vecteur pour la trajectoire en graphique : " << graphicsTrajectory.size() << endl;
    }
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
        graphicsTrajectory[i].setZ(new_zG);

        cout << " | zA = " << ballTrajectory[i].getZ() << " | zG after = " << graphicsTrajectory[i].getZ() << endl;


    }
}
