//
// Created by patri on 09/04/2021.
//

#ifndef QBERT_PLATILLO_HPP
#define QBERT_PLATILLO_HPP

constexpr static const float platilloSize = 16.0;

enum Posicion {
    IZQ, DER
};

class Platillo : public Objeto {
    bool jumping = false,
         falling = false;
    Direction dir;
    int airTimer = 0;
    int i, j; // coordenada respecto piramide (cubo)
    Posicion pos;

public:
    // Rango donde puede estar ubicado -> [i, -1] izq y [i, j] der tq. i=(-1:5), j=(i+1:6)

    /* Constructor */
    Platillo(Piramide piramide, int fila, Posicion pos) {
        this->yRespectCube = -16;
        this->i = fila;
        if(pos == IZQ){
            this->xRespectCube = -8;
            this->x = piramide.map[i][0].x + xRespectCube, this->y = piramide.map[i][0].y + yRespectCube;
            this->j = -1;

        } else { // "der"
            this->xRespectCube = 24;
            this->x = piramide.map[i][i%6].x + xRespectCube, this->y = piramide.map[i][i%6].y + yRespectCube;
            this->j = i+1;
        }

        ALLEGRO_BITMAP *bitmap = al_load_bitmap("../sprites/platillos.png");
        must_init(bitmap, "platillo");
        this->draw = bitmap;

        sourceX = 0;
        this-> sizePixels = 16;
    }

    void resize(Piramide piramide) override {
        if(pos == IZQ){
            setX(piramide.map[i][0].x+this->xRespectCube);
            setY(piramide.map[i][0].y+this->yRespectCube);
        } else { // "der"
            setX(piramide.map[i][i%6].x+this->xRespectCube);
            setY(piramide.map[i][i%6].y+this->yRespectCube);
        }
    }


    /*************************
     * GETTER'S AND SETTER'S *
     *************************/

    bool isJumping() const { return jumping; }
    void setJumping(bool jumping) { Platillo::jumping = jumping; }

    bool isFalling() const { return falling; }
    void setFalling(bool falling) { Platillo::falling = falling; }

    const Direction &getDir() const { return dir; }
    void setDir(const Direction &dir) { Platillo::dir = dir; }

    int getAirTimer() const { return airTimer; }
    void setAirTimer(int airTimer) { Platillo::airTimer = airTimer; }

    int getI() const { return i; }
    void setI(int i) { Platillo::i = i; }

    int getJ() const { return j; }
    void setJ(int j) { Platillo::j = j; }

};

#endif //QBERT_PLATILLO_HPP
