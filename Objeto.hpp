//
// Created by patri on 10/04/2021.
//

#ifndef ALLEGRO5TUTORIAL_OBJETO_HPP
#define ALLEGRO5TUTORIAL_OBJETO_HPP

class Objeto {
protected:
    float x, y;             // display coordinates
    float xRespectCube, yRespectCube;
    ALLEGRO_BITMAP *draw;   // sprite
    int sourceX = 0, sourceY = 0;
    int sourceI = 0, sourceJ = 0;
    int sizePixels;
    int timer = 0;

public:

    virtual void drawBitmap(){
        al_draw_bitmap_region(getDraw(), getSourceX() + (sourceJ * sizePixels),
                              getSourceY() + (sourceI * sizePixels), sizePixels, sizePixels,
                              getX(), getY(), 0);
    }

    virtual void resize(Piramide *piramide) {
        setX(piramide->map[0][0].x+this->xRespectCube);
        setY(piramide->map[0][0].y+this->yRespectCube);
    }

    virtual void destroy() {
        al_destroy_bitmap(getDraw());
    }

    virtual void movement() {}

    void playOnce(ALLEGRO_SAMPLE *sound){
        al_play_sample(sound, 1.0, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
    }

    void timerplusplus(){ Objeto::timer++; }


    /*************************
     * GETTER'S AND SETTER'S *
     *************************/

    int getSourceX() const { return sourceX; }
    void setSourceX(int _sourceX) { Objeto::sourceX = _sourceX; }

    int getSourceY() const { return sourceY; }
    void setSourceY(int _sourceY) { Objeto::sourceY = _sourceY; }

    float getXRespectCube() const { return xRespectCube; }
    void setXRespectCube(float _xRespectCube) { Objeto::xRespectCube = _xRespectCube; }

    float getYRespectCube() const { return yRespectCube; }
    void setYRespectCube(float _yRespectCube) { Objeto::yRespectCube = _yRespectCube; }

    float getX() const { return x; }
    void setX(float _x) { Objeto::x = _x; }

    float getY() const { return y; }
    void setY(float _y) { Objeto::y = _y; }

    ALLEGRO_BITMAP *getDraw() const { return draw; }
    void setDraw(ALLEGRO_BITMAP *_draw) { Objeto::draw = _draw; }

    int getTimer() const { return timer; }
    void setTimer(int _timer) { Objeto::timer = _timer; }

    int getSourceI() const { return sourceI; }
    void setSourceI(int _sourceI) { Objeto::sourceI = _sourceI; }

    int getSourceJ() const { return sourceJ; }
    void setSourceJ(int _sourceJ) { Objeto::sourceJ = _sourceJ; }

    void must_init(bool test, const char *description) {
        if (test) return;

        printf("No se ha podido inicializar: %s\n", description);
        exit(1);
    }

};

#endif //ALLEGRO5TUTORIAL_OBJETO_HPP
