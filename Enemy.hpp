//
// Created by diego on 13/3/21.
//
#include "Character.hpp"
#include <random>
#include <chrono>
#include <list>

#ifndef ALLEGRO5TUTORIAL_ENEMY_HPP
#define ALLEGRO5TUTORIAL_ENEMY_HPP

#define MIN 60
#define MAX 120

enum Estado {
    CIELO, INGAME
};

class Enemy : public Character {


    bool changingGroundPower = false;
    bool helpingPower = false;
    int randMoveTimer = 0;
    int randMovePeriod;
    Estado estado = CIELO;
    bool colisionado = false;

protected:
    int IA; //0 CLASICO, 1 MEDIO, 2 DIFICIL

public:
    bool isCoily = false;
    //de todos los sprites, el primer sprite, o el sprite a la izquierda de la pareja, tiene ancho leftSprite
    //usado porque Coily tiene su primera pareja de tamaño 14, el resto es de tamaño 16
    int leftSprite = 16;

    /* Constructor */
    Enemy(Piramide piramide, std::string nom, int i, int j, int xRespectCube, int yRespectCube, int IA=0) :
            Character(piramide, nom, i, j, DOWNRIGHT, xRespectCube, yRespectCube) {

        Enemy::setSourceX(16);
        this->IA = IA;
        std::mt19937 mt(std::chrono::system_clock::now().time_since_epoch().count());
        //
        std::uniform_int_distribution<int> dist(MIN, MAX);
        Enemy::randMovePeriod;
        switch (piramide.getLevel()) {
            case 6:
                randMovePeriod = 23;
                break;
            case 7:
                randMovePeriod = 20;
                break;
            case 8:
                randMovePeriod = 17;
                break;
            case 9:
                randMovePeriod = 15;
                break;
            default:
                randMovePeriod = 25;
                break;
        }

        if(nom != "Ugg" && nom != "WrongWay"){
            Character::y = piramide.map[i][j].y -32-10-32;
        }

        if (nom == "GreenBlob") {
            setHelpingPower(true);
        }

        if(nom == "Slick" || nom == "Sam"){
            ALLEGRO_SAMPLE *musica1 = al_load_sample(("./sounds/" + nom + "1.ogg").c_str());
            ALLEGRO_SAMPLE *musica2 = al_load_sample(("./sounds/" + nom + "2.ogg").c_str());

            playOnce(musica1);
            playOnce(musica2);
        }
    }


    virtual void randomMovement(int iQBert, int jQBert) {
        bool aleatorio;
        std::random_device rd;
        std::mt19937 mt(std::chrono::system_clock::now().time_since_epoch().count());
        std::uniform_int_distribution<int> dist(1, 2);

        if(IA == 0) aleatorio = true;
        else if(IA == 1){ // 50% aleatorio 50% IA --> MEDIO
            if(dist(mt) == 1) aleatorio = true;
            else aleatorio = false;
        }
        else if(IA == 2) {
            aleatorio = false;
        }

        Direction d;
        if(aleatorio){
            if (dist(mt) == 1) d = DOWNRIGHT;
            else d = DOWNLEFT;
        }
        else {
            if(getJ()>=jQBert){
                d = DOWNLEFT;
            }
            else {
                d = DOWNRIGHT;
            }
        }

        setDir(d);
        assignIJ();

        Enemy::setJumping(true);
        Enemy::setSourceX(getSourceX() - 16);
    }

    int distanciaQbert(int qberti, int qbertj){

        int distanciaQbert =  abs(getJ() - qbertj);
        if(getI() == qberti){
            //está en la misma fila, siempre habrá el mismo movimiento hacia vertical que lateral
            distanciaQbert *=  2;
        }else if(distanciaQbert == 0){
            //solo hay diferencia vertical
            distanciaQbert =  abs(getI() - qberti);
        }else{//estamos difente tanto en fila como en columna
            if((getJ() > qbertj && getI() < qberti) || (getJ() < qbertj && getI() > qberti)) {
                //si vamos hacia abajo a la izquierda o arriba a la derecha, necesitamos un movimiento más hacia abajo o arriba
                distanciaQbert += distanciaQbert + 1;
            }else if((getJ() < qbertj && getI() < qberti) || (getJ() > qbertj && getI() > qberti)){
                //si vamos hacia arriba a la izquierda o abajo a la derecha, necesitamos un movimiento menos hacia abajo o arriba
                distanciaQbert += distanciaQbert - 1;
            }
        }
        return distanciaQbert;
    }

    int distanciaQbert(int qberti, int qbertj, int i, int j){

        int distanciaAQbert =  abs(j - qbertj);
        if(i == qberti){
            //está en la misma fila, siempre habrá el mismo movimiento hacia vertical que lateral
            distanciaAQbert *=  2;
        }else if(distanciaAQbert == 0){
            //solo hay diferencia vertical
            distanciaAQbert =  abs(i - qberti);
        }else{//estamos difente tanto en fila como en columna
            if((j > qbertj && i < qberti) || (j < qbertj && i > qberti)) {
                //si vamos hacia abajo a la izquierda o arriba a la derecha, necesitamos un movimiento más hacia abajo o arriba
                distanciaAQbert += distanciaAQbert + 1;
            }else if((j < qbertj && i < qberti) || (j > qbertj && i > qberti)){
                //si vamos hacia arriba a la izquierda o abajo a la derecha, necesitamos un movimiento menos hacia abajo o arriba
                distanciaAQbert += distanciaAQbert - 1;
            }
        }
        return distanciaAQbert;
    }

    virtual void assignIJ() {
        if (getDir() == TOPLEFT) setI(getI() - 1), setJ(getJ() - 1);
        else if (getDir() == DOWNRIGHT) setI(getI() + 1), setJ(getJ() + 1);
        else if (getDir() == TOPRIGHT) setI(getI() - 1);
        else if (getDir() == DOWNLEFT) setI(getI() + 1);
    }

    virtual void movement(Piramide *piramide, int WIDTH, int HEIGHT, int &puntuacion, Character *qbert, std::_List_iterator<Enemy *> it,
                  std::list<Enemy*> & borrarEnemies) {
        if (estado == INGAME) { // Esta en el campo de juego
            if (isJumping()) {
                timerplusplus();
                if (!isFalling()) {
                    if (getTimer() < airTime / 2) {
                        //GO UP AND DIRECTION
                        if (getDir() == TOPRIGHT || getDir() == DOWNRIGHT)
                            setX(movementX + getX());
                        else if(getDir() == TOPLEFT || getDir() == DOWNLEFT)
                            setX(getX() - movementX);
                        if (getDir() == TOPRIGHT || getDir() == TOPLEFT)
                            setY(getY() - movementY);
                    } else if (getTimer() > airTime / 2 && getTimer() < airTime) {
                        //GO DOWN AND DIRECTION
                        if (getDir() == TOPRIGHT || getDir() == DOWNRIGHT)
                            setX(movementX + getX());
                        else if(getDir() == TOPLEFT || getDir() == DOWNLEFT)
                            setX(getX() - movementX);
                        if (getDir() == DOWNRIGHT || getDir() == DOWNLEFT)
                            setY(getY() + movementY);
                    } else if (getTimer() > airTime) {
                        if ((getJ() < 0 || getJ() > getI() || getI() > 6) ||
                                (nom == "Ugg" && (getJ() <= 0 || getJ() > getI() || getI() > 6)) ||
                                (nom == "WrongWay" && (getJ() < 0 || getJ() >= getI() || getI() > 6)) ){
                            setFalling(true);
                            if(nom == "coilyBola") {
                                puntuacion+=500;
                                ALLEGRO_SAMPLE *coilyCae = al_load_sample("./sounds/snake-fall.ogg");
                                playOnce(coilyCae);
                            }
                            if (getSourceX() != 0) {
                                setSourceX(getSourceX() + 16);
                            }
                        } else {
                            //WE LANDED
                            playOnce(getJumpSound());
                            if (hasChangingGroundPower()) {
                                int none = 0;
                                piramide->changeCubeInverse(getI(), getJ());
                            }
                            setX(piramide->map[getI()][getJ()].x + getXRespectCube());
                            setY(piramide->map[getI()][getJ()].y + getYRespectCube());
                            setTimer(0);
                            setJumping(false);
                            setSourceX(getSourceX() + leftSprite);

                            if (getI() == qbert->getI() && getJ() == qbert->getJ()) {
                                colisionado = true;
                            }
                        }
                    }
                } else {    //se cae de la piramide

                    if(nom == "Ugg" && getX() >= WIDTH/2-200){
                        setX(getX() - movementX);
                    }
                    else if(nom == "WrongWay" && getX() <= WIDTH/2+200){
                        setX(getX() + movementX);
                    }
                    else if(nom != "WrongWay" && nom != "Ugg" && getY() <= HEIGHT){
                        setY(getY() + movementY);
                    }
                    else{//si hemos llegado al suelo
                        borrarEnemies.push_back(it.operator*());//lo borramos
                    }
                }
            }
        } else { // Esta bajando del cielo, CIELO
            timerplusplus();
            setX(piramide->map[getI()][getJ()].x + getXRespectCube());
            if (getY() < piramide->map[getI()][getJ()].y + getYRespectCube()) {
                setY(getY() + movementY);
            } else {
                estado = INGAME;
                if(nom == "Slick" || nom == "Sam"){
                    piramide->changeCubeInverse(getI(),getJ());
                }
                if (getI() == qbert->getI() && getJ() == qbert->getJ()) {
                    colisionado = true;
                }
                playOnce(getJumpSound());
                setX(piramide->map[getI()][getJ()].x + getXRespectCube());
                setY(piramide->map[getI()][getJ()].y + getYRespectCube());
                setTimer(0);
            }
        }
    }


    /*************************
     * GETTER'S AND SETTER'S *
     *************************/

    bool haColisionado() {
        return colisionado;
    }

    void setHaColisionado(bool col) {
        colisionado = col;
    }

    void setChangingGroundPower(bool changingGroundPower) {
        Enemy::changingGroundPower = changingGroundPower;
    }

    bool hasHelpingPower() const {
        return helpingPower;
    }

    void setHelpingPower(bool helpingPower) {
        Enemy::helpingPower = helpingPower;
    }

    int getRandMoveTimer() const { return randMoveTimer; }

    void randomMoveTimerplusplus() { Enemy::randMoveTimer++; }

    void resetRandomMoveTimer() { Enemy::randMoveTimer = 0; }

    int getRandMovePeriod() const { return randMovePeriod; }

    bool hasChangingGroundPower() const { return changingGroundPower; }

    Estado getEstado() const {
        return estado;
    }

    void setEstado(Estado estado) {
        Enemy::estado = estado;
    }

    bool estaCielo() {
        return estado == CIELO;
    }

};

#endif //ALLEGRO5TUTORIAL_ENEMY_HPP
