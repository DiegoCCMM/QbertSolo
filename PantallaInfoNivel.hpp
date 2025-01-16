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

    ALLEGRO_BITMAP *infoLevel = al_load_bitmap("./sprites/infonivel.png");

    ALLEGRO_SAMPLE *musicSound;
    ALLEGRO_SAMPLE *coinSound = al_load_sample("./sounds/coin.ogg");

    Piramide piramide;
    QBert qbert;

public:

    PantallaInfoNivel(float width, float height) : width(width), height(height) {}

    void sonidoMoneda(){
        al_play_sample(coinSound, 1.0, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
    }

    void startLevel(int _level){
        level = _level;

        std::string path;
        if(level == 1 || level == 8) path = "./sounds/tune-1.ogg";
        else if(level == 2 || level == 7) path = "./sounds/tune-2.ogg";
        else if(level == 3 || level == 5 || level == 9) path = "./sounds/tune-3.ogg";
        else if(level == 4 || level == 6) path = "./sounds/tune-4.ogg";

        musicSound = al_load_sample(path.c_str());
        al_play_sample(musicSound, 1.0, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);

        piramide.loadMap_IN(level, 1, width, height);
        qbert = QBert(piramide);
    }

    void movement() {
        // level 1 --> 2, 1, 3, 0
        if(timer > 100 && timer < 102) {
            if(level == 1) qbert.setMove(DOWNRIGHT);
            else {
                qbert.setMove(DOWNRIGHT);
            }
        }
        else if(timer > 120 && timer < 122) {
            if(level == 1) qbert.setMove(TOPLEFT);
            else {
                qbert.setMove(DOWNLEFT);
            }
        }
        else if(timer > 140 && timer < 142) {
            if(level == 1) qbert.setMove(DOWNLEFT);
            else {
                qbert.setMove(TOPLEFT);
            }
        }
        else if(timer > 160 && timer < 162) {
            if(level == 1) qbert.setMove(TOPRIGHT);
            else {
                qbert.setMove(DOWNRIGHT);
            }
        }
        else if(timer > 180 && timer < 182) {
            if(level != 1) qbert.setMove(TOPRIGHT);
        }
        else if(timer > 200 && timer < 202) {
            if(level != 1) qbert.setMove(DOWNLEFT);
        }

        qbert.movement_IN(&piramide);
        timer++;
        if(timer > 240) finish = true;
    }

    void drawBitmap() {
        float x, y;

        // Cargar piramide
        piramide.drawBitmap_IN();

        // Cargar Q*Bert
        qbert.drawBitmap();

        // Cargar letras level
        int sourceX = 220, sourceY = 50;
        if(level==1 || level==5 || level==9) al_draw_bitmap_region(infoLevel, 0, 32*2, sourceX, sourceY, width/2-sourceX/2, height/2, 0);
        if(level==3 || level==7) al_draw_bitmap_region(infoLevel, 0, 32*2+55, sourceX, sourceY, width/2-sourceX/2, height/2, 0);
        if(level==2 || level==4 || level==6 || level==8) al_draw_bitmap_region(infoLevel, 0, 32*2+55*2, sourceX, sourceY, width/2-sourceX/2, height/2, 0);

        // Cargar circulo
        sourceX = 40, sourceY = 40;
        al_draw_bitmap_region(infoLevel, 0, 20,
                              sourceX, sourceY, width/2-sourceX/2+3,height/2+70, 0);

        // Cargar numero --> parpadeante
        sourceX = 16, sourceY = 20;
        if( (timer>10 && timer<30) || (timer > 60 && timer<80) || timer > 100) {
            al_draw_bitmap_region(infoLevel, sourceX * (level - 1), 0,
                                  sourceX, sourceY, width / 2 - sourceX / 2, height / 2 + 79, 0);
        }
    }

    void resize(float _width, float _height){
        width = _width;
        height = _height;

        piramide.resizeMap_IN(width, height);
        qbert.resize(&piramide);
    }

    void destroy(){
        qbert.destroy();
        piramide.destroy_IN();

        al_destroy_bitmap(infoLevel);

        al_destroy_sample(musicSound);
        al_destroy_sample(coinSound);
    }

    bool fin(){
        return finish;
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

};

#endif //QBERT_PANTALLAINFONIVEL_HPP
