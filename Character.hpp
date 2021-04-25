//
// Created by diego on 13/3/21.
//

#ifndef ALLEGRO5TUTORIAL_CHARACTER_HPP
#define ALLEGRO5TUTORIAL_CHARACTER_HPP

#include <vector>
#include <allegro5/allegro.h>
#include <iostream>
#include "Objeto.hpp"

constexpr static const float movementX = 4.0;
constexpr static const float movementY = 12.0;
constexpr static const int airTime = 6;

enum Direction {
    TOPRIGHT, TOPLEFT, DOWNRIGHT, DOWNLEFT
};

class Character : public Objeto {
    bool jumping = false,
         falling = false;
    Direction dir;
    int i, j; // coordenada cubo

    ALLEGRO_SAMPLE *jumpSound = nullptr;

public:

    /* Constructor */
    Character(Piramide piramide, std::string nom, int i, int j, Direction dir, int xRespectCube, int yRespectCube) {
        this->x = piramide.map[i][j].x + xRespectCube, this->y = piramide.map[i][j].y + yRespectCube;
        this->i = i, this->j = j;
        this->dir = dir;
        this-> sizePixelsX = 16;
        this-> sizePixelsY = 16;
        this->xRespectCube = xRespectCube, this->yRespectCube = yRespectCube;

        std::string path = "../sprites/" + nom + ".png";
        ALLEGRO_BITMAP *player = al_load_bitmap(path.c_str());
        must_init(player, nom.c_str());
        this->draw = player;

        CsetJumpSound(nom);
    }

    /* Constructor 2 */
    Character(std::string nom, Direction dir){
        this->dir = dir;
        this-> sizePixelsX = 16;
        this-> sizePixelsY = 16;

        std::string path = "../sprites/" + nom + ".png";
        ALLEGRO_BITMAP *player = al_load_bitmap(path.c_str());
        must_init(player, nom.c_str());
        this->draw = player;

        CsetJumpSound(nom);
    }

    void resize(Piramide *piramide) override {
        Character::setX(piramide->map[getI()][getJ()].x+this->xRespectCube);
        Character::setY(piramide->map[getI()][getJ()].y+this->yRespectCube);
    }

    virtual void movement(Piramide *piramide, int HEIGHT, int &i, int &j) {}

    void destroy() override {
        al_destroy_bitmap(getDraw());
        al_destroy_sample(getJumpSound());
    }

    /*************************
     * GETTER'S AND SETTER'S *
     *************************/

    virtual Direction getDir() const { return dir; }
    void setDir(Direction _dir) { Character::dir = _dir; }

    bool isFalling() const { return falling; }
    void setFalling(bool _falling) { Character::falling = _falling; }

    bool isJumping() const { return jumping; }
    void setJumping(bool _jumping) { Character::jumping = _jumping; }

    int getI() const { return i; }
    void setI(int _i) { Character::i = _i; }

    int getJ() const { return j; }
    void setJ(int _j) { Character::j = _j; }

    ALLEGRO_SAMPLE *getJumpSound() const { return jumpSound; }
    void CsetJumpSound(std::string nom) {
        Character::jumpSound = al_load_sample(("../sounds/" + nom + "-jump.ogg").c_str());
        must_init(jumpSound, ("../sounds/" + nom + "-jump.ogg").c_str());
    }

};

#endif //ALLEGRO5TUTORIAL_CHARACTER_HPP
