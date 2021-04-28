//
// Created by diego on 13/3/21.
//

enum State{
    GOING_DOWN, AI, COULD_FALL, FALLING
};

#ifndef ALLEGRO5TUTORIAL_COILY_HPP
#define ALLEGRO5TUTORIAL_COILY_HPP
class Coily : public Enemy{

    State state = GOING_DOWN;
    ALLEGRO_BITMAP* secondPhaseDraw;

public:
    Coily(const Piramide &piramide, const std::string &nom, int i, int j, int xRespectCube, int yRespectCube) : Enemy(
            piramide, nom, i, j, xRespectCube, yRespectCube) {
        ALLEGRO_BITMAP *draw = al_load_bitmap("../sprites/coilyEstirado.png");
        must_init(draw, nom.c_str());
        secondPhaseDraw = draw;
        leftSprite = 16;
    }

    void setState(State s){
        state = s;
    }

    void AIMovement(const int& qberti, const int& qbertj){

        int coilyI = getI(), coilyJ = getJ();

        int dirI = coilyI - qberti;

        int dirJ = coilyJ - qbertj;

        if(dirI > 0){//qbert esta por arriba 7 - 0 es 7, qbert en el sitio inicial, coily abajo max
            if(dirJ > 0){ //qbert esta por la izq, 7 - 0 es 7, qbert en el inicial, coily a la derecha del to-do
                setDir(TOPLEFT);
            }else{ //qbert esta por la derecha
                setDir(TOPRIGHT);
            }
        }else{//qbert esta por abajo 0 - 7 es -7, qbert en abajo max, coily arriba
            if(dirJ >= 0){
                setDir(DOWNLEFT);
            }else{
                setDir(DOWNRIGHT);
            }
        }

        assignIJ();

        Enemy::setJumping(true);
        Enemy::setSourceX(getSourceX()- leftSprite);
    }

    void randomMovement(const int& i, const int& j) override {

        if(getI() == 6){
            state = AI;
            setSourceX(14);
            setYRespectCube( -20 );
        }
        if(state == GOING_DOWN){
            Enemy::randomMovement(i, j);
        }else{
            Coily::AIMovement(i, j);
        }

    }

    void drawBitmap() override {
        if(state == GOING_DOWN) {
            al_draw_bitmap_region(getDraw(), getSourceX() + (sourceJ * sizePixelsX),
                                  getSourceY() + (sourceI * sizePixelsY), sizePixelsX, sizePixelsY,
                                  getX(), getY(), 0);
        }else{
            al_draw_bitmap_region(getDraw(), getSourceX() + (getDir() * 2 * sizePixelsX),
                                  getSourceY() + (sourceI * sizePixelsY), sizePixelsX, sizePixelsY*2,
                                  getX(), getY(), 0);
        }
    }

    ALLEGRO_BITMAP* getDraw() const override {
        if(state == GOING_DOWN){
            return(draw);
        }else{
            return(secondPhaseDraw);
        }
    }
};
#endif //ALLEGRO5TUTORIAL_COILY_HPP
