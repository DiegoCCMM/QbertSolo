//
// Created by diego on 13/3/21.
//
#include "Character.hpp"
#include "QBert.hpp"
#include <random>

#ifndef ALLEGRO5TUTORIAL_ENEMY_HPP
#define ALLEGRO5TUTORIAL_ENEMY_HPP
class Enemy : public Character{
    Direction dir;
    bool changingGroundPower = false;
    int sourceX = 16;
public:
    int getSourceX() const {
        return sourceX;
    }

    void setSourceX(int sourceX) {
        Enemy::sourceX = sourceX;
    }

public:
    Enemy(Piramide piramide, std::string nom, int i, int j, int xRespectCube, int yRespectCube) :
                                            Character(piramide, nom, i, j, DOWNRIGHT, xRespectCube, yRespectCube) {}

    bool hasChangingGroundPower() const {
        return changingGroundPower;
    }

    Direction getDir() const {
        return dir;
    }

    void randomMovement(){
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_int_distribution<int> dist(1, 2);
        if(dist(mt) == 1){
            dir = DOWNRIGHT;
        }else{
            dir = DOWNLEFT;
        }

        setJumping(true);
        sourceX -= 16;
    }
    void assignIJ(){
        if(dir == DOWNRIGHT){
            setI(getI()+1);
            setJ(getJ()+1);

        }else{//dir == DOWNLEFT
            setI(getI()+1);
        }
    }
};
#endif //ALLEGRO5TUTORIAL_ENEMY_HPP
