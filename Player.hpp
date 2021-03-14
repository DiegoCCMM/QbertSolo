//
// Created by diego on 13/3/21.
//
#include "Character.hpp"
#ifndef ALLEGRO5TUTORIAL_PLAYER_HPP
#define ALLEGRO5TUTORIAL_PLAYER_HPP
enum Direction {
    TOPRIGHT, TOPLEFT, DOWNRIGHT, DOWNLEFT
};


class Player : public Character{
    Direction dir = DOWNRIGHT;
    int lives = 3;
    int score = 0;
public:
    int getLives() const {
        return lives;
    }

    void setLives(int lives) {
        Player::lives = lives;
    }

    int getScore() const {
        return score;
    }

    void setScore(int score) {
        Player::score = score;
    }

public:
    Player(float x, float y, ALLEGRO_BITMAP *draw, Direction dir) : Character(x, y, draw), dir(dir) {}

    Player(float x, float y, ALLEGRO_BITMAP *draw, int i, int j, Direction dir) : Character(x, y, draw, i, j),
                                                                                  dir(dir) {}

    Direction getDir() const {
        return dir;
    }

    void setDir(Direction dir) {
        Player::dir = dir;
    }
};


#endif //ALLEGRO5TUTORIAL_PLAYER_HPP
