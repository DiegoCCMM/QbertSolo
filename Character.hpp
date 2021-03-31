//
// Created by diego on 13/3/21.
//

#ifndef ALLEGRO5TUTORIAL_CHARACTER_HPP
#define ALLEGRO5TUTORIAL_CHARACTER_HPP

#include <allegro5/allegro.h>

enum Direction {
    TOPRIGHT, TOPLEFT, DOWNRIGHT, DOWNLEFT
};

class Character{
    float x, y;             // display coordinates
    ALLEGRO_BITMAP *draw;   //sprite
    bool jumping = false,
         falling = false;
    Direction dir;
    float xRespectCube, yRespectCube;

public:
    float getXRespectCube() const {
        return xRespectCube;
    }

    void setXRespectCube(float xRespectCube) {
        Character::xRespectCube = xRespectCube;
    }

    float getYRespectCube() const {
        return yRespectCube;
    }

    void setYRespectCube(float yRespectCube) {
        Character::yRespectCube = yRespectCube;
    }

public:
    Direction getDir() const {
        return dir;
    }

    void setDir(Direction dir) {
        Character::dir = dir;
    }

public:
    bool isFalling() const {
        return falling;
    }

    void setFalling(bool falling) {
        Character::falling = falling;
    }

private:
    int airTimer = 0;
public:
    int getAirTimer() const {
        return airTimer;
    }

    void setAirTimer(int airTimer) {
        Character::airTimer = airTimer;
    }
    void airTimerplusplus(){
        airTimer++;
    }

public:
    bool isJumping() const {
        return jumping;
    }

    void setJumping(bool jumping) {
        Character::jumping = jumping;
    }

private:
    int i, j; // coordenada cubo

public:
    Character(float x, float y, ALLEGRO_BITMAP *draw, Direction dir) : x(x), y(y), draw(draw), dir(dir) {}

    void setX(float x) {
        Character::x = x;
    }

    void must_init(bool test, const char *description) {
        if (test) return;

        printf("couldn't initialize %s\n", description);
        exit(1);
    }

    Character(Piramide piramide, std::string nom, int i, int j, Direction dir, int xRespectCube, int yRespectCube) {
        this->x = piramide.map[i][j].x + xRespectCube, this->y =piramide.map[i][j].y + yRespectCube;
        this->i = i, this->j = j;
        this->dir = dir;
        this->xRespectCube = xRespectCube, this->yRespectCube = yRespectCube;

        std::string path = "../sprites/" + nom + ".png";
        ALLEGRO_BITMAP *player = al_load_bitmap(path.c_str());
        must_init(player, nom.c_str());
        this->draw = player;
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

    float getX() const {
        return x;
    }

    float getY() const {
        return y;
    }

    ALLEGRO_BITMAP *getDraw() const {
        return draw;
    }

    void setY(float y) {
        Character::y = y;
    }

    void setDraw(ALLEGRO_BITMAP *draw) {
        Character::draw = draw;
    }

    virtual void resize(Piramide *piramide) {
        Character::setX(piramide->map[getI()][getJ()].x+this->xRespectCube);
        Character::setY(piramide->map[getI()][getJ()].y+this->yRespectCube);
    }
};

#endif //ALLEGRO5TUTORIAL_CHARACTER_HPP
