//
// Created by diego on 13/3/21.
//

#include "Character.hpp"
#ifndef ALLEGRO5TUTORIAL_PLAYER_HPP
#define ALLEGRO5TUTORIAL_PLAYER_HPP

class QBert : public Character{
    int lives = 3;
    int score = 0;
    ALLEGRO_SAMPLE *fallingSound = al_load_sample("../sounds/qbert-falling.ogg");

public:

    /* Constructor */
    QBert(Piramide piramide) : Character(piramide, "qbert", 0, 0, DOWNLEFT, 7, -8) {
        must_init(fallingSound, "qbert-FallingSound");
        managementSpriteQbert();
    }

    /* Cut the bitmap to get the different frames */
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

    /* Reinicia la posicion de Q*Bert al inicio de la piramide */
    void reset(Piramide *piramide){
        QBert::setFalling(false);
        QBert::setJumping(false);
        QBert::setAirTimer(0);
        QBert::setSourceX(QBert::getSourceX() - 16);
        QBert::setDir(DOWNLEFT);
        QBert::setI(0), QBert::setJ(0);
        QBert::setX(piramide->map[0][0].x+getXRespectCube());
        QBert::setY(piramide->map[0][0].y+getYRespectCube());
        piramide->changeCube(0, 0);
    }

    /* Dibuja la figura de Q*Bert */
    void drawBitmap() override {
        al_draw_bitmap_region(QBert::getDraw(), QBert::getSourceX() + (QBert::getDir() * 2 * framePixels),
                              0, framePixels, framePixels, QBert::getX(), QBert::getY(), 0);
    }

    void movement(Piramide *piramide, int HEIGHT) override {
        if (isJumping()) {
            airTimerplusplus();
            if(!isFalling()) {
                if (getAirTimer() < airTime / 2) {
                    //GO UP AND DIRECTION
                    if (getDir() == TOPRIGHT || getDir() == DOWNRIGHT)
                        setX(movementX + getX());
                    else
                        setX(getX() - movementX);
                    if (getDir() != DOWNRIGHT && getDir() != DOWNLEFT)
                        setY(getY() - movementY);
                } else if (getAirTimer() > airTime / 2 && getAirTimer() < airTime) {
                    //GO DOWN AND DIRECTION
                    if (getDir() == TOPRIGHT || getDir() == DOWNRIGHT)
                        setX(movementX + getX());
                    else
                        setX(getX() - movementX);
                    if (getDir() == DOWNRIGHT || getDir() == DOWNLEFT)
                        setY(getY() + movementY);
                } else if (getAirTimer() > airTime) {
                    if(getJ()<0 || getJ() > getI() || getI()>=7){
                        // TODO: Si no hay platillo Q*Bert cae al vacio y pierde 1 vida
                        setFalling(true);
                        // Reproducir sonido caida
                        al_play_sample(fallingSound, 1.0, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
                    } else {
                        //WE LANDED
                        playOnce(getJumpSound());
                        piramide->changeCube(getI(), getJ());

                        //setX(piramide.map[getI()][getJ()].x+getXRespectCube());
                        //setY(piramide.map[getI()][getJ()].y+getYRespectCube());
                        setAirTimer(0);
                        setJumping(false);
                        setSourceX(getSourceX() - 16);
                    }
                }
            } else { // Esta cayendo
                if(getY() <= HEIGHT){
                    setY(getY() + movementY);
                } else {
                    reset(piramide);
                }
            }
        }
    }

    void destroy() override {
        al_destroy_bitmap(getDraw());
        al_destroy_sample(getJumpSound());
        al_destroy_sample(fallingSound);
    }

    /*************************
     * GETTER'S AND SETTER'S *
     *************************/

    int getLives() const { return lives; }
    void setLives(int _lives) { QBert::lives = _lives; }

    int getScore() const { return score; }
    void setScore(int _score) { QBert::score = _score; }

};

#endif //ALLEGRO5TUTORIAL_PLAYER_HPP
