//
// Created by diego on 13/3/21.
//

#include "Character.hpp"
#include "Platillo.hpp"

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
        setSourceX(0);
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
        QBert::setSourceX(QBert::getSourceX() - 16);
        QBert::setDir(DOWNLEFT);
        QBert::setI(0), QBert::setJ(0);
        QBert::setX(piramide->map[0][0].x+getXRespectCube());
        QBert::setY(piramide->map[0][0].y+getYRespectCube());
        piramide->changeCube(0, 0);
        setTimer(0);
    }

    /* Dibuja la figura de Q*Bert */
    void drawBitmap() override {
        al_draw_bitmap_region(QBert::getDraw(), QBert::getSourceX() + (QBert::getDir() * 2 * sizePixels),
                              0, sizePixels, sizePixels, QBert::getX(), QBert::getY(), 0);
    }

    void movement(Piramide *piramide, int HEIGHT, std::list<Platillo> &platillos) {
        if (isJumping()) {
            airTimerplusplus();
            if(!isFalling()) {
                if (getTimer() < airTime / 2) {
                    //GO UP AND DIRECTION
                    if (getDir() == TOPRIGHT || getDir() == DOWNRIGHT)
                        setX(movementX + getX());
                    else
                        setX(getX() - movementX);
                    if (getDir() != DOWNRIGHT && getDir() != DOWNLEFT)
                        setY(getY() - movementY);
                } else if (getTimer() > airTime / 2 && getTimer() < airTime) {
                    //GO DOWN AND DIRECTION
                    if (getDir() == TOPRIGHT || getDir() == DOWNRIGHT)
                        setX(movementX + getX());
                    else
                        setX(getX() - movementX);
                    if (getDir() == DOWNRIGHT || getDir() == DOWNLEFT)
                        setY(getY() + movementY);
                } else if (getTimer() > airTime) {
                    if(getJ()<0 || getJ() > getI() || getI()>6){
                        //if(!enPlatillo) {
                            bool hayPlatillo = false;
                            for (std::_List_iterator<Platillo> it = platillos.begin(); it != platillos.end(); it++) {
                                if (it->getI() == getI() && it->getJ() == getJ()) {
                                    hayPlatillo = true;
                                    // TODO: subir el platillo con Q*Bert


                                    break;
                                }
                            }

                            if (!hayPlatillo) {
                                // TODO: perder vida
                                setFalling(true);
                                playOnce(fallingSound);
                            } else {
                                if(getY() <= HEIGHT){
                                    setY(getY() + movementY);
                                }
                            }
                        /*} else {
                            if(getY() <= HEIGHT){
                                setY(getY() + movementY);
                            }
                        }*/
                    } else {
                        //WE LANDED
                        playOnce(getJumpSound());
                        piramide->changeCube(getI(), getJ());

                        setTimer(0);
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
