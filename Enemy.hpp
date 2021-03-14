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
public:
    void randomMovement(){
        //TODO CHOOSE A RANDOM DIRECTION BETWEEN DOWNRIGHT AND DOWNLEFT

    }
};
#endif //ALLEGRO5TUTORIAL_ENEMY_HPP
