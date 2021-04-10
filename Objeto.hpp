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

public:

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

};

#endif //QBERT_OBJETO_HPP
