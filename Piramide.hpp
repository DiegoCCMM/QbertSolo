//
// Created by patri on 13/03/2021.
//

#ifndef QBERT_PIRAMIDE_HPP
#define QBERT_PIRAMIDE_HPP

#include <fstream>
#include <iostream>

constexpr static const float cubeSize = 32.0;

struct Cube {
    float x, y;
    int color;
    ALLEGRO_BITMAP *draw;
};

class Piramide {
    float sx[3], sy[3];
    int sigCubo[3];
    bool piramideCompleta;

    int time, level;
    ALLEGRO_SAMPLE *finishSound = al_load_sample("../sounds/levelup.ogg");

public:
    Cube map[7][7]; // Ha de ser publico

    /* Carga el mapa de la piramide de un .txt según el nivel y la ronda */
    void loadMap(int level, int round, float width, float height){
        this->level = level;
        ALLEGRO_BITMAP *cubes = al_load_bitmap("../sprites/cubos.png");
        must_init(cubes, "cubes");

        std::ifstream file("../levels/piramide/level" + std::to_string(level) + "_" + std::to_string(round) + ".txt");
        for(int i=0; !file.eof(); i++) {
            file >> sx[i] >> sy[i] >> sigCubo[i];
        }
        file.close();

        width /= 2, height = height/2 - cubeSize*3 + cubeSize;

        for(int i=0; i<7; i++){ // Fila
            width -= cubeSize/2;
            for(int j=0; j<i+1; j++){ // Columna
                Cube cubo{width + (float)j*cubeSize, height, 0, cubes};
                map[i][j] = cubo;
            }
            height += cubeSize*3/4;
        }

        piramideCompleta = false;
        time = 0;
    }

    /* Muestra por pantalla todos los cubos formando la piramide */
    void drawBitmap() {
        for(int i=0; i<7; i++){ // Fila
            for(int j=0; j<i+1; j++){ // Columna
                al_draw_bitmap_region(map[i][j].draw, sx[map[i][j].color], sy[map[i][j].color],
                                      cubeSize, cubeSize, map[i][j].x, map[i][j].y, 0);
            }
        }
    }

    /* Establece la posicion correspondiente de la piramide segun el nuevo tamaño de la ventana */
    void resizeMap(float width, float height) {
        width /= 2, height = height/2 - cubeSize*3 + cubeSize;
        for(int i=0; i<7; i++){ // Fila
            width -= cubeSize/2;
            for(int j=0; j<i+1; j++){ // Columna
                map[i][j].x = width + (float)j*cubeSize;
                map[i][j].y = height;
            }
            height += cubeSize*3/4;
        }
    }

    /* Cambia el cubo [i,j] al siguiente color correspondiente */
    void changeCube(int i, int j, int &puntuacion){
        int aux_color = sigCubo[map[i][j].color];

        if(aux_color != map[i][j].color && aux_color == 1) puntuacion += 15; // Intermediate color
        else if(aux_color != map[i][j].color && aux_color == 2) puntuacion += 25; // Target color

        map[i][j].color = aux_color;

        // Comprueba si todos los cuadrados estan en el color final
        bool completa = true;
        for(int h=0; h<7 && completa; h++){
            for(int k=0; k<h+1 && completa; k++) { // Columna
                completa = map[h][k].color == 2;
            }
        }
        piramideCompleta = completa;
    }

    /* Cambia el cubo [i,j] al color anterior correspondiente --> Slick y Sam */
    void changeCubeInverse(int i, int j){
        if(map[i][j].color != 0) { // Solo nos intersa cambiar el color cuando no es el inicial
            if (level==1 || level==3) {
                map[i][j].color = 0;
            } else{
                map[i][j].color--;
            }
        }
    }

    void destroy(){
        for(int i=0; i<7; i++){ // Fila
            for(int j=0; j<i+1; j++){ // Columna
                al_destroy_bitmap(map[i][j].draw);
            }
        }
    }

    static void must_init(bool test, const char *description) {
        if (test) return;

        printf("couldn't initialize %s\n", description);
        exit(1);
    }

    void setAllColor(int cubo){
        for(int i=0; i<7; i++){ // Fila
            for(int j=0; j<i+1; j++){ // Columna
                map[i][j].color = cubo;
            }
        }
    }

    /** Funciones explicitas para crear la piramide de la pantalla de 'Subir nivel/Info Nivel(IN)' **/

    /* Carga el mapa de la piramide de un .txt según el nivel y la ronda */
    void loadMap_IN(int level, int round, float width, float height){
        ALLEGRO_BITMAP *cubes = al_load_bitmap("../sprites/cubos.png");
        must_init(cubes, "cubes");

        std::ifstream file("../levels/piramide/level" + std::to_string(level) + "_" + std::to_string(round) + ".txt");
        for(int i=0; !file.eof(); i++) {
            file >> sx[i] >> sy[i] >> sigCubo[i];
        }
        file.close();

        width /= 2, height = height/2 - cubeSize*3 + cubeSize;

        for(int i=0; i<3; i++){ // Fila
            width -= cubeSize/2;
            for(int j=0; j<i+1; j++){ // Columna
                if(i==2 && (j==0 || j==2)) ;
                else {
                    Cube cubo{width + (float) j * cubeSize, height-30, 0, cubes};
                    map[i][j] = cubo;
                }
            }
            height += cubeSize*3/4;
        }
    }

    /* Muestra por pantalla todos los cubos formando la piramide */
    void drawBitmap_IN() {
        for(int i=0; i<3; i++){ // Fila
            for(int j=0; j<i+1; j++){ // Columna
                if(i==2 && (j==0 || j==2)) ;
                else
                    al_draw_bitmap_region(map[i][j].draw, sx[map[i][j].color], sy[map[i][j].color],
                                          cubeSize, cubeSize, map[i][j].x, map[i][j].y, 0);
            }
        }
    }

    /* Establece la posicion correspondiente de la piramide segun el nuevo tamaño de la ventana */
    void resizeMap_IN(float width, float height) {
        width /= 2, height = height/2 - cubeSize*3 + cubeSize;
        for(int i=0; i<3; i++){ // Fila
            width -= cubeSize/2;
            for(int j=0; j<i+1; j++){ // Columna
                if(i==2 && (j==0 || j==2)) ;
                else {
                    map[i][j].x = width + (float) j * cubeSize;
                    map[i][j].y = height-30;
                }
            }
            height += cubeSize*3/4;
        }
    }

    void destroy_IN(){
        for(int i=0; i<3; i++){ // Fila
            for(int j=0; j<i+1; j++){ // Columna
                if(i==2 && (j==0 || j==2)) ;
                else al_destroy_bitmap(map[i][j].draw);
            }
        }
    }


    /*************************
     * GETTER'S AND SETTER'S *
     *************************/

    bool isPiramideCompleta() const { return piramideCompleta; }
    void setPiramideCompleta(bool _piramideCompleta) { Piramide::piramideCompleta = _piramideCompleta; }

    const float getSx(int _i) const { return sx[_i]; }
    const float getSy(int _i) const { return sy[_i]; }

    int getTime() const { return time; }
    void setTime(int time) { Piramide::time = time; }

    ALLEGRO_SAMPLE *getFinishSound() const { return finishSound; }

    int getLevel() const { return level; }

};

#endif //QBERT_PIRAMIDE_HPP
