//
// Created by diego on 13/3/21.
//

#include "Character.hpp"
#include "Platillo.hpp"
#include "Enemy.hpp"

#define SUPERPOWER_PERIOD 10

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
    bool colision = false;
    int timerColision = 0;
    int timerSuperPower = 0;
    std::list<Platillo>::iterator platillo;

public:

    /* Constructor */
    QBert(Piramide piramide) : Character(piramide, "qbert", 0, 0, DOWNLEFT, 8, -8) {
        must_init(fallingSound, "qbert-FallingSound");
        setSourceX(0);
    }

    /* Constructor 2 */
    QBert() : Character("qbert", DOWNRIGHT) {}

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

    void drawBocadillo(){
        al_draw_bitmap_region(bocadilloDraw, 0,
                              0, 6 * 8, 4 * 8, getX() - 6 * 2, getY() - 4 * 8, 0);
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
                  std::list<Enemy*> &enemies, bool & hasCoily, bool & hasSlickSam, int &puntuacion) {
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

                                    for (std::_List_iterator<Enemy *> it = enemies.begin(); it != enemies.end(); it++) {
                                        if(it.operator*()->isCoily){

                                            Coily* col = dynamic_cast<Coily*>(it.operator*());

                                            if(col->coilyCouldFall()) {
                                                col->setState(REACHING_LEDGE);
                                                col->setReachQberti(getI());
                                                col->setReachQbertj(getJ());
                                                /*if(getJ()<0) {
                                                    col->setReachQberti(getI());
                                                    col->setReachQbertj(getJ() -1);
                                                }else if(getJ() > getI()){
                                                    col->setReachQberti(getI());
                                                    col->setReachQbertj(getJ()-1);
                                                }else if( getI()>6){
                                                    col->setReachQberti(getI()-1);
                                                    col->setReachQbertj(getJ());
                                                }*/
                                            }
                                        }
                                    }
                                    hasCoily = false;
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
                                hasCoily = reset(piramide, puntuacion, enemies);
                                hasSlickSam = false;
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
                        if(superpower){
                            if(timerSuperPower == SUPERPOWER_PERIOD){
                                superpower = false;
                                timerSuperPower = 0;
                            }else{
                                timerSuperPower++;
                            }
                        }
                        playOnce(getJumpSound());
                        piramide->changeCube(getI(), getJ(), puntuacion);
                        for (auto it = enemies.begin(); it != enemies.end(); it++) {
                            //DID WE LANDED ON AN ENEMY?
                            if(it.operator*()->getI() == QBert::getI() && it.operator*()->getJ() == QBert::getJ()){
                                //estamos con un enemigo en el mismo sitio
                                if(it.operator*()->hasHelpingPower()){ // Blob verde
                                    superpower = true;
                                    puntuacion += 100;
                                    enemies.erase(it);
                                    break;
                                }else if(it.operator*()->hasChangingGroundPower()){ // Slick y Sam
                                    hasSlickSam = false;
                                    puntuacion += 300;
                                    enemies.erase(it);
                                    break;
                                }else{
                                colision = true;
                                lives--;
                                hasCoily = animacionMuerte(piramide);
                                hasCoily = reset(piramide, puntuacion, enemies, getI(), getJ(), getDir());
                                hasSlickSam = false;
                                break;
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
                    hasCoily = reset(piramide, puntuacion, enemies);
                    hasSlickSam = false;
                }
            }
        }
    }

    void setSuperpower(bool superpower) {
        QBert::superpower = superpower;
    }

    bool animacionMuerte(Piramide *piramide){

        al_draw_bitmap_region(bocadilloDraw, 0,
                              0, 6*8, 4*8, QBert::getX()-6*2, QBert::getY()-4*8, 0);
        al_play_sample(colisionSound, 1.0, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);

        // Colocar a QBert en la posicion anterior a la colision

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

    int getTimerColision() const { return timerColision; }
    void setTimerColision(int timerColision) { QBert::timerColision = timerColision; }

    bool isColision() const { return colision; }
    void setColision(bool colision) { QBert::colision = colision; }

    bool hasSuperpower() const { return superpower; }

    bool isEnPlatillo() const { return enPlatillo; }
    void setEnPlatillo(bool enPlatillo) { QBert::enPlatillo = enPlatillo; }

};

#endif //ALLEGRO5TUTORIAL_QBERT_HPP
