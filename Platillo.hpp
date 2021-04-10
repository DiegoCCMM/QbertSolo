//
// Created by patri on 09/04/2021.
//

#ifndef QBERT_PLATILLO_HPP
#define QBERT_PLATILLO_HPP

constexpr static const float platilloSize = 16.0;

class Platillo : public Objeto {
    bool jumping = false,
         falling = false;
    Direction dir;
    int airTimer = 0;
    int i, j; // coordenada respecto piramide (cubo)

public:
    // Rango donde puede estar ubicado -> [i, -1] izq y [i, j] der tq. i=(-1:5), j=(i+1:6)

    /* Constructor */
    Platillo(Piramide piramide, int fila, std::string posicion) {
        this->xRespectCube = 0, this->yRespectCube = -32;
        this->i = fila;
        if(posicion == "izq"){
            this->x = piramide.map[i][0].x + xRespectCube, this->y = piramide.map[i][0].y + yRespectCube;
            this->j = -1;
        } else { // "der"
            this->x = piramide.map[i][i+1].x + xRespectCube, this->y = piramide.map[i][i+1].y + yRespectCube;
            this->j = i+1;
        }
        //this->dir = dir;

        ALLEGRO_BITMAP *bitmap = al_load_bitmap("../sprites/platillos.png");
        must_init(bitmap, "platillo");
        this->draw = bitmap;
    }

    void drawBitmap(){
        al_draw_bitmap_region(getDraw(), getSourceX(), 0, platilloSize, platilloSize,
                              getX(), getY(), 0);
    }

    void must_init(bool test, const char *description) {
        if (test) return;

        printf("No se ha podido inicializar: %s\n", description);
        exit(1);
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
