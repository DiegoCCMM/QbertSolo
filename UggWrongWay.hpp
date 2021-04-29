//
// Created by diego on 29/4/21.
//

#include "Enemy.hpp"

#ifndef ALLEGRO5TUTORIAL_UGGWRONGWAY_HPP
#define ALLEGRO5TUTORIAL_UGGWRONGWAY_HPP

class UggWrongWay : public Enemy{
    bool isUgg = true;
public:
    UggWrongWay(const Piramide &piramide, const std::string &nom, int i, int j, int xRespectCube, int yRespectCube) : Enemy(
            piramide, nom, i, j, xRespectCube, yRespectCube) {
        if(nom == "Ugg"){
            isUgg = true;
        }else{
            isUgg = false;
        }
        i = 6;
        j = 6;

    }

    void randomMovement(int i, int j) override {

        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_int_distribution<int> dist(1, 2);

        if(dist(mt) == 1){
            setDir(TOPLEFT);
        }else{
            setDir(DOWNLEFT);
        }
        assignIJ();

        Enemy::setJumping(true);
        Enemy::setSourceX(getSourceX()-16);
    }

    void drawBitmap() override {
        al_draw_bitmap_region(getDraw(), getSourceX() + (getDir() * 2 * sizePixelsX),
                              getSourceY() + (sourceI * sizePixelsY), sizePixelsX, sizePixelsY*2,
                              getX(), getY(), 0);
    }
};


#endif //ALLEGRO5TUTORIAL_UGGWRONGWAY_HPP
