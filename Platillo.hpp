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

constexpr static const float velocidad[7] = {0.15, 0.3, 0.4, 0.55, 0.7, 0.85, 1};

class Platillo : public Objeto {
    bool jumping = false,
         falling = false;
    int i, j; // coordenada respecto piramide (cubo)
    Posicion pos;
    PosicionQBert posQBert = NONE;
    ALLEGRO_SAMPLE *moveSound = al_load_sample("../sounds/platillo.ogg");

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

        this-> sizePixelsX = 16;
        this-> sizePixelsY = 16;
    }

    /* Constructor */
    Platillo(float width, float height) {
        ALLEGRO_BITMAP *bitmap = al_load_bitmap("../sprites/platillos.png");
        must_init(bitmap, "platillo");
        this->draw = bitmap;

        this-> sizePixelsX = 16;
        this-> sizePixelsY = 16;
    }

    void drawBitmap() override {
        al_draw_bitmap_region(getDraw(), getSourceX() + (sourceJ * sizePixelsX),
                              getSourceY() + (sourceI * sizePixelsY), sizePixelsX, sizePixelsY,
                              getX(), getY(), 0);
        if(posQBert == MONTANDO) {
            al_clear_to_color(al_map_rgb(0, 223, 0));
        }
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
                playOnce(moveSound);
            }
        } else if(posQBert == SUBIENDO){
            if(getY() > piramide->map[i+1][0].y-30){
                yRespectCube -= 2.5;
                setY(getY()-2.5);
            } else {
                posQBert = MOVIMIENTO;
            }

        } else if(posQBert == MOVIMIENTO){
            float aux_x = 1.75*velocidad[i+1], aux_y = 2.5*velocidad[i+1];
            if(pos == IZQ){
                if(piramide->map[0][0].x+8 > getX()){
                    xRespectCube += aux_x;
                    setX(getX()+aux_x);
                } else {
                    posQBert = BAJANDO;
                }
            } else {
                if(piramide->map[0][0].x+8 < getX()){
                    xRespectCube -= aux_x;
                    setX(getX()-aux_x);
                } else {
                    posQBert = BAJANDO;
                }
            }

            yRespectCube -= aux_y;
            setY(getY()-aux_y);

        } else if(posQBert == BAJANDO){
            if(_y < piramide->map[0][0].y-8){
                _y += 5;
            } else {
                posQBert = NONE;
            }
        }
    }

    void destroy() override {
        al_destroy_bitmap(getDraw());
        al_destroy_sample(moveSound);
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
