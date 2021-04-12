//
// Created by patri on 13/03/2021.
//

#ifndef QBERT_PIRAMIDE_HPP
#define QBERT_PIRAMIDE_HPP

#include <fstream>

constexpr static const float cubeSize = 32.0;

struct Cube {
    float x, y;
    int color;
    ALLEGRO_BITMAP *draw;
};

class Piramide {
    float sx[3], sy[3];
    int sigCubo[3];

public:
    Cube map[7][7]; // Ha de ser publico

    /* Carga el mapa de la piramide de un .txt según el nivel y la ronda */
    void loadMap(int level, int round, float width, float height){
        ALLEGRO_BITMAP *cubes = al_load_bitmap("../sprites/cubos.png");
        must_init(cubes, "cubes");

        std::ifstream file("../levels/level" + std::to_string(level) + "_" + std::to_string(round) + ".txt");
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
    void changeCube(int i, int j){
        map[i][j].color = sigCubo[map[i][j].color];
    }

    static void must_init(bool test, const char *description) {
        if (test) return;

        printf("couldn't initialize %s\n", description);
        exit(1);
    }

    void destroy(){
        for(int i=0; i<7; i++){ // Fila
            for(int j=0; j<i+1; j++){ // Columna
                al_destroy_bitmap(map[i][j].draw);
            }
        }
    }

};


#endif //QBERT_PIRAMIDE_HPP
