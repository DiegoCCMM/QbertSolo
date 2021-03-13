//
// Created by diego on 13/3/21.
//

#ifndef ALLEGRO5TUTORIAL_CHARACTER_HPP
#define ALLEGRO5TUTORIAL_CHARACTER_HPP

#include <allegro5/allegro.h>

class Character{
    float x;    // display coordinates
    float y;
    ALLEGRO_BITMAP *draw;   //sprite
    int i, j; // coordenada cubo

public:
    Character(float x, float y, ALLEGRO_BITMAP *draw) : x(x), y(y), draw(draw) {}

    void setX(float x) {
        Character::x = x;
    }

    int getI() const {
        return i;
    }

    void setI(int i) {
        Character::i = i;
    }

    int getJ() const {
        return j;
    }

    void setJ(int j) {
        Character::j = j;
    }

    void setY(float y) {
        Character::y = y;
    }

    void setDraw(ALLEGRO_BITMAP *draw) {
        Character::draw = draw;
    }
};

#endif //ALLEGRO5TUTORIAL_CHARACTER_HPP
