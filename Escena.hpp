//
// Created by patri on 14/04/2021.
//
#include "Piramide.hpp"
#include "Objeto.hpp"
#include "SlickSam.hpp"
#include "UggWrongWay.hpp"

#define limEnemigos 6

#include <random>

#ifndef QBERT_ESCENA_HPP
#define QBERT_ESCENA_HPP

#define periodoEnemigos 120

class Escena {
    float width, height;

    int maxEnemigos = 0;

    Objeto playerObj, numPlayerObj,
            puntuacionObj,
            changetoObj,
            flechaObj, cuadrObj,
            vidaObj, numvidaObj,
            levelRoundObj, numLevelObj, numRoundObj,
            gameoverObj;

public:

    Piramide piramide;
    QBert qbert;
    std::list<Enemy *> enemies;
    std::list<Platillo> platillos;
    int level, round, periodEnemies = 0;
    int puntuacion;
    bool hasCoily = false, gameover = false, hasSlickSam = false;

    /* Constructor */
    Escena(float _width, float _height) : width(_width), height(_height), level(1), round(1) {
        playerObj.setDraw(al_load_bitmap("../sprites/player-letras.png"));
        playerObj.setSizePixelsX(16 * 3 + 4);
        playerObj.setSizePixelsY(8);
        playerObj.setXRespectCube(-3 * 32);
        playerObj.setYRespectCube(-32 - 10);

        numPlayerObj.setDraw(al_load_bitmap("../sprites/player-num.png"));
        numPlayerObj.setSizePixelsX(9);
        numPlayerObj.setSizePixelsY(12);
        numPlayerObj.setXRespectCube(-32 - 3);
        numPlayerObj.setYRespectCube(-32 - 13);

        puntuacionObj.setDraw(al_load_bitmap("../sprites/fonts.png"));
        puntuacionObj.setSizePixelsX(8);
        puntuacionObj.setSizePixelsY(8);
        puntuacionObj.setXRespectCube(-3 * 32);
        puntuacionObj.setYRespectCube(-32 - 1);

        changetoObj.setDraw(al_load_bitmap("../sprites/changeto-text.png"));
        changetoObj.setSizePixelsX(7 * 8);
        changetoObj.setSizePixelsY(8);
        changetoObj.setXRespectCube(-3 * 32);
        changetoObj.setYRespectCube(-4);

        flechaObj.setDraw(al_load_bitmap("../sprites/flechas.png"));
        flechaObj.setSizePixelsX(9 * 8);
        flechaObj.setSizePixelsY(8);
        flechaObj.setXRespectCube(-3 * 32);
        flechaObj.setYRespectCube(6);

        cuadrObj.setDraw(al_load_bitmap("../sprites/cubos.png"));
        cuadrObj.setSizePixelsX(16);
        cuadrObj.setSizePixelsY(16);
        cuadrObj.setXRespectCube(-3 * 32 + 17);
        cuadrObj.setYRespectCube(2);

        vidaObj.setDraw(al_load_bitmap("../sprites/qbert-vida.png"));
        vidaObj.setSizePixelsX(16);
        vidaObj.setSizePixelsY(16);
        vidaObj.setXRespectCube(-3 * 32);
        vidaObj.setYRespectCube(32);

        numvidaObj.setDraw(al_load_bitmap("../sprites/fonts.png"));
        numvidaObj.setSourceI(2);
        numvidaObj.setSizePixelsX(8);
        numvidaObj.setSizePixelsY(8);
        numvidaObj.setXRespectCube(-3 * 32 + 16);
        numvidaObj.setYRespectCube(33);

        levelRoundObj.setDraw(al_load_bitmap("../sprites/level-round-text.png"));
        levelRoundObj.setSizePixelsX(40);
        levelRoundObj.setSizePixelsY(16);
        levelRoundObj.setXRespectCube(2 * 32);
        levelRoundObj.setYRespectCube(-4);

        numLevelObj.setDraw(al_load_bitmap("../sprites/fonts.png"));
        numLevelObj.setSizePixelsX(8);
        numLevelObj.setSizePixelsY(8);
        numLevelObj.setXRespectCube(2 * 32 + 40);
        numLevelObj.setYRespectCube(-4);

        numRoundObj.setDraw(al_load_bitmap("../sprites/fonts.png"));
        numRoundObj.setSizePixelsX(8);
        numRoundObj.setSizePixelsY(8);
        numRoundObj.setXRespectCube(2 * 32 + 40);
        numRoundObj.setYRespectCube(4);

        gameoverObj.setDraw(al_load_bitmap("../sprites/gameover.png"));
        gameoverObj.setSizePixelsX(8);
        gameoverObj.setSizePixelsY(8);
        gameoverObj.setXRespectCube(-16);
        gameoverObj.setYRespectCube(3 * 32);

    }

    void load(float _width, float _height) {
        width = _width, height = _height;
        if (level == 1 && round == 1) puntuacion = 0;
        gameover = false;

        // Cargar mapa
        piramide.loadMap(level, round, width, height);

        // Cargar personajes
        qbert = QBert(piramide);

        enemies.clear();
        hasCoily = false;
        platillos.clear();

        // Establecer info del juego
        playerObj.resize(&piramide);
        numPlayerObj.resize(&piramide);
        puntuacionObj.resize(&piramide);
        changetoObj.resize(&piramide);
        flechaObj.resize(&piramide);

        cuadrObj.resize(&piramide);
        cuadrObj.setSourceJ(piramide.getSx(2) / 16 + 4);
        cuadrObj.setSourceI(piramide.getSy(2) / 16);

        vidaObj.resize(&piramide);
        numvidaObj.resize(&piramide);
        levelRoundObj.resize(&piramide);
        numLevelObj.resize(&piramide);
        numRoundObj.resize(&piramide);
        gameoverObj.resize(&piramide);

        cargarPlatillos();
    }

    void cargarPlatillos() {
        std::string path;
        if (level < 6)
            path = "../levels/platillos/level" + std::to_string(level) + "_" + std::to_string(round) + ".txt";
        else path = "../levels/platillos/restoLevels.txt";

        std::ifstream file(path);
        int pos, fila;
        while (!file.eof()) {
            file >> pos >> fila;
            Platillo plato = Platillo(piramide, fila, static_cast<Posicion>(pos));
            platillos.push_back(plato);
        }
        file.close();
    }

    bool piramideCompleta() {
        if (piramide.isPiramideCompleta()) {
            int time = piramide.getTime();
            if (time == 0) {
                al_play_sample(piramide.getFinishSound(), 1.0, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
            }

            if (time > 170) {
                calculatePoints();
                return true;
            }

            if (time % 30 < 10) {
                piramide.setAllColor(0);
            } else if (time % 30 >= 10 && time % 30 < 20) {
                piramide.setAllColor(1);
            } else if (time % 30 >= 20 && time % 30 < 30) {
                piramide.setAllColor(2);
            }

            piramide.setTime(time + 2);

        }

        return false;
    }

    void movementAll() {
        int p = 0;
        if (!qbert.isColision() && !piramide.isPiramideCompleta()) {
            qbert.movement(&piramide, height, platillos, enemies, hasCoily, hasSlickSam, p);
            if (!qbert.isFalling()) {
                if (!qbert.hasSuperpower()) {
                    int i = -2, j = -2;
                    //mover a todos los enemigos
                    std::list<Enemy *> borrarEnemigos;
                    for (std::_List_iterator<Enemy *> it = enemies.begin(); it != enemies.end(); it++) {
                        if (!it.operator*()->estaCielo()) {
                            i = it.operator*()->getI();
                            j = it.operator*()->getJ();
                            checkRandMovementEnemies(it);
                            it.operator*()->movement(&piramide, width, height, &qbert, it, borrarEnemigos);
                            if (it.operator*()->haColisionado()) {
                                if (it.operator*()->hasHelpingPower()) { // Blob verde
                                    qbert.setSuperpower(true);
                                    puntuacion += 100;
                                    enemies.erase(it);
                                    break;
                                } else if (it.operator*()->hasChangingGroundPower()) { // Slick y Sam
                                    hasSlickSam = false;
                                    puntuacion += 300;
                                    enemies.erase(it);
                                    break;
                                } else {
                                    //enemigo mata a qbert
                                    if (!(qbert.getJ() < 0 || qbert.getJ() > qbert.getI() || qbert.getI() > 6)) {
                                        qbert.setColision(true);
                                        qbert.setLives(qbert.getLives() - 1);
                                        qbert.animacionMuerte(&piramide);
                                        hasCoily = qbert.reset(&piramide, p, enemies, qbert.getI(), qbert.getJ(),
                                                               qbert.getDir());
                                        hasSlickSam = false;
                                        puntuacion += p;
                                        break;
                                    }
                                }
                            }
                        } else { // El enemigo esta bajando del cielo
                            it.operator*()->movement(&piramide, width, height, &qbert, it, borrarEnemigos);
                        }
                    }
                    for (const auto &item : borrarEnemigos){
                        enemies.remove(item);
                    }borrarEnemigos.clear();
                }

                for (std::_List_iterator<Platillo> it = platillos.begin(); it != platillos.end(); it++) {
                    it->movement();
                }

                playerObj.movement();
                if (playerObj.getTimer() % 2 == 0) {
                    playerObj.setSourceI((playerObj.getSourceI() + 1) % 6);
                }

                flechaObj.movement();
            }

            puntuacion += p; // Actualizamos la puntuacion
        }
    }

    void drawAll() {

        if (qbert.isFalling()) { // Si QBert esta cayendo primero se dibuja a QBert y luego la piramide
            qbert.drawBitmap();
            piramide.drawBitmap();

            // Recordar que mientras cae el resto de elementos dejan de moverse
            for (std::_List_iterator<Platillo> it = platillos.begin(); it != platillos.end(); it++) {
                it->drawBitmap();
            }
        } else {
            if (qbert.getLives() != 0) {
                for (std::_List_iterator<Enemy *> it = enemies.begin(); it != enemies.end(); it++) {
                    if(it.operator*()->isFalling() && it.operator*()->getI()<6){
                        it.operator*()->drawBitmap();
                    }
                }
                // Dibujado de las cosas durante el juego normal
                piramide.drawBitmap();
                if (qbert.isColision()) { //qbert se enfada
                    qbert.drawBocadillo();
                    if (qbert.getTimerColision() > 50) {
                        qbert.setColision(false);
                        qbert.setTimerColision(0);
                    } else {
                        qbert.setTimerColision(qbert.getTimerColision() + 1);
                    }
                }
                for (std::_List_iterator<Platillo> it = platillos.begin(); it != platillos.end(); it++) {
                    it->drawBitmap();
                }
                qbert.drawBitmap();
                for (std::_List_iterator<Enemy *> it = enemies.begin(); it != enemies.end(); it++) {
                    if(!it.operator*()->isFalling() || it.operator*()->getI()>=6){
                        it.operator*()->drawBitmap();
                    }
                }
            } else {
                // QBERT tiene 0 vidas
                piramide.drawBitmap();
                for (std::_List_iterator<Platillo> it = platillos.begin(); it != platillos.end(); it++) {
                    it->drawBitmap();
                }

                std::string frase = "GAME OVER";
                float aux_x = gameoverObj.getX();
                for (std::string::size_type i = 0; i < frase.size(); i++) {
                    if (int(frase[i]) != 32) {
                        std::cout << int(frase[i]) % 65 << std::endl;
                        gameoverObj.setSourceJ(int(frase[i]) % 65);
                        gameoverObj.setSourceI(7);
                    } else {
                        gameoverObj.setSourceJ(0);
                        gameoverObj.setSourceI(6);
                    }
                    gameoverObj.drawBitmap();
                    gameoverObj.setX(gameoverObj.getX() + 8);
                }
                gameoverObj.setX(aux_x);

                if (gameoverObj.getTimer() > 100) gameover = true;
                gameoverObj.setTimer(gameoverObj.getTimer() + 1);
            }
        }

        playerObj.drawBitmap();
        numPlayerObj.drawBitmap();

        std::string numero = std::to_string(puntuacion);
        float aux_x = puntuacionObj.getX();
        for (std::string::size_type i = 0; i < numero.size(); i++) {
            puntuacionObj.setSourceJ((int(numero[i]) % 48));
            puntuacionObj.drawBitmap();
            puntuacionObj.setX(puntuacionObj.getX() + 8);
        }
        puntuacionObj.setX(aux_x);

        changetoObj.drawBitmap();

        if (flechaObj.getTimer() > 20) {
            flechaObj.setSourceI(1);
            flechaObj.drawBitmap();

            if (flechaObj.getTimer() > 40) {
                flechaObj.setSourceI(0);
                flechaObj.drawBitmap();

                if (flechaObj.getTimer() > 60) flechaObj.setTimer(0);
            }
        }

        cuadrObj.drawBitmap();
        vidaObj.drawBitmap();

        numero = std::to_string(qbert.getLives());
        aux_x = numvidaObj.getX();
        for (std::string::size_type i = 0; i < numero.size(); i++) {
            numvidaObj.setSourceJ((int(numero[i]) % 48));
            numvidaObj.drawBitmap();
            numvidaObj.setX(numvidaObj.getX() + 8);
        }
        numvidaObj.setX(aux_x);

        levelRoundObj.drawBitmap();

        numLevelObj.resize(&piramide);
        numero = std::to_string(level);
        aux_x = numLevelObj.getX();
        for (std::string::size_type i = 0; i < numero.size(); i++) {
            numLevelObj.setSourceJ((int(numero[i]) % 48));
            numLevelObj.drawBitmap();
            numLevelObj.setX(numLevelObj.getX() + 8);
        }
        numLevelObj.setX(aux_x);

        numRoundObj.resize(&piramide);
        numero = std::to_string(round);
        aux_x = numRoundObj.getX();
        for (std::string::size_type i = 0; i < numero.size(); i++) {
            numRoundObj.setSourceJ((int(numero[i]) % 48));
            numRoundObj.drawBitmap();
            numRoundObj.setX(numRoundObj.getX() + 8);
        }
        numRoundObj.setX(aux_x);

    }

    void generarEnemigos() {
        if (!qbert.isColision() && !qbert.isEnPlatillo() && !qbert.hasSuperpower()) {
            if (periodEnemies >= periodoEnemigos && enemies.size() < limEnemigos) {
                //genera un enemigo aleatorio
                std::random_device rd;
                std::mt19937 mt(std::chrono::system_clock::now().time_since_epoch().count());
                std::uniform_int_distribution<int> dist(0, 60);
                int eleccion = dist(mt);
                if (eleccion >= 0 && eleccion <= 14) {
                    //redblob o poder
                    if(eleccion <= 12) {
                        Enemy *redblob = new Enemy(piramide, "Redblob", 1, eleccion%2, 9,
                                                   0); // X e Y (pixeles) posicion respecto al cubo[i,j]
                        enemies.push_back(redblob);
                    }else {
                        Enemy *green = new Enemy(piramide, "GreenBlob", 1, eleccion%2, 9,
                                                 -5); // X e Y (pixeles) posicion respecto al cubo[i,j]
                        enemies.push_back(green);
                    }

                } else if (eleccion >= 15 && eleccion <= 29) {
                    //coily
                    if (!hasCoily) {
                        std::cout << "meto coily" << std::endl;
                        Coily *coily = new Coily(piramide, "coilyBola", 1, eleccion%2, 9, -3);
                        enemies.push_back(coily);
                        hasCoily = true;
                        std::cout << "meto redblobl" << std::endl;
                    }
                } else if (eleccion >= 30 && eleccion <= 44) {
                    //ugg o wrong way
                    if (eleccion <= 37) {
                        UggWrongWay *ugg = new UggWrongWay(piramide, "Ugg", 6, 6, 9, -6);
                        enemies.push_back(ugg);
                    } else {
                        UggWrongWay *WrongWay = new UggWrongWay(piramide, "WrongWay", 6, 0, 9, -6);
                        enemies.push_back(WrongWay);
                    }
                } else if (eleccion >= 45 && eleccion <= 60) {
                    //slick o sam
                    if (!hasSlickSam) {
                        SlickSam *slickObj = new SlickSam(piramide, "Slick", 1, eleccion%2, 9, -6);
                        enemies.push_back(slickObj);
                        hasSlickSam = true;
                    }
                    // TODO: Sam
                }
                //reinicio periodEnemies
                periodEnemies = 0;
            } else {
                periodEnemies++;
            }
        }
    }

    void calculatePoints() {
        // Platillos no usados
        puntuacion += platillos.size() * 50;

        // Segun la ronda que se haya completado
        puntuacion += 1000 + 250 * (round % 4 - 1);

    }

    void resizAll(float _width, float _height) {
        width = _width, height = _height;
        piramide.resizeMap(width, height);

        for (std::_List_iterator<Enemy *> it = enemies.begin(); it != enemies.end(); it++) {
            it.operator*()->resize(&piramide);
        }

        for (std::_List_iterator<Platillo> it = platillos.begin(); it != platillos.end(); it++) {
            it->resize(&piramide);
        }

        qbert.resize(&piramide);

        playerObj.resize(&piramide);
        numPlayerObj.resize(&piramide);
        puntuacionObj.resize(&piramide);
        changetoObj.resize(&piramide);
        flechaObj.resize(&piramide);
        cuadrObj.resize(&piramide);
        vidaObj.resize(&piramide);
        numvidaObj.resize(&piramide);
        levelRoundObj.resize(&piramide);
        numLevelObj.resize(&piramide);
        numRoundObj.resize(&piramide);
        gameoverObj.resize(&piramide);
    }

    void destroyAll() {
        piramide.destroy();
        qbert.destroy();

        for (std::_List_iterator<Enemy *> it = enemies.begin(); it != enemies.end(); it++) {
            it.operator*()->destroy();
        }

        for (std::_List_iterator<Platillo> it = platillos.begin(); it != platillos.end(); it++) {
            it->destroy();
        }

        playerObj.destroy();
        numPlayerObj.destroy();
    }

    void checkRandMovementEnemies(std::_List_iterator<Enemy *> it) {
        //for (std::_List_iterator<Enemy*> it = enemies.begin(); it != enemies.end(); it++){
        if (it.operator*()->getRandMoveTimer() == it.operator*()->getRandMovePeriod()) {
            std::cout << "muevo enemi" << std::endl;
            qbert.getI();
            std::cout << "muevo enemiI" << std::endl;
            qbert.getJ();
            std::cout << "muevo enemiJ" << std::endl;
            std::cout << it.operator*()->isCoily << std::endl;
            std::cout << "lolazo" << std::endl;

            it.operator*()->randomMovement(qbert.getI(), qbert.getJ());
            it.operator*()->resetRandomMoveTimer();
            std::cout << "acabo de mover enemi" << std::endl;
        } else {
            it.operator*()->randomMoveTimerplusplus();
        }
        //}
    }

    void setMoveQBert(Direction dir) {
        if (!piramide.isPiramideCompleta() && qbert.getLives() != 0) {
            qbert.setMove(dir);
        }
    }

    /*************************
     * GETTER'S AND SETTER'S *
     *************************/

    int getLevel() const { return level; }

    void setLevel(int _level) { Escena::level = _level; }

    int getRound() const { return round; }

    void setRound(int _round) { Escena::round = _round; }

    bool isGameover() { return gameover; }

    bool itHasCoily() const { return hasCoily; }

    void setHasCoily(bool hasCoily) { Escena::hasCoily = hasCoily; }

    const Objeto &getPuntuacionObj() const { return puntuacionObj; }

    void setPuntuacionObj(const Objeto &puntuacionObj) { Escena::puntuacionObj = puntuacionObj; }

    int getPuntuacion() const { return puntuacion; }

    void setPuntuacion(int puntuacion) { Escena::puntuacion = puntuacion; }

};

#endif //QBERT_ESCENA_HPP
