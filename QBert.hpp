//
// Created by diego on 13/3/21.
//

#include "Character.hpp"
#include "Platillo.hpp"
#include "Enemy.hpp"


#ifndef ALLEGRO5TUTORIAL_QBERT_HPP
#define ALLEGRO5TUTORIAL_QBERT_HPP

class QBert : public Character{
    int lives = 3;
    int score = 0;
    ALLEGRO_SAMPLE *fallingSound = al_load_sample("../sounds/qbert-falling.ogg");
    ALLEGRO_SAMPLE *colisionSound = al_load_sample("../sounds/colision-qbert.ogg");
    ALLEGRO_BITMAP *bocadilloDraw = al_load_bitmap("../sprites/qbert-blasfemia.png");;   // sprite bocadillo
    bool superpower = false;
    bool enPlatillo = false;
    std::list<Platillo>::iterator platillo;

public:

    /* Constructor */
    QBert(Piramide piramide) : Character(piramide, "qbert", 0, 0, DOWNLEFT, 7, -8) {
        must_init(fallingSound, "qbert-FallingSound");
        setSourceX(0);
        //managementSpriteQbert();
    }

    /* Constructor 2 */
    QBert() : Character("qbert", DOWNRIGHT) {}

    /* Cut the bitmap to get the different frames */
    void managementSpriteQbert() {
        std::vector<int> source;
        std::vector<int> width;

        ALLEGRO_COLOR pixel, lastPixel, separatorColor;

        source.push_back(0);

        for (int i = 0; i < al_get_bitmap_width(getDraw()) - 1; i++) {
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
    bool reset(Piramide *piramide, int &puntuacion, std::list<Enemy*> &enemies, int i=0, int j=0, Direction direct = DOWNLEFT){
        QBert::setFalling(false);
        QBert::setJumping(false);
        if(QBert::getSourceX() != 0){
            QBert::setSourceX(0);
        }
        QBert::setDir(direct);
        QBert::setI(i), QBert::setJ(j);
        QBert::setX(piramide->map[i][j].x+getXRespectCube());
        QBert::setY(piramide->map[i][j].y+getYRespectCube());
        //TODO si se cae, problemas, porque puntuacion devuelve un error que esta optimized out para ahorrarnos errores
        // pero ocurre
        piramide->changeCube(i, j, puntuacion);
        setTimer(0);
        enemies.clear();
        return(false);
    }

    /* Dibuja la figura de Q*Bert */
    void drawBitmap() override {
        al_draw_bitmap_region(QBert::getDraw(), QBert::getSourceX() + (QBert::getDir() * 2 * sizePixelsX),
                              0, sizePixelsX, sizePixelsY, QBert::getX(), QBert::getY(), 0);
    }

    void setMove(Direction dir) {
        setDir(dir);
        setJumping(true);
        setSourceX(getSourceX() + 16);

        if(dir == TOPLEFT) setI(getI() - 1), setJ(getJ() - 1);
        else if(dir == DOWNRIGHT) setI(getI() + 1), setJ(getJ() + 1);
        else if(dir == TOPRIGHT) setI(getI() - 1);
        else if(dir == DOWNLEFT) setI(getI() + 1);
    }

    void movement(Piramide *piramide, int HEIGHT, std::list<Platillo> &platillos,
                  std::list<Enemy*> &enemies, bool & salida, int &puntuacion) {
        if (isJumping()) {
            timerplusplus();
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
                        if(!enPlatillo) {
                            for (std::_List_iterator<Platillo> it = platillos.begin(); it != platillos.end(); it++) {
                                if (it->getI() == getI() && it->getJ() == getJ()) {
                                    enPlatillo = true;
                                    platillo = it;
                                    platillo->setPosQBert(MONTANDO);

                                    break;
                                }
                            }

                            if (!enPlatillo) {
                                lives--;
                                setFalling(true);
                                if(getSourceX()!=0) {
                                    setSourceX(getSourceX() - 16);
                                }
                                playOnce(fallingSound);
                            }

                        } else {
                            float _x = getX(), _y = getY();
                            platillo->updateWithQBert(_x, _y, piramide);

                            if(platillo->getPosQBert() == NONE) { // Q*Bert ha salido del platillo, esta en el primer cubo
                                salida = reset(piramide, puntuacion, enemies);
                                enPlatillo = false;
                                platillos.erase(platillo);
                            } else if(platillo->getPosQBert() == BAJANDO) {
                                QBert::setY(_y);
                            } else { // Q*Bert sigue en platillo
                                QBert::setX(platillo->getX());
                                QBert::setY(platillo->getY()-6);
                            }
                        }
                    } else {
                        //WE LANDED
                        playOnce(getJumpSound());
                        piramide->changeCube(getI(), getJ(), puntuacion);
                        for (std::_List_iterator<Enemy*> it = enemies.begin(); it != enemies.end(); it++) {
                            if(it.operator*()->getI() == getI() && it.operator*()->getJ() == getJ()){
                                //estamos con un enemigo en el mismo sitio
                                if(!it.operator*()->hasChangingGroundPower() * !it.operator*()->hasHelpingPower()){
                                    salida = animacionMuerte(piramide);
                                    salida = reset(piramide, puntuacion, enemies);
                                    break;
                                }else if(it.operator*()->hasChangingGroundPower()){ // Slick y Sam
                                    enemies.erase(it, it);
                                    puntuacion += 300;
                                }else if(it.operator*()->hasHelpingPower()){ // Blob verde
                                    enemies.erase(it, it);
                                    superpower = true;
                                    puntuacion += 100;
                                }

                            }
                        }

                        setTimer(0);
                        setJumping(false);
                        if(getSourceX()!=0) {
                            setSourceX(getSourceX() - 16);
                        }
                    }
                }
            } else { // Esta cayendo
                if(getY() <= HEIGHT){
                    setY(getY() + movementY);
                } else {
                    salida = reset(piramide, puntuacion, enemies);
                }
            }
        }
    }

    bool animacionMuerte(Piramide *piramide){
        al_draw_bitmap_region(bocadilloDraw, 0,
                              0, 6*8, 4*8, QBert::getX()-6*2, QBert::getY()-4*8, 0);
        al_play_sample(colisionSound, 1.0, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);

        int none = 0;

        // Colocar a QBert en la posicion anterior a la colision
        // Si su direccion es:
        // TOPRIGHT
        /*if(getDir() == TOPRIGHT){
            reset(piramide, none, getI()+1, getJ(), getDir());
        }
            // TOPLEFT
        else if(getDir() == TOPLEFT){
            reset(piramide, none, getI()+1, getJ()+1, getDir());
        }
            // DOWNRIGHT
        else if(getDir() == DOWNRIGHT){
            reset(piramide, none, getI()-1, getJ()-1, getDir());
        }
            // DOWNLEFT
        else if(getDir() == DOWNLEFT){
            reset(piramide, none, getI()-1, getJ(), getDir());
        }*/

        //sleep(2);

        return(false);
    }

    void movementInstr() {
        float moveX = 3.0;
        float moveY[5] = {9.0, 25.0, 22.0, 20.0, 22.0};

        if (isJumping()) {
            timerplusplus();
            if (getTimer() < airTime / 2) {
                setX(moveX + getX());
                setXRespectCube(getXRespectCube()+moveX);
            } else if (getTimer() > airTime / 2 && getTimer() < airTime) {
                setX(moveX + getX());
                setXRespectCube(getXRespectCube()+moveX);
                setY(getY() + moveY[getJ()]);
                setYRespectCube(getYRespectCube()+moveY[getJ()]);
            } else if (getTimer() > airTime) {
                //WE LANDED
                playOnce(getJumpSound());

                setTimer(0);
                setJumping(false);
            }
        }
    }

    void movement_IN(Piramide *piramide) {
        if (isJumping()) {
            timerplusplus();
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
                //WE LANDED
                playOnce(getJumpSound());
                int none = 0;
                piramide->changeCube(getI(), getJ(), none);

                setTimer(0);
                setJumping(false);
                setSourceX(getSourceX() - 16);
            }
        }
    }

    void resize(Piramide *piramide) override {
        if(enPlatillo){
            QBert::setX(platillo->getX());
            QBert::setY(platillo->getY()-6);
        } else {
            QBert::setX(piramide->map[getI()][getJ()].x+this->xRespectCube);
            QBert::setY(piramide->map[getI()][getJ()].y+this->yRespectCube);
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

#endif //ALLEGRO5TUTORIAL_QBERT_HPP
