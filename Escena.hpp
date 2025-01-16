//
// Created by patri on 14/04/2021.
//
#include "Piramide.hpp"
#include "Objeto.hpp"
#include "SlickSam.hpp"
#include "UggWrongWay.hpp"
#include <random>

#ifndef QBERT_ESCENA_HPP
#define QBERT_ESCENA_HPP

#define limEnemigos 6
#define periodoEnemigos 120

class Escena {
    float width, height;

    bool enemigosPosibles[5];
    int puntosVidasAdicionales;
    int colorSuperPower;
    int timerGlobal;

    Objeto playerObj, numPlayerObj,
            puntuacionObj,
            changetoObj,
            flechaObj, cuadrObj,
            vidaObj, numvidaObj,
            levelRoundObj, numLevelObj, numRoundObj,
            gameoverObj,
            pauseObj;

    ALLEGRO_BITMAP *fonts = al_load_bitmap("./sprites/fonts.png");

public:

    Piramide piramide;
    QBert qbert;
    std::list<Enemy *> enemies;
    std::list<Platillo> platillos;
    int level, round, periodEnemies = 0;
    int puntuacion;
    bool hasCoily = false, gameover = false, hasSlickSam = false;
    int posPause = 0;
    int cuboTiempo, IACoily, IASlickSam, IABlob;

    /* Constructor */
    Escena(float _width, float _height) : width(_width), height(_height), level(1), round(1) {
        pauseObj.setDraw(al_load_bitmap("./sprites/escape.png"));
        pauseObj.setSizePixelsX(230);
        pauseObj.setSizePixelsY(60);
        pauseObj.setXRespectCube(-94);
        pauseObj.setYRespectCube(20);

        playerObj.setDraw(al_load_bitmap("./sprites/player-letras.png"));
        playerObj.setSizePixelsX(16 * 3 + 4);
        playerObj.setSizePixelsY(8);
        playerObj.setXRespectCube(-3 * 32);
        playerObj.setYRespectCube(-32 - 10);

        numPlayerObj.setDraw(al_load_bitmap("./sprites/player-num.png"));
        numPlayerObj.setSizePixelsX(9);
        numPlayerObj.setSizePixelsY(12);
        numPlayerObj.setXRespectCube(-32 - 3);
        numPlayerObj.setYRespectCube(-32 - 13);

        puntuacionObj.setDraw(al_load_bitmap("./sprites/fonts.png"));
        puntuacionObj.setSizePixelsX(8);
        puntuacionObj.setSizePixelsY(8);
        puntuacionObj.setXRespectCube(-3 * 32);
        puntuacionObj.setYRespectCube(-32 - 1);

        changetoObj.setDraw(al_load_bitmap("./sprites/changeto-text.png"));
        changetoObj.setSizePixelsX(7 * 8);
        changetoObj.setSizePixelsY(8);
        changetoObj.setXRespectCube(-3 * 32);
        changetoObj.setYRespectCube(-4);

        flechaObj.setDraw(al_load_bitmap("./sprites/flechas.png"));
        flechaObj.setSizePixelsX(9 * 8);
        flechaObj.setSizePixelsY(8);
        flechaObj.setXRespectCube(-3 * 32);
        flechaObj.setYRespectCube(6);

        cuadrObj.setDraw(al_load_bitmap("./sprites/cubos.png"));
        cuadrObj.setSizePixelsX(16);
        cuadrObj.setSizePixelsY(16);
        cuadrObj.setXRespectCube(-3 * 32 + 17);
        cuadrObj.setYRespectCube(2);

        vidaObj.setDraw(al_load_bitmap("./sprites/qbert-vida.png"));
        vidaObj.setSizePixelsX(16);
        vidaObj.setSizePixelsY(16);
        vidaObj.setXRespectCube(-3 * 32);
        vidaObj.setYRespectCube(32);

        numvidaObj.setDraw(al_load_bitmap("./sprites/fonts.png"));
        numvidaObj.setSourceI(2);
        numvidaObj.setSizePixelsX(8);
        numvidaObj.setSizePixelsY(8);
        numvidaObj.setXRespectCube(-3 * 32 + 16);
        numvidaObj.setYRespectCube(33);

        levelRoundObj.setDraw(al_load_bitmap("./sprites/level-round-text.png"));
        levelRoundObj.setSizePixelsX(40);
        levelRoundObj.setSizePixelsY(16);
        levelRoundObj.setXRespectCube(2 * 32);
        levelRoundObj.setYRespectCube(-4);

        numLevelObj.setDraw(al_load_bitmap("./sprites/fonts.png"));
        numLevelObj.setSizePixelsX(8);
        numLevelObj.setSizePixelsY(8);
        numLevelObj.setXRespectCube(2 * 32 + 40);
        numLevelObj.setYRespectCube(-4);

        numRoundObj.setDraw(al_load_bitmap("./sprites/fonts.png"));
        numRoundObj.setSizePixelsX(8);
        numRoundObj.setSizePixelsY(8);
        numRoundObj.setXRespectCube(2 * 32 + 40);
        numRoundObj.setYRespectCube(4);

        gameoverObj.setDraw(al_load_bitmap("./sprites/gameover.png"));
        gameoverObj.setSizePixelsX(8);
        gameoverObj.setSizePixelsY(8);
        gameoverObj.setXRespectCube(-16);
        gameoverObj.setYRespectCube(3 * 32);

    }

    void load(float _width, float _height, bool backdoor, int _cuboTiempo, int _IACoily, int _IASlickSam, int _IABlob) {
        width = _width, height = _height;
        gameover = false;
        colorSuperPower = 0;
        timerGlobal = 0;

        cuboTiempo = _cuboTiempo;
        IACoily = _IACoily;
        IASlickSam = _IASlickSam;
        IABlob = _IABlob;

        // Cargar mapa
        piramide.loadMap(level, round, width, height);

        // Cargar personajes
        if(level == 1 && round == 1 || backdoor){
            qbert = QBert(piramide);
            puntuacion = 0;
            puntosVidasAdicionales = 0;
        } else {
            qbert = QBert(piramide, qbert.getLives());
        }

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
        pauseObj.resize(&piramide);

        cargarPlatillos();
        cargarEnemigos();
    }

    void cargarPlatillos() {
        std::string path;
        if (level < 6)
            path = "./levels/platillos/level" + std::to_string(level) + "_" + std::to_string(round) + ".txt";
        else path = "./levels/platillos/restoLevels.txt";

        std::ifstream file(path);
        int pos, fila;
        while (!file.eof()) {
            file >> pos >> fila;
            Platillo plato = Platillo(piramide, fila, static_cast<Posicion>(pos));
            platillos.push_back(plato);
        }
        file.close();
    }

    void cargarEnemigos(){
        std::string path;
        if (level < 6)
            path = "./levels/enemigos/level" + std::to_string(level) + "_" + std::to_string(round) + ".txt";
        else path = "./levels/enemigos/restoLevels.txt";

        // Formato de los ficheros:
        // blob verde, blob rojo, coily, wrong way&ugg, sam&slick

        std::ifstream file(path);
        file >> enemigosPosibles[0] >> enemigosPosibles[1] >> enemigosPosibles[2] >> enemigosPosibles[3] >> enemigosPosibles[4];
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
                    //mover a todos los enemigos
                    std::list<Enemy *> borrarEnemigos;
                    for (std::_List_iterator<Enemy *> it = enemies.begin(); it != enemies.end(); it++) {
                        if (!it.operator*()->estaCielo()) {
                            checkRandMovementEnemies(it);
                            it.operator*()->movement(&piramide, width, height, p, &qbert, it, borrarEnemigos);
                            if (it.operator*()->haColisionado()) {
                                if (it.operator*()->hasHelpingPower()) { // Blob verde
                                    qbert.setSuperpower(true);
                                    al_play_sample(qbert.getSuperPowerSound(), 1.0, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
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
                                        hasCoily = qbert.reset(&piramide, p, enemies, true, qbert.getI(), qbert.getJ(),
                                                               qbert.getDir());
                                        hasSlickSam = false;
                                        break;
                                    }
                                }
                            }
                        } else { // El enemigo esta bajando del cielo
                            it.operator*()->movement(&piramide, width, height, p, &qbert, it, borrarEnemigos);
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
        }
        puntuacion += p; // Actualizamos la puntuacion
        checkPointsLives();
    }

    void drawAll() {
        std::string frase;
        float x, y;

        x = width / 2 + 8*4, y = height/2-140+20;
        frase = "ESC TO PAUSE";                                                                                                                                                                             "EMPEZAR EN NIVEL     -" + std::to_string(level) + "-\\";
        for (std::string::size_type i = 0; i < frase.size(); i++) {
            if (int(frase[i]) != 32) { // En caso de ser un espacio no se dibuja
                al_draw_bitmap_region(fonts, (int(frase[i]) % 65) * 8, 8*7,
                                      8, 8, x, y, 0);
            }
            x += 8;
        }

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
                if(qbert.hasSuperpower()){
                    // Tengo el superpower
                    if(colorSuperPower < 15){
                        al_clear_to_color(al_map_rgb(157, 115, 0));
                        colorSuperPower++;
                    } else if(colorSuperPower < 30) {
                        al_clear_to_color(al_map_rgb(208, 41, 238));
                        colorSuperPower++;
                    } else if(colorSuperPower < 45) {
                        al_clear_to_color(al_map_rgb(98, 41, 238));
                        colorSuperPower++;
                    } else{
                        al_clear_to_color(al_map_rgb(98, 41, 238));
                        colorSuperPower = 0;
                    }
                }

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
                        //std::cout << int(frase[i]) % 65 << std::endl;
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

                if (gameoverObj.getTimer() > 100) {
                    gameover = true;
                    gameoverObj.setTimer(0);
                }
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

    void drawPause(ALLEGRO_BITMAP *pauseBitmap, ALLEGRO_BITMAP *fonts, int posPause) {
        al_draw_bitmap_region(pauseBitmap, 0, 0, 300, 200, width / 2 - 113, height / 2-50, 0);
        float x, y;
        std::string frase;

        frase = "WHAT DO YOU WANT TO DO?\\";
        x = width / 2 - frase.size()*8/2, y = height / 2 - 8 * 5;
        for (std::string::size_type i = 0; i < frase.size(); i++) {
            if (int(frase[i]) == 92) { // '\' --> \n
                x = width / 2 - 8 * 12 - 8;
                y += 9 * 3;
            } else if (int(frase[i]) == 63) { // '?'
                al_draw_bitmap_region(fonts, 7 * 8 + 9 * 9, 8 * 4,
                                      7, 8, x, y, 0);
            } else if (int(frase[i]) != 32) { // En caso de ser un espacio no se dibuja
                al_draw_bitmap_region(fonts, (int(frase[i]) % 65) * 8, 8,
                                      8, 8, x, y, 0);
            }
            x += 8;
        }

        x = width / 2 - 8 * 6;
        int naranja = 0;
        frase = "CONTINUE\\GO TO MENU\\CLOSE GAME";
        for (std::string::size_type i = 0; i < frase.size(); i++) {
            if (int(frase[i]) == 92) { // '\' --> \n
                x = width / 2 - 8 * 6-8;
                y += 9*2;
                naranja++;
            } else if (int(frase[i]) >= 48 && int(frase[i]) <= 57) { // Numero
                if (posPause == naranja) {
                    al_draw_bitmap_region(fonts, (int(frase[i]) % 48) * 8 + 9 * 9, 8 * 2,
                                          8, 8, x, y, 0);
                } else {
                    al_draw_bitmap_region(fonts, (int(frase[i]) % 48) * 8 + 9 * 9, 0,
                                          8, 8, x, y, 0);
                }
            } else if (int(frase[i]) != 32) { // En caso de ser un espacio no se dibuja
                if (posPause == naranja) {
                    al_draw_bitmap_region(fonts, (int(frase[i]) % 65) * 8, 8 * 3,
                                          8, 8, x, y, 0);
                } else {
                    al_draw_bitmap_region(fonts, (int(frase[i]) % 65) * 8, 8,
                                          8, 8, x, y, 0);
                }
            }
            x += 8;
        }
    }

    void generarEnemigos() {
        if (!qbert.isColision() && !qbert.isEnPlatillo() && !qbert.hasSuperpower() && !piramide.isPiramideCompleta()) {
            if (periodEnemies >= periodoEnemigos && enemies.size() < limEnemigos) {
                //genera un enemigo aleatorio
                std::random_device rd;
                std::mt19937 mt(std::chrono::system_clock::now().time_since_epoch().count());
                std::uniform_int_distribution<int> dist(0, 60);
                int eleccion = dist(mt);

                if (eleccion >= 0 && eleccion <= 14) {
                    //redblob o poder
                    if(eleccion <= 12 && enemigosPosibles[1]) {
                        Enemy *red = new Enemy(piramide, "Redblob", 1, eleccion%2, 10,
                                                 0, IABlob); // X e Y (pixeles) posicion respecto al cubo[i,j]
                        enemies.push_back(red);
                    }else if(eleccion > 12 && enemigosPosibles[0]) {
                        Enemy *green = new Enemy(piramide, "GreenBlob", 1, eleccion%2, 9,
                                                 -5); // X e Y (pixeles) posicion respecto al cubo[i,j]
                        enemies.push_back(green);
                    }
                    periodEnemies = 0;
                } else if (eleccion >= 15 && eleccion <= 29 && enemigosPosibles[2]) {
                    //coily
                    if (!hasCoily) {
                        //std::cout << "meto coily" << std::endl;
                        Coily *coily = new Coily(piramide, "coilyBola", 1, eleccion%2, 9, -3, IACoily, platillos);
                        enemies.push_back(coily);
                        hasCoily = true;
//                        std::cout << "meto redblob" << std::endl;
                    }
                    periodEnemies = 0;
                } else if (eleccion >= 30 && eleccion <= 44 && enemigosPosibles[3]) {
                    //ugg o wrong way
                    if (eleccion <= 37) {
                        UggWrongWay *ugg = new UggWrongWay(piramide, "Ugg", 6, 6, 9, -6);
                        enemies.push_back(ugg);
                    } else {
                        UggWrongWay *WrongWay = new UggWrongWay(piramide, "WrongWay", 6, 0, 9, -6);
                        enemies.push_back(WrongWay);
                    }
                    periodEnemies = 0;
                } else if (eleccion >= 45 && eleccion <= 60 && enemigosPosibles[4]) {
                    //slick o sam
                    if (!hasSlickSam) {
                        if(eleccion % 2 == 0) {
                            SlickSam *slickObj = new SlickSam(piramide, "Slick", 1, eleccion % 2, 9, -6, IASlickSam);
                            enemies.push_back(slickObj);
                            hasSlickSam = true;
                        }else{
                            SlickSam *sam = new SlickSam(piramide, "Sam", 1, eleccion % 2, 9, -6, IASlickSam);
                            enemies.push_back(sam);
                            hasSlickSam = true;
                        }
                        periodEnemies = 0;
                    }
                }


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

        checkPointsLives();

    }

    void checkPointsLives(){
        if(puntuacion >= 8000 && puntosVidasAdicionales == 0){
            qbert.setLives(qbert.getLives()+1);
            puntosVidasAdicionales++;
        }

        if(puntosVidasAdicionales!=0 && (puntuacion-8000)/(14000 * puntosVidasAdicionales) > 0){
            puntosVidasAdicionales++;
            qbert.setLives(qbert.getLives()+1);
        }
    }

    void resizAll(float _width, float _height) {
        width = _width, height = _height;
        piramide.resizeMap(width, height);
        qbert.resize(&piramide);

        for (std::_List_iterator<Enemy *> it = enemies.begin(); it != enemies.end(); it++) {
            it.operator*()->resize(&piramide);
        }

        for (std::_List_iterator<Platillo> it = platillos.begin(); it != platillos.end(); it++) {
            it->resize(&piramide);
        }

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
        pauseObj.resize(&piramide);
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
        puntuacionObj.destroy();
        changetoObj.destroy();
        flechaObj.destroy();
        cuadrObj.destroy();
        vidaObj.destroy();
        numvidaObj.destroy();
        levelRoundObj.destroy();
        numLevelObj.destroy();
        numRoundObj.destroy();
        gameoverObj.destroy();
        pauseObj.destroy();

        al_destroy_bitmap(fonts);
    }

    void checkRandMovementEnemies(std::_List_iterator<Enemy *> it) {
        //for (std::_List_iterator<Enemy*> it = enemies.begin(); it != enemies.end(); it++){
        if (it.operator*()->getRandMoveTimer() == it.operator*()->getRandMovePeriod()) {
//            std::cout << "muevo enemi" << std::endl;
            qbert.getI();
//            std::cout << "muevo enemiI" << std::endl;
            qbert.getJ();
//            std::cout << "muevo enemiJ" << std::endl;
//            std::cout << it.operator*()->isCoily << std::endl;
//            std::cout << "lolazo" << std::endl;

            it.operator*()->randomMovement(qbert.getI(), qbert.getJ());
            it.operator*()->resetRandomMoveTimer();
//            std::cout << "acabo de mover enemi" << std::endl;
        } else {
            it.operator*()->randomMoveTimerplusplus();
        }
        //}
    }

    void setMoveQBert(Direction dir) {
        if (!qbert.isColision() && !piramide.isPiramideCompleta() && qbert.getLives() != 0) {
            qbert.setMove(dir);
        }
    }

    void changeCube(){
        if(cuboTiempo != 0 && timerGlobal!= 0 && timerGlobal%(30*(45-15*(cuboTiempo-1))) == 0){
//            std::cout << cuboTiempo << std::endl;
            //genera un enemigo aleatorio
            std::random_device rd;
            std::mt19937 mt(std::chrono::system_clock::now().time_since_epoch().count());
            std::uniform_int_distribution<int> dist(0, 6);
            int i = dist(mt), j = dist(mt);
//            std::cout << i << " " << j << std::endl;
            piramide.changeCubeInverse(i,j);
        }
        timerGlobal++;
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

    void setPiramideCompleta(){
        piramide.setTime(180);
        piramide.setPiramideCompleta(true);
    }

};

#endif //QBERT_ESCENA_HPP
