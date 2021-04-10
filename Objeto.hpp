//
// Created by patri on 10/04/2021.
//

#ifndef QBERT_OBJETO_HPP
#define QBERT_OBJETO_HPP

class Objeto {
protected:
    float x, y;             // display coordinates
    float xRespectCube, yRespectCube;
    ALLEGRO_BITMAP *draw;   // sprite
    int sourceX;
    int sizePixels;

public:

    virtual void drawBitmap(){
        al_draw_bitmap_region(getDraw(), getSourceX(), 0, sizePixels, sizePixels,
                              getX(), getY(), 0);
    }

    virtual void resize(Piramide piramide) {
        setX(piramide.map[0][0].x+this->xRespectCube);
        setY(piramide.map[0][0].y+this->yRespectCube);
    }

    virtual void destroy() {
        al_destroy_bitmap(getDraw());
    }

    int getSourceX() const { return sourceX; }
    void setSourceX(int _sourceX) { Objeto::sourceX = _sourceX; }

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

    void must_init(bool test, const char *description) {
        if (test) return;

        printf("No se ha podido inicializar: %s\n", description);
        exit(1);
    }

};

#endif //QBERT_OBJETO_HPP
