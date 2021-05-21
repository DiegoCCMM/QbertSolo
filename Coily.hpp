//
// Created by diego on 13/3/21.
//
#include     <cmath>
#include "Platillo.hpp"

enum State {
    GOING_DOWN, AI, COULD_FALL, REACHING_LEDGE
};

#define VACIO 9

#ifndef ALLEGRO5TUTORIAL_COILY_HPP
#define ALLEGRO5TUTORIAL_COILY_HPP

class Coily : public Enemy {

    State state = GOING_DOWN;
    ALLEGRO_BITMAP *secondPhaseDraw;
    int reachQberti = VACIO, reachQbertj = VACIO;
    int IACoily;                                  //0 CLASICO, 1 MEDIO, 2 DIFICIL
    int qbertNosEspera = 0;//turnos de qbertEsperando
    std::list<Platillo> platillos;


public:
    Coily(const Piramide &piramide, const std::string &nom, int i, int j, int xRespectCube, int yRespectCube,
          int IACoily, std::list<Platillo> platillos) : Enemy(piramide, nom, i, j, xRespectCube, yRespectCube) {
        ALLEGRO_BITMAP *draw = al_load_bitmap("../sprites/coilyEstirado.png");
        must_init(draw, nom.c_str());
        secondPhaseDraw = draw;
        leftSprite = 16;
        this->IACoily = IACoily;
        Enemy::isCoily = true;
        this->platillos = platillos;
    }

    void setState(State s) {
        state = s;
    }

    bool coilyCouldFall() {
        return (state == COULD_FALL);
    }

    void AIMovement(const int &qberti, const int &qbertj) {

        if (!isFalling()) {//mientras no estemos cayendo nos movemos como siempre

            int coilyI = getI(), coilyJ = getJ();
            int dirI, dirJ;

            if (state == REACHING_LEDGE) {//si tenemos que llegar al borde, perseguimos la última posición de qbert
                dirI = coilyI - reachQberti;
                dirJ = coilyJ - reachQbertj;
            } else {//en cualquier otro caso, perseguimos la última posición de qbert que tenemos
                dirI = coilyI - qberti;
                dirJ = coilyJ - qbertj;
            }

            /*std::cout << "valor qbert i" << qberti << std::endl;
            std::cout << "valor qbert j" << qbertj << std::endl;

            std::cout << "valor i" << coilyI << std::endl;
            std::cout << "valor j" << coilyJ << std::endl;

            std::cout << "distancia i" << abs(dirI) << std::endl;
            std::cout << "distancia j" << abs(dirJ) << std::endl;*/

            //la diferencia de altura es igual en distancia en bloques
            int cercania = abs(dirI);
            if ((dirJ < 0 && dirI >= 0) || (dirI <= 0 && dirJ > 0)) {
                // Sólo tenemos diferencia lateral o la diferencia lateral no es en la misma dirección que la altura,
                // impidiendo usar los movimientos diagonales para ajustar ambos
                // , lo que implica que tendremos que hacer 2 movimientos
                // por cada diferencia lateral EJ de 5 4 a 5 3 sólo podemos llegar pisando antes 6 4 y luego 5 3
                cercania += 2 * abs(dirJ);
            }
            //la otra alternativa sería que dirJ y dirI tuvieran el mismo signo
            //significa que la diferencia lateral se puede solucionar usando dirI movimientos diagonales
            //y en el caso de que dirJ fuera menor que dirI, con los movimientos diagonales solucionamos la diferencia
            //lateral y sólo nos quedaría la diferencia de altura.
            //Es decir, que con dirI solucionamos dirJ y sólo nos queda dirI - dirJ que ya sabemos que es igual.



            //std::cout << "cercania " << cercania << std::endl;


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
                        if (dirJ == 0 && dirI == 0) {
                            setDir(DONTMOVE);
                        } else {
                            setDir(DOWNLEFT);
                        }
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

        Enemy::setJumping(true);
        Enemy::setSourceX(getSourceX() - leftSprite);
    }

    void AIMovement2(const int &qberti, const int &qbertj) {

        if (!isFalling()) {//mientras no estemos cayendo nos movemos como siempre

            int coilyI = getI(), coilyJ = getJ();
            int dirI, dirJ;

            if (state == REACHING_LEDGE) {//si tenemos que llegar al borde, perseguimos el top de la pirámide
                dirI = coilyI - reachQberti;
                dirJ = coilyJ - reachQbertj;
            } else if (qbertj < 0 || qbertj > qberti) {//si qbert se sube a platillo, buscamos cuspide
                dirI = coilyI - 0;
                dirJ = coilyJ - 0;
            } else {//en cualquier otro caso, perseguimos la última posición de qbert que tenemos
                dirI = coilyI - qberti;
                dirJ = coilyJ - qbertj;
            }

            /*std::cout << "valor qbert i" << qberti << std::endl;
            std::cout << "valor qbert j" << qbertj << std::endl;

            std::cout << "valor i" << coilyI << std::endl;
            std::cout << "valor j" << coilyJ << std::endl;

            std::cout << "distancia i" << abs(dirI) << std::endl;
            std::cout << "distancia j" << abs(dirJ) << std::endl;*/

            //la diferencia de altura es igual en distancia en bloques
            int cercania = abs(dirI);
            if ((dirJ < 0 && dirI >= 0) || (dirI <= 0 && dirJ > 0)) {
                // Sólo tenemos diferencia lateral o la diferencia lateral no es en la misma dirección que la altura,
                // impidiendo usar los movimientos diagonales para ajustar ambos
                // , lo que implica que tendremos que hacer 2 movimientos
                // por cada diferencia lateral EJ de 5 4 a 5 3 sólo podemos llegar pisando antes 6 4 y luego 5 3
                cercania += 2 * abs(dirJ);
            }
            //la otra alternativa sería que dirJ y dirI tuvieran el mismo signo
            //significa que la diferencia lateral se puede solucionar usando dirI movimientos diagonales
            //y en el caso de que dirJ fuera menor que dirI, con los movimientos diagonales solucionamos la diferencia
            //lateral y sólo nos quedaría la diferencia de altura.
            //Es decir, que con dirI solucionamos dirJ y sólo nos queda dirI - dirJ que ya sabemos que es igual.



            //std::cout << "cercania " << cercania << std::endl;


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
                        if (dirJ == 0 && dirI == 0) {
                            setDir(DONTMOVE);
                        } else {
                            setDir(DOWNLEFT);
                        }
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

        Enemy::setJumping(true);
        Enemy::setSourceX(getSourceX() - leftSprite);
    }

    void AIMovement3(const int &qberti, const int &qbertj) {

        if (!isFalling()) {//mientras no estemos cayendo nos movemos como siempre

            int coilyI = getI(), coilyJ = getJ();
            int dirI, dirJ;

            if (state == REACHING_LEDGE) {//si tenemos que llegar al borde, perseguimos la última posición de qbert
                dirI = coilyI - reachQberti;
                dirJ = coilyJ - reachQbertj;
            } else if (qbertj < 0 || qbertj > qberti) {//si qbert se sube a platillo, buscamos cuspide
                dirI = coilyI - 0;
                dirJ = coilyJ - 0;
            } else {//en cualquier otro caso, perseguimos la última posición de qbert que tenemos
                dirI = coilyI - qberti;
                dirJ = coilyJ - qbertj;
                if (qberti == reachQberti && qbertj == reachQbertj) {   //qbert está quieto
                    for(std::_List_iterator<Platillo> iter = platillos.begin(); iter != platillos.end(); iter++){
                        std::cout << " platillo pos i: " << iter->getI() + 1<< " platillo pos j -1: " << iter->getJ() -1<<
                        " platillo pos j +1: " << iter->getJ()+1 << std::endl;
                        std::cout << " qberti: " << qberti << " qbertj: " << qbertj<< std::endl;
                        if(iter->getI() + 1 == qberti && (iter->getJ() == qbertj  || iter->getJ() + 1 == qbertj)){   //qbert nos espera en un platillo
                            if (qbertNosEspera != 1) {  //es distinto de 1
                                qbertNosEspera++;
                            }
                        }
                    }
                }else if(qbertNosEspera == 1){
                    qbertNosEspera = 0;
                }
                reachQbertj = qbertj;
                reachQberti = qberti;
            }

            /*std::cout << "valor qbert i" << qberti << std::endl;
            std::cout << "valor qbert j" << qbertj << std::endl;

            std::cout << "valor i" << coilyI << std::endl;
            std::cout << "valor j" << coilyJ << std::endl;

            std::cout << "distancia i" << abs(dirI) << std::endl;
            std::cout << "distancia j" << abs(dirJ) << std::endl;*/

            //la diferencia de altura es igual en distancia en bloques
            int cercania = abs(dirI);
            if ((dirJ < 0 && dirI >= 0) || (dirI <= 0 && dirJ > 0)) {
                // Sólo tenemos diferencia lateral o la diferencia lateral no es en la misma dirección que la altura,
                // impidiendo usar los movimientos diagonales para ajustar ambos
                // , lo que implica que tendremos que hacer 2 movimientos
                // por cada diferencia lateral EJ de 5 4 a 5 3 sólo podemos llegar pisando antes 6 4 y luego 5 3
                cercania += 2 * abs(dirJ);
            }
            //la otra alternativa sería que dirJ y dirI tuvieran el mismo signo
            //significa que la diferencia lateral se puede solucionar usando dirI movimientos diagonales
            //y en el caso de que dirJ fuera menor que dirI, con los movimientos diagonales solucionamos la diferencia
            //lateral y sólo nos quedaría la diferencia de altura.
            //Es decir, que con dirI solucionamos dirJ y sólo nos queda dirI - dirJ que ya sabemos que es igual.



            //std::cout << "cercania " << cercania << std::endl;


            if (cercania <= 2 && qbertNosEspera != 1) { //si estamos "cerca" nos tiramos
                setState(COULD_FALL);
            } else if (coilyCouldFall() && abs(dirI) + abs(dirJ) > 1) {
                setState(AI);
            }
            std::cout << "la cercania es de :" << cercania << std::endl;
            if (qbertNosEspera == 1 && cercania <= 2) {   //si nos esperan, no nos movemos
                setDir(DONTMOVE);
            } else {
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
                            if (dirJ == 0 && dirI == 0) {
                                setDir(DONTMOVE);
                            } else {
                                setDir(DOWNLEFT);
                            }
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
        }

        assignIJ();

        Enemy::setJumping(true);
        Enemy::setSourceX(getSourceX() - leftSprite);
    }


    void setReachQberti(int reachQberti) {
        Coily::reachQberti = reachQberti;
    }

    void setReachQbertj(int reachQbertj) {
        Coily::reachQbertj = reachQbertj;
    }

    State getState() const {
        return state;
    }

    void randomMovement(int i, int j) override {
        //std::cout << "entro2" << std::endl;
        if (getI() == 6) { // anyadir condicion de que el estado anterior sea una bola
            state = AI;
            CsetJumpSound("coilyEstirado");
            setSourceX(14);
            setYRespectCube(-20);
        }
        if (state == GOING_DOWN) {
            Enemy::randomMovement(i, j);
        } else {
            if (IACoily == 0)
                Coily::AIMovement(i, j);
            else if (IACoily == 1)
                Coily::AIMovement2(i, j);
            else if (IACoily == 2)
                Coily::AIMovement3(i, j);
        }

    }

    void drawBitmap() override {
        if (state == GOING_DOWN) {
            al_draw_bitmap_region(getDraw(), getSourceX() + (sourceJ * sizePixelsX),
                                  getSourceY() + (sourceI * sizePixelsY), sizePixelsX, sizePixelsY,
                                  getX(), getY(), 0);
        } else {
            if (getDir() != DONTMOVE) {
                al_draw_bitmap_region(getDraw(), getSourceX() + (getDir() * 2 * sizePixelsX),
                                      getSourceY() + (sourceI * sizePixelsY), sizePixelsX, sizePixelsY * 2,
                                      getX(), getY(), 0);
            } else {
                al_draw_bitmap_region(getDraw(), getSourceX() + (DOWNRIGHT * 2 * sizePixelsX) + 16,
                                      getSourceY() + (sourceI * sizePixelsY), sizePixelsX, sizePixelsY * 2,
                                      getX(), getY(), 0);
            }
        }
    }

    ALLEGRO_BITMAP *getDraw() const override {
        if (state == GOING_DOWN) {
            return (draw);
        } else {
            return (secondPhaseDraw);
        }
    }
};

#endif //ALLEGRO5TUTORIAL_COILY_HPP
