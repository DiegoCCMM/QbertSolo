//
// Created by diego on 29/4/21.
//

#include "Enemy.hpp"

#ifndef ALLEGRO5TUTORIAL_UGGWRONGWAY_HPP
#define ALLEGRO5TUTORIAL_UGGWRONGWAY_HPP

class UggWrongWay : public Enemy{
    bool isUgg = true;  //ugg goes from right to left, wrong way, on the other way
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

        ALLEGRO_SAMPLE *musica1 = al_load_sample(("../sounds/" + nom + "-1.ogg").c_str());
        ALLEGRO_SAMPLE *musica2 = al_load_sample(("../sounds/" + nom + "-2.ogg").c_str());

        playOnce(musica1);
        playOnce(musica2);
    }

    void randomMovement(int i, int j) override {

        std::random_device rd;
        std::mt19937 mt(std::chrono::system_clock::now().time_since_epoch().count());
        std::uniform_int_distribution<int> dist(1, 2);
        if(isUgg) {
            if (dist(mt) == 1) {
                setDir(TOPLEFT);
            } else {
                if(getI() != 6){//si no estamos en la ultima fila
                    setDir(DOWNLEFT);
                }else{
                    setDir(TOPLEFT);
                }

            }
        }else{//somos wrong way, vamos de izq a derecha
            if (dist(mt) == 1) {
                setDir(TOPRIGHT);
            } else {
                if(getI() != 6){//si no estamos en la ultima fila
                    setDir(DOWNRIGHT);
                }else{
                    setDir(TOPRIGHT);
                }

            }
        }
        assignIJ();

        setJumping(true);
        setSourceX(getSourceX()-16);
    }

    void drawBitmap() override {
        al_draw_bitmap_region(getDraw(), getSourceX() + (getDir() * 2 * sizePixelsX),
                              getSourceY() + (sourceI * sizePixelsY), sizePixelsX, sizePixelsY*2,
                              getX(), getY(), 0);
    }
};

#endif //ALLEGRO5TUTORIAL_UGGWRONGWAY_HPP
