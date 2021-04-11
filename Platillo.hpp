//
// Created by patri on 09/04/2021.
//

#ifndef QBERT_PLATILLO_HPP
#define QBERT_PLATILLO_HPP

enum Posicion {
    IZQ, DER
};

enum PosicionQBert {
    NONE, MONTANDO, SUBIENDO, MOVIMIENTO, BAJANDO
};

class Platillo : public Objeto {
    bool jumping = false,
         falling = false;
    int i, j; // coordenada respecto piramide (cubo)
    Posicion pos;
    PosicionQBert posQBert = NONE;

public:
    // Rango fila [0,6]

    /* Constructor */
    Platillo(Piramide piramide, int fila, Posicion pos) {
        this->yRespectCube = -16;
        this->pos = pos;
        this->i = fila-1;
        if(pos == IZQ){
            this->xRespectCube = -9;
            this->j = -1;
            this->x = piramide.map[i+1][j+1].x + xRespectCube, this->y = piramide.map[i+1][j+1].y + yRespectCube;
        } else { // "der"
            this->xRespectCube = 24;
            this->j = i+1;
            this->x = piramide.map[i+1][j].x + xRespectCube, this->y = piramide.map[i+1][j].y + yRespectCube;
        }

        ALLEGRO_BITMAP *bitmap = al_load_bitmap("../sprites/platillos.png");
        must_init(bitmap, "platillo");
        this->draw = bitmap;

        this-> sizePixels = 16;
    }

    void resize(Piramide *piramide) override {
        if(pos == IZQ){
            setX(piramide->map[i+1][j+1].x + xRespectCube);
            setY(piramide->map[i+1][j+1].y + yRespectCube);
        } else { // "der"
            setX(piramide->map[i+1][j].x + xRespectCube);
            setY(piramide->map[i+1][j].y + yRespectCube);
        }
    }

    void movement() override{
        if(++timer > 2) {
            if(++sourceJ > 3) {
                sourceJ = 0;
            }
            timer = 0;
        }
    }

    /* Actualiza las coordenadas (x,y) de Q*Bert respecto al platillo y su estado en el */
    void updateWithQBert(float &_x, float &_y, Piramide *piramide){
        if(posQBert == MONTANDO){
            if(_y <= getY()-11){
                _y += 5;
            } else {
                posQBert = SUBIENDO;
            }
        } else if(posQBert == SUBIENDO){
            std::cout << piramide->map[i+1][0].y << std::endl;
            std::cout << getY() << std::endl;
            if(getY() > piramide->map[i+1][0].y-30){
                std::cout << piramide->map[i+1][0].y << std::endl;
                std::cout << getY() << std::endl;
                yRespectCube -= 2.5;
                setY(getY()-2.5);
            } else {
                posQBert = MOVIMIENTO;
            }

        } else if(posQBert == MOVIMIENTO){
            if(pos == IZQ){
                if(piramide->map[0][0].x+8 > getX()){
                    xRespectCube += 1.75;
                    setX(getX()+1.75);
                } else {
                    posQBert = BAJANDO;
                }
            } else {
                if(piramide->map[0][0].x+8 < getX()){
                    xRespectCube -= 1.75;
                    setX(getX()-1.75);
                } else {
                    posQBert = BAJANDO;
                }
            }

            yRespectCube -= 2.5;
            setY(getY()-2.5);

        } else if(posQBert == BAJANDO){
            if(_y < piramide->map[0][0].y-8){
                _y += 5;
            } else {
                posQBert = NONE;
            }
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

    Posicion getPos() const { return pos; }
    void setPos(Posicion _pos) { Platillo::pos = _pos; }

    PosicionQBert getPosQBert() const { return posQBert; }
    void setPosQBert(PosicionQBert _posQBert) { Platillo::posQBert = _posQBert; }

};

#endif //QBERT_PLATILLO_HPP
