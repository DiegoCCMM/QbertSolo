//
// Created by patri on 13/04/2021.
//

#ifndef QBERT_PANTALLAINFONIVEL_HPP
#define QBERT_PANTALLAINFONIVEL_HPP

class PantallaInfoNivel{
    float width, height;
    int timer = 0;
    bool finish = false;

    int level;

    ALLEGRO_BITMAP *drawLevel = al_load_bitmap("../sprites/fonts.png");
    ALLEGRO_BITMAP *drawCircle = al_load_bitmap("../sprites/circle.png");
    ALLEGRO_BITMAP *drawNumber = al_load_bitmap("../sprites/fonts.png");

    ALLEGRO_SAMPLE *musicSound;
    ALLEGRO_SAMPLE *coinSound = al_load_sample("../sounds/coin.ogg");

    Piramide piramide;
    QBert qbert; // ??

public:

    PantallaInfoNivel(float width, float height) : width(width), height(height) {}

    void sonidoMoneda(){
        al_play_sample(coinSound, 1.0, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
    }

    void startLevel(int _level){
        level = _level;
        // TODO: si la musica se repite en algun nivel --> anyadir condiciones para el path (if's)
        std::string path = "../sounds/level-" + std::to_string(level) + ".ogg";
        musicSound = al_load_sample(path.c_str());
        al_play_sample(musicSound, 1.0, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);

        piramide.loadMap_IN(level, 1, width, height);
        qbert = QBert(piramide);
    }

    void movement() {
        // level 1 --> 2, 1, 3, 0
        if(timer%20==0 && timer < 25) {
            if(level == 1) ;//qbert.setMove(DOWNRIGHT);
            else {

            }
        }/*
        else if(timer%50==0 && timer < 220) {
            if(level == 1) qbert.setMove(TOPLEFT);
            else {

            }
        }
        else if(timer%50==0 && timer < 320) {
            if(level == 1) qbert.setMove(DOWNLEFT);
            else {

            }
        }
        else if(timer%50==0 && timer < 420) {
            if(level == 1) qbert.setMove(TOPRIGHT);
            else {

            }
        }*/

        qbert.movement_IN(&piramide);
        timer++;
        if(timer > 200) finish = true;
    }

    void drawBitmap() {
        float x, y;

        // Cargar piramide
        piramide.drawBitmap_IN();

        // Cargar Q*Bert
        qbert.drawBitmap();
/*
        // Cargar letras level
        al_draw_bitmap_region(drawLevel, 0, 4 * 8,
                              12*8, 2*8, x, y, 0); // CAMBIAR

        // Cargar circulo
        al_draw_bitmap_region(drawCircle, 0, 4 * 8,
                              12*8, 2*8, x, y, 0); // CAMBIAR

        // Cargar numero --> parpadeante
        if(timer < 200 && timer%20==0){
            al_draw_bitmap_region(drawNumber, 0, 4 * 8,
                                  12*8, 2*8, x, y, 0); // CAMBIAR
        }*/
    }

    void resize(float _width, float _height){
        width = _width;
        height = _height;

        piramide.resizeMap_IN(width, height);
        qbert.resize(&piramide);

        // TODO: Resize el resto de elementos
    }

    void destroy(){
        qbert.destroy();
        piramide.destroy_IN();

        al_destroy_bitmap(drawLevel);
        al_destroy_bitmap(drawCircle);
        al_destroy_bitmap(drawNumber);

        al_destroy_sample(musicSound);
        al_destroy_sample(coinSound);
    }

    bool fin(){
        return finish;
    }
};

#endif //QBERT_PANTALLAINFONIVEL_HPP
