//
// Created by patri on 23/04/2021.
//

#ifndef QBERT_CREDITOS_HPP
#define QBERT_CREDITOS_HPP

class Creditos{
    enum Pantalla {
        REGNOM, HIGHSCORES
    };

    float width, height;
    int timer = 0;

    Pantalla pant;
    int scores[23];
    ALLEGRO_BITMAP *drawInit = al_load_bitmap("../sprites/fonts.png");
    ALLEGRO_SAMPLE *helloSound = al_load_sample("../sounds/qbert-hello.ogg");

public:


    void movement() {
        if(pant == REGNOM){

        } else {

        }
    }

    void drawBitmap() {
        if(pant == REGNOM){

        } else {

        }
    }

    void resize(float _width, float _height){
        width = _width;
        height = _height;

        if(pant == REGNOM){
            //platillo.setX(width/2+platillo.getXRespectCube());
            //platillo.setY(height/2+platillo.getYRespectCube());
        } else { // HIGHSCORE

        }

    }

    void destroy() {
        if(pant == REGNOM){

        } else {

        }
    }


    /*************************
     * GETTER'S AND SETTER'S *
     *************************/

    //

};

#endif //QBERT_CREDITOS_HPP
