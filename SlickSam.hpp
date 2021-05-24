//
// Created by diego on 29/4/21.
//

#include "Enemy.hpp"

#ifndef ALLEGRO5TUTORIAL_SLICKSAM_HPP
#define ALLEGRO5TUTORIAL_SLICKSAM_HPP

class SlickSam : public Enemy{

    bool repeticion = false;
    int IASlickSam;         //0 CLASICO, 1 MEDIO, 2 DIFICIL

public:
    SlickSam(const Piramide &piramide, const std::string &nom, int i, int j, int xRespectCube, int yRespectCube, int IASlickSam) : Enemy(
            piramide, nom, i, j, xRespectCube, yRespectCube) {
        this->IASlickSam = IASlickSam;
        setChangingGroundPower(true);
    }


    void IAnivel2(int i, int j) {
        std::random_device rd;
        std::mt19937 mt(std::chrono::system_clock::now().time_since_epoch().count());
        std::uniform_int_distribution<int> dist2(1, 2);

        if (dist2(mt) == 1) {//50% jugamos como siempre
            IAnivel1(i, j);
        } else {   //50% IA que evita a qbert
            IAnivel3(i, j);
        }


    }

    void IAnivel3(int qberti, int qbertj) {
        //std::cout<<"empezamos"<<std::endl;
        int cercania =  distanciaQbert(qberti, qbertj);

        //precalculo del siguiente paso y eliminación del que no se pueda
        int cercaniaTL = distanciaQbert(qberti, qbertj, getI()-1, getJ()-1);
        int cercaniaTR = distanciaQbert(qberti, qbertj, getI()-1, getJ());
        int cercaniaDL = distanciaQbert(qberti, qbertj, getI()+1, getJ());
        int cercaniaDR = distanciaQbert(qberti, qbertj, getI()+1, getJ()+1);

        int cercaniaEsquinaAbajoIzq = distanciaQbert(6, 0, getI(), getJ());
        int cercaniaEsquinaArriba = distanciaQbert(0, 0, getI(), getJ());
        int cercaniaEsquinaAbajoDer = distanciaQbert(6, 6, getI(), getJ());
//        std::cout << "cercaniaEsquinaAbajoDer " << cercaniaEsquinaAbajoDer << std::endl;

        std::list<Direction> siguientePaso = {TOPLEFT, TOPRIGHT, DOWNLEFT, DOWNRIGHT};
        //ni vamos hacia qbert ni nos caemos

        if(cercaniaTL < cercania || getJ() == 0){
            siguientePaso.remove(TOPLEFT);
        }if(cercaniaTR < cercania || getJ() == getI()){
            siguientePaso.remove(TOPRIGHT);
        }if(cercaniaDL < cercania || getI() == 6){
            siguientePaso.remove(DOWNLEFT);
        }if(cercaniaDR < cercania || getI() == 6){
            siguientePaso.remove(DOWNRIGHT);
        }

        //evitamos las esquinas
        if(cercaniaEsquinaAbajoIzq <= 1){
            siguientePaso.remove(DOWNLEFT);
        }if(cercaniaEsquinaArriba <= 1){
            siguientePaso.remove(TOPLEFT);
            siguientePaso.remove(TOPRIGHT);
        }if(cercaniaEsquinaAbajoDer <= 1){

            siguientePaso.remove(DOWNRIGHT);
        }

        std::random_device rd;
        std::mt19937 mt(std::chrono::system_clock::now().time_since_epoch().count());
        std::uniform_int_distribution<int> dist2(1, siguientePaso.size());

        if(siguientePaso.size() == 0){//parcheo del bug, si no tiene movimiento, elegimos esos dos, bug equina izquierda
            if(getI() == 6 && getJ() == 1){
                siguientePaso.push_back(TOPLEFT);
            }else if(getI() == 5 && getJ() == 0) {
                siguientePaso.push_back(DOWNRIGHT);
            }
            else if(getI() == 5 && getJ() == 5) {
                siguientePaso.push_back(DOWNLEFT);
            }
            else if(getI() == 1 && getJ() == 1){
                siguientePaso.push_back(DOWNRIGHT);
            }
        }

        auto sp_front = siguientePaso.begin();
        if(siguientePaso.size() != 1) {//elegimos una posibilidad de forma aleatoria
            std::advance(sp_front, dist2(mt) - 1);
        }
        //std::cout<< "Siguiente Paso: "<< *sp_front <<std::endl;
        setDir(*sp_front);


/*
        if(distanciai > 0){
            //no puedes ir arriba
            if(getI() != 6) {//podemos ir abajo
                int esquinaDer = (6 - getJ());
                if(esquinaDer >= 3){//buscamos la otra, estamos más cerca de la izq
                    setDir(DOWNRIGHT);
                }else{
                    setDir(DOWNLEFT);
                }

            }else{//decidimos arriesgar
                if(qbertj > getJ()){
                    //qbert más a la derecha
                    setDir(TOPLEFT);
                }else{
                    //qbert más a la izq
                    setDir(TOPRIGHT);
                }
            }

        }else if(distanciai < 0 && distanciaj => 0){
            //no puedes ir abajo derecha
            //TODO preferiblemente ir abajo izquierda.


        }else if(distanciaj < 0){
            //no puedes ir arriba izquierda


        }else{//distanciai < 0 && distanciaj < 0
            //no puedes ir abajo
            if(abs(distanciai) >= 3){
                //avanzamos aleatoriamente porque no hay mucho peligro
                IAnivel1(qberti, qbertj);
            }else{
                //huimos

            }

        }
*/
    }

    void randomMovement(int i, int j) override {
//        std::cout << IASlickSam << std::endl;
        if(IASlickSam == 0) {
            IAnivel1(i, j);
        }else if(IASlickSam == 1){
            IAnivel2(i, j);
        }else if(IASlickSam == 2){
            IAnivel3(i, j);
        }

        assignIJ();

        Enemy::setJumping(true);
        Enemy::setSourceX(getSourceX()-16);


    }

    void IAnivel1(int i, int j) {
        std::random_device rd;
        std::mt19937 mt(std::chrono::system_clock::now().time_since_epoch().count());
        std::uniform_int_distribution<int> dist4(1, 4);
        std::uniform_int_distribution<int> dist2(1, 2);
        std::uniform_int_distribution<int> dist3(1, 3);

        if (getI() == 6) {//estamos abajo
//            std::cout<<"estamos abajo"<<std::endl;
            if (getJ() == 0) {
                //esquina abajo a la izquierda
                setDir(TOPRIGHT);
            } else if (getJ() == getI()) {
                //esquina abajo a la derecha
                setDir(TOPLEFT);
            } else {  //evitamos caer abajo
                if (dist2(mt) == 1) {
                    setDir(TOPRIGHT);
                } else {
                    setDir(TOPLEFT);
                }
            }


        } else if (getI() == getJ()) {//estamos a la derecha
            if (getJ() == 0) {
                //esquina arriba
                if (dist2(mt) == 1) {
                    setDir(DOWNRIGHT);
                } else {
                    setDir(DOWNLEFT);
                }
            } else {//lateral derecho
                if (repeticion) {
                    setDir(DOWNLEFT);
                    repeticion = false;
                } else {
                    if (dist2(mt) == 1) {
                        setDir(DOWNRIGHT);
                    } else {
                        setDir(TOPLEFT);
                    }
                    repeticion = true;
                }
            }
        } else if (getJ() == 0) {//estamos a la izquierda
//            std::cout<<"estamos izquierda"<<std::endl;
            if (repeticion) {
                setDir(DOWNRIGHT);
                repeticion = false;
            } else {
                if (dist2(mt) == 1) {
                    setDir(TOPRIGHT);
                } else {
                    setDir(DOWNLEFT);
                }
                repeticion = true;
            }
        } else { //sin restriccion
//            std::cout<<"sin restriccion"<<std::endl;
            int a = dist4(mt);
            if (a == 1) {
                setDir(TOPLEFT);
            } else if (a == 2) {
                setDir(DOWNLEFT);
            } else if (a == 3) {
                setDir(DOWNRIGHT);
            } else {
                setDir(TOPRIGHT);
            }

        }
    }

    void drawBitmap() override {
            al_draw_bitmap_region(getDraw(), getSourceX() + (getDir() * 2 * sizePixelsX),
                                  getSourceY() + (sourceI * sizePixelsY), sizePixelsX, sizePixelsY*2,
                                  getX(), getY(), 0);
    }
};

#endif //ALLEGRO5TUTORIAL_SLICKSAM_HPP
