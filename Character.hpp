//
// Created by diego on 13/3/21.
//

#ifndef ALLEGRO5TUTORIAL_CHARACTER_HPP
#define ALLEGRO5TUTORIAL_CHARACTER_HPP

#include <vector>
#include <allegro5/allegro.h>
#include <iostream>

constexpr static const float movementX = 4.0;
constexpr static const float movementY = 12.0;
constexpr static const int framePixels = 16;
constexpr static const int airTime = 6;

enum Direction {
    TOPRIGHT, TOPLEFT, DOWNRIGHT, DOWNLEFT
};

class Character{
    float x, y;             // display coordinates
    float xRespectCube, yRespectCube;
    ALLEGRO_BITMAP *draw;   // sprite
    bool jumping = false,
         falling = false;
    Direction dir;
    int sourceX = 0;
    int airTimer = 0;
    int i, j; // coordenada cubo

    ALLEGRO_SAMPLE *jumpSound = nullptr;

public:

    /* Constructor */
    Character(Piramide piramide, std::string nom, int i, int j, Direction dir, int xRespectCube, int yRespectCube) {
        this->x = piramide.map[i][j].x + xRespectCube, this->y =piramide.map[i][j].y + yRespectCube;
        this->i = i, this->j = j;
        this->dir = dir;
        this->xRespectCube = xRespectCube, this->yRespectCube = yRespectCube;

        std::string path = "../sprites/" + nom + ".png";
        ALLEGRO_BITMAP *player = al_load_bitmap(path.c_str());
        must_init(player, nom.c_str());
        this->draw = player;

        CsetJumpSound(nom);
    }

    virtual void resize(Piramide *piramide) {
        Character::setX(piramide->map[getI()][getJ()].x+this->xRespectCube);
        Character::setY(piramide->map[getI()][getJ()].y+this->yRespectCube);
    }

    virtual void movement(Piramide *piramide, int HEIGHT) {}

    virtual void drawBitmap(){
        al_draw_bitmap_region(Character::getDraw(), Character::getSourceX(), 0, framePixels, framePixels,
                              Character::getX(), Character::getY(), 0);
    }

    void playOnce(ALLEGRO_SAMPLE *sound){
        al_play_sample(sound, 1.0, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
    }

    virtual void destroy() {
        al_destroy_bitmap(getDraw());
        al_destroy_sample(getJumpSound());
    }

    void must_init(bool test, const char *description) {
        if (test) return;

        printf("No se ha podido inicializar: %s\n", description);
        exit(1);
    }

    /*************************
     * GETTER'S AND SETTER'S *
     *************************/

    int getSourceX() const { return sourceX; }
    void setSourceX(int _sourceX) { Character::sourceX = _sourceX; }

    float getXRespectCube() const { return xRespectCube; }
    void setXRespectCube(float _xRespectCube) { Character::xRespectCube = _xRespectCube; }

    float getYRespectCube() const { return yRespectCube; }
    void setYRespectCube(float _yRespectCube) { Character::yRespectCube = _yRespectCube; }

    virtual Direction getDir() const { return dir; }
    void setDir(Direction _dir) { Character::dir = _dir; }

    bool isFalling() const { return falling; }
    void setFalling(bool _falling) { Character::falling = _falling; }

    int getAirTimer() const { return airTimer; }
    void setAirTimer(int _airTimer) { Character::airTimer = _airTimer; }
    void airTimerplusplus(){ Character::airTimer++; }

    bool isJumping() const { return jumping; }
    void setJumping(bool _jumping) { Character::jumping = _jumping; }

    int getI() const { return i; }
    void setI(int _i) { Character::i = _i; }

    int getJ() const { return j; }
    void setJ(int _j) { Character::j = _j; }

    float getX() const { return x; }
    void setX(float _x) { Character::x = _x; }

    float getY() const { return y; }
    void setY(float _y) { Character::y = _y; }

    ALLEGRO_BITMAP *getDraw() const { return draw; }
    void setDraw(ALLEGRO_BITMAP *_draw) { Character::draw = _draw; }

    ALLEGRO_SAMPLE *getJumpSound() const { return jumpSound; }
    void CsetJumpSound(std::string nom) {
        Character::jumpSound = al_load_sample(("../sounds/" + nom + "-jump.ogg").c_str());
        must_init(jumpSound, ("../sounds/" + nom + "-jump.ogg").c_str());
    }

};

#endif //ALLEGRO5TUTORIAL_CHARACTER_HPP
