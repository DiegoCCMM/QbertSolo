//
// Created by diego on 13/3/21.
//
#include     <cmath>
enum State{
    GOING_DOWN, AI, COULD_FALL, REACHING_LEDGE, FALLING
};

#define VACIO -10

#ifndef ALLEGRO5TUTORIAL_COILY_HPP
#define ALLEGRO5TUTORIAL_COILY_HPP
class Coily : public Enemy{

    State state = GOING_DOWN;
    ALLEGRO_BITMAP* secondPhaseDraw;
    int reachQberti = VACIO,  reachQbertj = VACIO;

public:
    Coily(const Piramide &piramide, const std::string &nom, int i, int j, int xRespectCube, int yRespectCube) : Enemy(
            piramide, nom, i, j, xRespectCube, yRespectCube) {
        ALLEGRO_BITMAP *draw = al_load_bitmap("../sprites/coilyEstirado.png");
        must_init(draw, nom.c_str());
        secondPhaseDraw = draw;
        leftSprite = 16;
        Enemy::isCoily = true;
    }

    void setState(State s){
        state = s;
    }

    bool coilyCouldFall(){
        return(state == COULD_FALL);
    }

    void AIMovement(const int& qberti, const int& qbertj){
        if(state == REACHING_LEDGE && reachQberti == VACIO){
            //si tenemos que llegar al borde y no tenemos última posición de qbert, la pillamos para llegar al borde
            reachQberti = qberti;
            reachQbertj = qbertj;
        }

        if(state != FALLING) {//mientras no estemos cayendo nos movemos como siempre

            int coilyI = getI(), coilyJ = getJ();
            int dirI, dirJ;

            if(state == REACHING_LEDGE){//si tenemos que llegar al borde, perseguimos la última posición de qbert
                dirI = coilyI - reachQberti;
                dirJ = coilyJ - reachQbertj;
            } else {//en cualquier otro caso, perseguimos la última posición de qbert que tenemos
                dirI = coilyI - qberti;
                dirJ = coilyJ - qbertj;
            }

            std::cout << "valor qbert i" << qberti << std::endl;
            std::cout << "valor qbert j" << qbertj << std::endl;
/*
            std::cout << "valor i" << coilyI << std::endl;
            std::cout << "valor j" << coilyJ << std::endl;
*/
            std::cout << "distancia i" << abs(dirI) << std::endl;
            std::cout << "distancia j" << abs(dirJ) << std::endl;
            //TODO REHACER CERCANIA
            int cercania = fmax(abs(dirI), abs(dirJ));
            if(cercania == dirJ && cercania != dirI){
                cercania = cercania * 2;
            }
            std::cout << "cercania " << cercania << std::endl;
            //TODO REHACER LÓGICA DE COULD_FALL DECISION
            if (cercania <= 3) { //si estamos "cerca" nos tiramos
                setState(COULD_FALL);
            } else if (coilyCouldFall() && abs(dirI) + abs(dirJ) > 4) {
                setState(AI);
            }

            if (dirI > 0) {//qbert esta por arriba 7 - 0 es 7, qbert en el sitio inicial, coily abajo max
                if (dirJ > 0) { //qbert esta por la izq, 7 - 0 es 7, qbert en el inicial, coily a la derecha del to-do
                    setDir(TOPLEFT);
                } else { //qbert esta por la derecha
                    setDir(TOPRIGHT);
                }
            } else {//qbert esta por abajo 0 - 7 es -7, qbert en abajo max, coily arriba
                if (dirJ >= 0) {
                    if (coilyI == 6 || coilyJ == 6) {
                        setDir(TOPLEFT);
                    } else {
                        setDir(DOWNLEFT);
                    }
                } else {
                    if (coilyI == 6 || coilyJ == 6) {
                        setDir(TOPRIGHT);
                    } else {
                        setDir(DOWNRIGHT);
                    }
                }
            }
        }

        assignIJ();
        if((getI() == getJ() || getI() == 0) && state == REACHING_LEDGE){
            state = FALLING;
            //TODO FALLING ANIMATION OR SO
        }

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
