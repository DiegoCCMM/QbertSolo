//
// Created by diego on 29/4/21.
//

#ifndef ALLEGRO5TUTORIAL_UGGWRONGWAY_HPP
#define ALLEGRO5TUTORIAL_UGGWRONGWAY_HPP

class UggWrongWay : public Enemy{
    bool isUgg = true;  //ugg goes from right to left, wrong way, on the other way
public:
    UggWrongWay(const Piramide &piramide, const std::string &nom, int i, int j, int xRespectCube, int yRespectCube) : Enemy(
            piramide, nom, i, j, xRespectCube, yRespectCube) {
        if(nom == "Ugg"){
            isUgg = true;
        }else{
            isUgg = false;
        }
        i = 6;
        j = 6;

        ALLEGRO_SAMPLE *musica1 = al_load_sample(("./sounds/" + nom + "-1.ogg").c_str());
        ALLEGRO_SAMPLE *musica2 = al_load_sample(("./sounds/" + nom + "-2.ogg").c_str());

        playOnce(musica1);
        playOnce(musica2);
    }

    void randomMovement(int i, int j) override {

        std::random_device rd;
        std::mt19937 mt(std::chrono::system_clock::now().time_since_epoch().count());
        std::uniform_int_distribution<int> dist(1, 2);
        if(isUgg) {
            if (dist(mt) == 1) {
                setDir(TOPLEFT);
            } else {
                if(getI() != 6){//si no estamos en la ultima fila
                    setDir(DOWNLEFT);
                }else{
                    setDir(TOPLEFT);
                }

            }
        }else{//somos wrong way, vamos de izq a derecha
            if (dist(mt) == 1) {
                setDir(TOPRIGHT);
            } else {
                if(getI() != 6){//si no estamos en la ultima fila
                    setDir(DOWNRIGHT);
                }else{
                    setDir(TOPRIGHT);
                }

            }
        }
        assignIJ();

        setJumping(true);
        setSourceX(getSourceX()-16);
    }
    void assignIJ() override {
        if (getDir() == TOPLEFT) setI(getI() - 1), setJ(getJ() - 1);
        else if (getDir() == DOWNRIGHT) setJ(getJ() + 1);
        else if (getDir() == TOPRIGHT) setI(getI() - 1);
        else if (getDir() == DOWNLEFT) setJ(getJ() - 1);
    }
    void movement(Piramide *piramide, int WIDTH, int HEIGHT, int &puntuacion, Character *qbert, std::_List_iterator<Enemy *> it,
                  std::list<Enemy*> & borrarEnemies) override  {
        float movementXEspecial = 5.0;
        if (!Enemy::estaCielo()) { // Esta en el campo de juego
            if (isJumping()) {
                timerplusplus();
                if (!isFalling()) {
                    if (getTimer() < airTime / 2) {
                        //GO UP AND DIRECTION
                        if (getDir() == TOPRIGHT)
                            setX(movementX + getX());
                        else if( getDir() == DOWNRIGHT)
                            setX(movementXEspecial + getX());
                        else if(getDir() == TOPLEFT)
                            setX(getX() - movementX);
                        else
                            setX(getX() - movementXEspecial);
                        if (getDir() == TOPRIGHT || getDir() == TOPLEFT)
                            setY(getY() - movementY);
                    } else if (getTimer() > airTime / 2 && getTimer() < airTime) {
                        //GO DOWN AND DIRECTION
                        if (getDir() == TOPRIGHT)
                            setX(movementX + getX());
                        else if( getDir() == DOWNRIGHT)
                            setX(movementXEspecial + getX());
                        else if(getDir() == TOPLEFT)
                            setX(getX() - movementX);
                        else
                            setX(getX() - movementXEspecial);
                    } else if (getTimer() > airTime) {
                        if ((getJ() < 0 || getJ() > getI() || getI() > 6) ||
                            (nom == "Ugg" && (getJ() <= 0 || getJ() > getI() || getI() > 6)) ||
                            (nom == "WrongWay" && (getJ() < 0 || getJ() >= getI() || getI() > 6)) ){
                            setFalling(true);
                            if(nom == "coilyBola") {
                                puntuacion+=500;
                                ALLEGRO_SAMPLE *coilyCae = al_load_sample("./sounds/snake-fall.ogg");
                                playOnce(coilyCae);
                            }
                            if (getSourceX() != 0) {
                                //setSourceX(getSourceX() + 16);
                            }
                        } else {
                            //WE LANDED
                            playOnce(getJumpSound());
                            if (hasChangingGroundPower()) {
                                int none = 0;
                                piramide->changeCubeInverse(getI(), getJ());
                            }
                            setX(piramide->map[getI()][getJ()].x + getXRespectCube());
                            setY(piramide->map[getI()][getJ()].y + getYRespectCube());
                            setTimer(0);
                            setJumping(false);
                            setSourceX(getSourceX() + leftSprite);

                            if (getI() == qbert->getI() && getJ() == qbert->getJ()) {
                                setHaColisionado(true);
                            }
                        }
                    }
                } else {    //se cae de la piramide

                    if(nom == "Ugg" && getX() >= WIDTH/2-200){
                        setX(getX() - movementX);
                    }
                    else if(nom == "WrongWay" && getX() <= WIDTH/2+200){
                        setX(getX() + movementX);
                    }
                    else if(nom != "WrongWay" && nom != "Ugg" && getY() <= HEIGHT){
                        setY(getY() + movementY);
                    }
                    else{//si hemos llegado al suelo
                        borrarEnemies.push_back(it.operator*());//lo borramos
                    }
                }
            }
        } else { // Esta bajando del cielo, CIELO
            timerplusplus();
            setX(piramide->map[getI()][getJ()].x + getXRespectCube());
            if (getY() < piramide->map[getI()][getJ()].y + getYRespectCube()) {
                setY(getY() + movementY);
            } else {
                setEstado(INGAME);
                if (getI() == qbert->getI() && getJ() == qbert->getJ()) {
                    setHaColisionado(true);
                }
                playOnce(getJumpSound());
                setX(piramide->map[getI()][getJ()].x + getXRespectCube());
                setY(piramide->map[getI()][getJ()].y + getYRespectCube());
                setTimer(0);
            }
        }
    };
    void drawBitmap() override {
        al_draw_bitmap_region(getDraw(), getSourceX() + (getDir() * 2 * sizePixelsX),
                              getSourceY() + (sourceI * sizePixelsY), sizePixelsX, sizePixelsY*2,
                              getX(), getY(), 0);
    }
};

#endif //ALLEGRO5TUTORIAL_UGGWRONGWAY_HPP
