//
// Created by patri on 14/04/2021.
//
#include "Piramide.hpp"
#include "Objeto.hpp"

#ifndef QBERT_ESCENA_HPP
#define QBERT_ESCENA_HPP

#define periodoEnemigos 12

class Escena{
    float width, height;

public:

    Piramide piramide;
    QBert qbert;
    std::list<Enemy> enemies;
    std::list<Platillo> platillos;
    int level, round, periodEnemies = 0;
    int puntuacion;
    bool gameover = false;


    /* Constructor */
    Escena(float _width, float _height) : width(_width), height(_height), level(1), round(1) {}

    void load(float _width, float _height){
        width = _width, height = _height;
        puntuacion = 0;

        // Cargar mapa
        piramide.loadMap(level, round, width, height);

        // Cargar personajes
        qbert = QBert(piramide);

        // TODO: implementar cuando aparecen los platillos
        //Platillo plato = Platillo(piramide, 3, IZQ);
        //platillos.push_back(plato);
    }

    bool piramideCompleta(){
        return piramide.isPiramideCompleta();
    }

    void movementAll() {
        qbert.movement(&piramide, height, platillos);

        if(!qbert.isFalling()) {
            checkRandMovementEnemies();
            for (std::_List_iterator<Enemy> it = enemies.begin(); it != enemies.end(); it++) {
                it->movement(&piramide, height);
            }

            for (std::_List_iterator<Platillo> it = platillos.begin(); it != platillos.end(); it++) {
                it->movement();
            }
        }
    }

    void drawAll() {

        if (qbert.isFalling()) { // Si QBert esta cayendo primero se dibuja a QBert y luego la piramide
            qbert.drawBitmap();
            piramide.drawBitmap();
            // TODO: dibujar resto de componentes de la pantalla MENOS los enemigos
            // Recordar que mientras cae el resto de elementos dejan de moverse
            enemies.clear();
            for (std::_List_iterator<Platillo> it = platillos.begin(); it != platillos.end(); it++) {
                it->drawBitmap();
            }
        } else {
            piramide.drawBitmap();
            for (std::_List_iterator<Platillo> it = platillos.begin(); it != platillos.end(); it++) {
                it->drawBitmap();
            }
            qbert.drawBitmap();
            for (std::_List_iterator<Enemy> it = enemies.begin(); it != enemies.end(); it++) {
                it->drawBitmap();
            }

            // TODO: dibujar resto de componentes de la pantalla
        }
    }

    void generarEnemigos(){

        if(periodEnemies == periodoEnemigos){
            //genera un enemigo aleatorio
            std::random_device rd;
            std::mt19937 mt(rd());
            std::uniform_int_distribution<int> dist(0, 60);
            int eleccion = dist(mt);
            if(eleccion >= 0 && eleccion <= 14){
                //redblob o poder
                //Enemy redblob = Enemy(piramide, "Redblob", 1, 0, 9, 0); // X e Y (pixeles) posicion respecto al cubo[i,j]
                //enemies.push_back(redblob);
            }else if(eleccion >= 15 && eleccion <= 29){
                //coily
                Coily coily = Coily(piramide, "coilyBola", 1, 0, 9, -3);
                enemies.push_back(coily);
            }else if(eleccion >= 30 && eleccion <= 44){
                //ugg o wrong way
            }else if(eleccion >= 45 && eleccion <= 60){
                //slick o sam
            }
            //reinicio periodEnemies
            periodEnemies = 0;
        }else{
            periodEnemies++;
        }
    }

    void resizAll(float _width, float _height){
        width = _width, height = _height;
        piramide.resizeMap(width, height);

        for (std::_List_iterator<Enemy> it = enemies.begin(); it != enemies.end(); it++) {
            it->resize(&piramide);
        }

        for (std::_List_iterator<Platillo> it = platillos.begin(); it != platillos.end(); it++) {
            it->resize(&piramide);
        }

        qbert.resize(&piramide);
    }

    void destroyAll(){
        piramide.destroy();
        qbert.destroy();

        for (std::_List_iterator<Enemy> it = enemies.begin(); it != enemies.end(); it++){
            it->destroy();
        }

        for (std::_List_iterator<Platillo> it = platillos.begin(); it != platillos.end(); it++) {
            it->destroy();
        }
    }

    void checkRandMovementEnemies() {
        for (std::_List_iterator<Enemy> it = enemies.begin(); it != enemies.end(); it++){
            if(it->getRandMoveTimer() == it->getRandMovePeriod()){
                it->randomMovement();
                it->resetRandomMoveTimer();
            }else {
                it->randomMoveTimerplusplus();
            }
        }
    }

    /*************************
     * GETTER'S AND SETTER'S *
     *************************/

    int getLevel() const { return level; }
    void setLevel(int _level) { Escena::level = _level; }

    int getRound() const { return round; }
    void setRound(int _round) { Escena::round = _round; }

    bool isGameover() const { return gameover; }
    void setGameover(bool gameover) { Escena::gameover = gameover; }

};

#endif //QBERT_ESCENA_HPP
