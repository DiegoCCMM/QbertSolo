//
// Created by diego on 13/3/21.
//
#include "Character.hpp"
#include <random>

#ifndef ALLEGRO5TUTORIAL_ENEMY_HPP
#define ALLEGRO5TUTORIAL_ENEMY_HPP

#define MIN 60
#define MAX 120

class Enemy : public Character{
    bool changingGroundPower = false;
    bool helpingPower = false;
public:
    bool isCoily = false;

    void setChangingGroundPower(bool changingGroundPower) {
        Enemy::changingGroundPower = changingGroundPower;
    }

    bool hasHelpingPower() const {
        return helpingPower;
    }

    void setHelpingPower(bool helpingPower) {
        Enemy::helpingPower = helpingPower;
    }

private:
    int randMoveTimer = 0;
    int randMovePeriod;

public:
    //de todos los sprites, el primer sprite, o el sprite a la izquierda de la pareja, tiene ancho leftSprite
    //usado porque Coily tiene su primera pareja de tamaño 14, el resto es de tamaño 16
    int leftSprite = 16;

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

    virtual void randomMovement(int i, int j)  {
        // TODO: revisar, no parece haber aleatoriedad
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_int_distribution<int> dist(1, 2);
        std::cout << "valor qbert i" << i << std::endl;
        std::cout << "valor qbert j" << j << std::endl;

        std::cout << "valor qbert i" << getI() << std::endl;
        std::cout << "valor qbert j" << getJ() << std::endl;

        if(dist(mt) == 1){
            setDir(DOWNRIGHT);
        }else{
            setDir(DOWNLEFT);
        }
        assignIJ();

        Enemy::setJumping(true);
        Enemy::setSourceX(getSourceX()-16);
    }

    void assignIJ(){
        if(getDir() == TOPLEFT) setI(getI() - 1), setJ(getJ() - 1);
        else if(getDir() == DOWNRIGHT) setI(getI() + 1), setJ(getJ() + 1);
        else if(getDir() == TOPRIGHT) setI(getI() - 1);
        else if(getDir() == DOWNLEFT) setI(getI() + 1);
    }

    void movement(Piramide *piramide, int HEIGHT, int &i, int &j){
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
                    int none = 0;
                    piramide->changeCubeInverse(getI(), getJ());
                }
                setX(piramide->map[getI()][getJ()].x + getXRespectCube());
                setY(piramide->map[getI()][getJ()].y + getYRespectCube());
                setTimer(0);
                setJumping(false);
                setSourceX(getSourceX() + leftSprite);
                i = getI();
                j = getJ();
            }
        }
        //randomMoveTimerplusplus();
    }

};

#endif //ALLEGRO5TUTORIAL_ENEMY_HPP
