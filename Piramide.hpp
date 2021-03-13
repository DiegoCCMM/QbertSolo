//
// Created by patri on 13/03/2021.
//

#ifndef QBERT_PIRAMIDE_HPP
#define QBERT_PIRAMIDE_HPP

struct Cube {
    float x, y;
    int color;
    ALLEGRO_BITMAP *draw;
};

class Piramide {
private:
    constexpr static const float cubeSize = 32.0;
    float sx[3], sy[3];
    int sigCubo[3];

public:
    Cube map[7][7];

    /* Carga el mapa de la piramide de un .txt según el nivel y la ronda */
    void loadMap(int level, int round, float width, float height){
        ALLEGRO_BITMAP *cubes = al_load_bitmap("../sprites/cubos.png");
        must_init(cubes, "cubes");

        // TODO: Segun el nivel establecer sx y sy leyendo de txt correspondiente
        sx[0] = 0, sy[0] = 0;
        sx[1] = 0, sy[1] = 32;
        sigCubo[0] = 1, sigCubo[1] = 1;
        // -----

        width /= 2, height = 100; // TODO: poner bien la relacion de width y height

        for(int i=0; i<7; i++){ // Fila
            width -= cubeSize/2;
            for(int j=0; j<i+1; j++){ // Columna
                Cube cubo{width + j*cubeSize, height, 0, cubes};
                map[i][j] = cubo;
            }
            height += cubeSize*3/4;
        }
    }

    /* Muestra por pantalla todos los cubos formando la piramide */
    void drawMap() {
        for(int i=0; i<7; i++){ // Fila
            for(int j=0; j<i+1; j++){ // Columna
                al_draw_bitmap_region(map[i][j].draw, sx[map[i][j].color], sy[map[i][j].color], cubeSize, cubeSize,
                                      map[i][j].x, map[i][j].y, 0);
            }
        }
    }

    // TODO: revisar, funciona mal
    /* Establece la posicion correspondiente de la piramide segun el nuevo tamaño de la ventana */
    void resizeMap(float width, float height){
        width /= 2, height = 100; // TODO: poner bien la relacion de width y height
        for(int i=0; i<7; i++){ // Fila
            width -= cubeSize/2;
            for(int j=0; j<i+1; j++){ // Columna
                map[i][j].x = width + j*cubeSize;
                std::cout << map[i][j].x << std::endl;
                map[i][j].y = height;
            }
            height += cubeSize*3/4;
        }
    }

    /* Cambia el cubo [i,j] al siguiente color correspondiente */
    void changeCube(int i, int j){
        map[i][j].color = sigCubo[map[i][j].color];
    }

    void must_init(bool test, const char *description) {
        if (test) return;

        printf("couldn't initialize %s\n", description);
        exit(1);
    }

};


#endif //QBERT_PIRAMIDE_HPP
