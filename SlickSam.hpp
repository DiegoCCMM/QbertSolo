//
// Created by diego on 29/4/21.
//

#include "Enemy.hpp"

#ifndef ALLEGRO5TUTORIAL_SLICKSAM_HPP
#define ALLEGRO5TUTORIAL_SLICKSAM_HPP

class SlickSam : public Enemy{

    int reachQberti = VACIO,  reachQbertj = VACIO;

public:
    SlickSam(const Piramide &piramide, const std::string &nom, int i, int j, int xRespectCube, int yRespectCube) : Enemy(
            piramide, nom, i, j, xRespectCube, yRespectCube) {
        setChangingGroundPower(true);
    }

    void randomMovement(int i, int j) override {

        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_int_distribution<int> dist4(1, 4);
        std::uniform_int_distribution<int> dist2(1, 2);
        std::uniform_int_distribution<int> dist3(1, 3);

        if(getI() == 6){//estamos abajo

            if(getJ() == 0){
                //esquina abajo a la izquierda
                setDir(TOPRIGHT);
            }else if(getJ() == getI()){
                //esquina abajo a la derecha
                setDir(TOPLEFT);
            }else{  //evitamos caer abajo
                if(dist2(mt) == 1){
                    setDir(TOPRIGHT);
                }else{
                    setDir(TOPLEFT);
                }
            }


        }else if(getI() == getJ()){//estamos a la derecha
            if(getJ() == 0) {
                //esquina arriba
                if(dist2(mt) == 1){
                    setDir(DOWNRIGHT);
                }else {
                    setDir(DOWNLEFT);
                }
            }else{//lateral derecho
                std::uniform_int_distribution<int> dist(1, 3);
                if(dist3(mt) == 1){
                    setDir(DOWNRIGHT);
                }else if(dist3(mt) == 2) {
                    setDir(DOWNLEFT);
                }else{
                    setDir(TOPLEFT);
                }
            }
        }else if(getJ() == 0){//estamos a la izquierda
            std::uniform_int_distribution<int> dist(1, 3);
            if(dist3(mt) == 1){
                setDir(TOPRIGHT);
            }if (dist3(mt) == 2) {
                setDir(DOWNLEFT);
            }else{
                setDir(DOWNRIGHT);

            }
        }else{ //sin restriccion
            if(dist4(mt) == 1){
                setDir(DOWNRIGHT);
            }else if(dist4(mt) == 2){
                setDir(DOWNLEFT);
            }else if(dist4(mt) == 3){
                setDir(TOPLEFT);
            }else{
                setDir(TOPRIGHT);
            }

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

#endif //ALLEGRO5TUTORIAL_SLICKSAM_HPP
