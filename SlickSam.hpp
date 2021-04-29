//
// Created by diego on 29/4/21.
//

#ifndef ALLEGRO5TUTORIAL_SLICKSAM_HPP
#define ALLEGRO5TUTORIAL_SLICKSAM_HPP

class SlickSam : public Enemy{

    State state = GOING_DOWN;
    ALLEGRO_BITMAP* secondPhaseDraw;
    int reachQberti = VACIO,  reachQbertj = VACIO;

public:
    SlickSam(const Piramide &piramide, const std::string &nom, int i, int j, int xRespectCube, int yRespectCube) : Enemy(
            piramide, nom, i, j, xRespectCube, yRespectCube) {
        setChangingGroundPower(true);
    }

    void randomMovement(int i, int j) override {



    }

    void drawBitmap() override {
            al_draw_bitmap_region(getDraw(), getSourceX() + (getDir() * 2 * sizePixelsX),
                                  getSourceY() + (sourceI * sizePixelsY), sizePixelsX, sizePixelsY*2,
                                  getX(), getY(), 0);
    }
};

#endif //ALLEGRO5TUTORIAL_SLICKSAM_HPP
