//
// Created by diego on 13/3/21.
//
#include "Character.hpp"
#include "Player.hpp"
#include <random>

#ifndef ALLEGRO5TUTORIAL_ENEMY_HPP
#define ALLEGRO5TUTORIAL_ENEMY_HPP
class Enemy : public Character{
    Direction dir;
    int sourceX = 16;
public:
    int getSourceX() const {
        return sourceX;
    }

    void setSourceX(int sourceX) {
        Enemy::sourceX = sourceX;
    }

public:
    Enemy(float x, float y, ALLEGRO_BITMAP *draw, int i, int j, Direction dir) : Character(x, y, draw, i, j),
                                                                                 dir(dir) {}

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
};
#endif //ALLEGRO5TUTORIAL_ENEMY_HPP
