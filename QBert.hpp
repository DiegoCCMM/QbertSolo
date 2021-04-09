//
// Created by diego on 13/3/21.
//
#include "Character.hpp"
#ifndef ALLEGRO5TUTORIAL_PLAYER_HPP
#define ALLEGRO5TUTORIAL_PLAYER_HPP

class QBert : public Character{
    int sourceX = 0;
public:
    int getSourceX() const {
        return sourceX;
    }

    void setSourceX(int sourceX) {
        QBert::sourceX = sourceX;
    }

private:
    int lives = 3;
    int score = 0;
public:
    int getLives() const {
        return lives;
    }

    void setLives(int lives) {
        QBert::lives = lives;
    }

    int getScore() const {
        return score;
    }

    void setScore(int score) {
        QBert::score = score;
    }

public:
    //QBert(float x, float y, ALLEGRO_BITMAP *draw, Direction dir) : Character(x, y, draw, dir) {}
    /*
     * Cut the bitmap to get the different frames
     */
    void managementSpriteQbert() {
        std::vector<int> source;
        std::vector<int> width;

        ALLEGRO_COLOR pixel, lastPixel, separatorColor;

        source.push_back(0);

        for (int i = 0; i < al_get_bitmap_width(getDraw()); i++) {
            separatorColor = al_map_rgba(0, 0, 255, 255);
            pixel = al_get_pixel(getDraw(), i, 0);

            if (memcmp(&pixel, &lastPixel, sizeof(ALLEGRO_COLOR))) {
                if (!memcmp(&pixel, &separatorColor, sizeof(ALLEGRO_COLOR))) {
                    source.push_back(i);
                    if (source.size() == 2)
                        width.push_back(i);
                    else
                        width.push_back(i - width[width.size() - 1]);
                }
            } else if (i == al_get_bitmap_width(getDraw()) - 1) {
                width.push_back(i - width[width.size() - 1]);
                lastPixel = pixel;
            }
        }
    }


    QBert(Piramide piramide) : Character(piramide, "qbert", 0, 0, DOWNLEFT, 7, -8) {}

    /*void resize(Piramide *piramide) override {
        setX(piramide->map[getI()][getJ()].x+6);
        setY(piramide->map[getI()][getJ()].y-10);
    }*/

    /*
     * Reinicia la posicion de Q*Bert al inicio de la piramide
     * */
    void reset(Piramide *piramide){
        QBert::setFalling(false);
        QBert::setJumping(false);
        QBert::setAirTimer(0);
        QBert::setSourceX(QBert::getSourceX() - 16);
        QBert::setDir(DOWNLEFT);
        QBert::setI(0), QBert::setJ(0);
        QBert::setX(piramide->map[0][0].x+getXRespectCube()), QBert::setY(piramide->map[0][0].y+getYRespectCube());
        piramide->changeCube(0, 0);
    }
};


#endif //ALLEGRO5TUTORIAL_PLAYER_HPP
