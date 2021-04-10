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
    int i, j; // coordenada respecto piramide (cubo)
    Posicion pos;

public:
    // Rango fila [0,6]

    /* Constructor */
    Platillo(Piramide piramide, int fila, Posicion pos) {
        this->yRespectCube = -16;
        this->i = fila-1;
        if(pos == IZQ){
            this->xRespectCube = -8;
            this->j = -1;
        } else { // "der"
            this->xRespectCube = 24;
            this->j = i+2;
        }
        this->x = piramide.map[i+1][j+1].x + xRespectCube, this->y = piramide.map[i+1][j+1].y + yRespectCube;

        ALLEGRO_BITMAP *bitmap = al_load_bitmap("../sprites/platillos.png");
        must_init(bitmap, "platillo");
        this->draw = bitmap;

        this-> sizePixels = 16;
    }

    void resize(Piramide *piramide) override {
        setX(piramide->map[i+1][j+1].x + xRespectCube);
        setY(piramide->map[i+1][j+1].y + yRespectCube);
    }

    void movement() override{
        if(++timer > 2) {
            if(++sourceJ > 3) sourceJ = 0;
            timer = 0;
        }
    }


    /*************************
     * GETTER'S AND SETTER'S *
     *************************/

    bool isJumping() const { return jumping; }
    void setJumping(bool _jumping) { Platillo::jumping = _jumping; }

    bool isFalling() const { return falling; }
    void setFalling(bool _falling) { Platillo::falling = _falling; }

    int getI() const { return i; }
    void setI(int _i) { Platillo::i = _i; }

    int getJ() const { return j; }
    void setJ(int _j) { Platillo::j = _j; }

};

#endif //QBERT_PLATILLO_HPP
