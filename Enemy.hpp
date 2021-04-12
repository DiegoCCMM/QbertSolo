//
// Created by diego on 13/3/21.
//
#include "Character.hpp"
#include "QBert.hpp"
#include <random>

#ifndef ALLEGRO5TUTORIAL_ENEMY_HPP
#define ALLEGRO5TUTORIAL_ENEMY_HPP

#define MIN 60
#define MAX 120

class Enemy : public Character{
    bool changingGroundPower = false;
    int randMoveTimer = 0;
    int randMovePeriod;

public:

    /* Constructor */
    Enemy(Piramide piramide, std::string nom, int i, int j, int xRespectCube, int yRespectCube) :
            Character(piramide, nom, i, j, DOWNRIGHT, xRespectCube, yRespectCube) {

        Enemy::setSourceX(16);
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_int_distribution<int> dist(MIN, MAX);
        Enemy::randMovePeriod = dist(mt);
    }

    int getRandMoveTimer() const { return randMoveTimer; }
    void randomMoveTimerplusplus() { Enemy::randMoveTimer++; }
    void resetRandomMoveTimer(){ Enemy::randMoveTimer = 0; }

    int getRandMovePeriod() const { return randMovePeriod; }

    bool hasChangingGroundPower() const { return changingGroundPower; }

    void randomMovement(){
        // TODO: revisar, no parece haber aleatoriedad
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_int_distribution<int> dist(1, 2);
        if(dist(mt) == 1){
            setDir(DOWNRIGHT);
        }else{
            setDir(DOWNLEFT);
        }

        Enemy::setJumping(true);
        Enemy::setSourceX(getSourceX()-16);
    }

    void assignIJ(){
        // TODO: crear resto de direcciones para un futuro los siguientes enemigos (ej: Ugg y Wrong-way, o Coily)
        if(getDir() == DOWNRIGHT){
            setI(getI()+1);
            setJ(getJ()+1);

        }else{//dir == DOWNLEFT
            setI(getI()+1);
        }
    }

    void movement(Piramide *piramide, int HEIGHT) override {
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
                if (hasChangingGroundPower()) {
                    piramide->changeCube(getI(), getJ());
                }
                setX(piramide->map[getI()][getJ()].x + getXRespectCube());
                setY(piramide->map[getI()][getJ()].y + getYRespectCube());
                setTimer(0);
                setJumping(false);
                assignIJ();
                setSourceX(getSourceX() + 16);
            }
        }
        //randomMoveTimerplusplus();
    }

};

#endif //ALLEGRO5TUTORIAL_ENEMY_HPP
