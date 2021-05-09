//
// Created by patri on 12/04/2021.
//

#ifndef QBERT_PANTALLAINICIAL_HPP
#define QBERT_PANTALLAINICIAL_HPP

#include <unistd.h>
#include "QBert.hpp"
#include "Platillo.hpp"

#define topTimer 80

class PantallaInicial{
    enum Pantalla {
        INICIO, INSTRUCCIONES
    };

    float width, height;
    int timer = 0;

    Pantalla pant = INICIO;
    ALLEGRO_BITMAP *drawInit = al_load_bitmap("../sprites/fonts.png");
    ALLEGRO_SAMPLE *helloSound = al_load_sample("../sounds/qbert-hello.ogg");
    QBert qbert = QBert();
    Platillo platillo = Platillo(width, height);

public:

    PantallaInicial(float width, float height) : width(width), height(height) {
        escenarioInit();

        al_play_sample(helloSound, 1.0, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
    }

    void escenarioInit(){
        platillo.setXRespectCube(-175);
        platillo.setYRespectCube(100);
        platillo.setX(width/2+platillo.getXRespectCube());
        platillo.setY(height/2+platillo.getYRespectCube());
        qbert.setX(platillo.getX());
        qbert.setY(platillo.getY()-6);
    }

    void escenarioInstruc(){
        pant = INSTRUCCIONES;
        timer = 0;
        qbert.setXRespectCube(20);
        qbert.setYRespectCube(50);
        qbert.setX(width/2-165+qbert.getXRespectCube());
        qbert.setY(height/2-140+qbert.getYRespectCube());
    }

    void movement() {
        if(pant == INICIO) {
            platillo.timerplusplus();
            if (platillo.getTimer() > 2) {
                platillo.setSourceJ(platillo.getSourceJ() + 1);
                if (platillo.getSourceJ() > 3) {
                    platillo.setSourceJ(0);
                }
                platillo.setTimer(0);
            }

            platillo.setXRespectCube(platillo.getXRespectCube()+ 1.75);
            platillo.setX(platillo.getX() + 1.75);
            qbert.setX(platillo.getX());

            if(platillo.getX() > width/2+4*32) escenarioInstruc();
        }
        else { // INSTRUCCIONES
            if(timer!=0 && timer%topTimer == 0 && timer <= topTimer*5) qbert.setJumping(true);
            if(timer >= topTimer*6) escenarioInit();
            qbert.movementInstr();
            timer++;
        }
    }

    void drawBitmap() {
        if(pant == INICIO) pantallaInicial();
        else pantallaInstruc();
    }

    void pantallaInicial(){
        float x = width/2-48, y = height/2-8*3;
        // Cargar logo Q*Bert
        al_draw_bitmap_region(drawInit, 0, 4 * 8,
                              12*8, 2*8, x, y, 0);

        // Cargar (C)
        x = width/2-88, y = height/2;
        al_draw_bitmap_region(drawInit, 18 * 8, 4 * 8,
                              8, 8, x, y, 0);
        x += 8;

        // Cargar anyo
        std::string frase = "2021";
        for (std::string::size_type i = 0; i < frase.size(); i++) {
            al_draw_bitmap_region(drawInit, (int(frase[i]) % 48) * 8, 0,
                                  8, 8, x, y, 0);
            x+=8;
        }

        // Cargar frase
        frase = " BY PATRI Y DIEGO\\ALL RIGHTS RESERVED";
        for (std::string::size_type i = 0; i < frase.size(); i++) {
            if(int(frase[i]) == 92) { // '\' --> \n
                x = width/2-76-8;
                y+=9;
            } else if(int(frase[i]) != 32){ // En caso de ser un espacio no se dibuja
                al_draw_bitmap_region(drawInit, (int(frase[i]) % 65) * 8, 9,
                                      8, 8, x, y, 0);
            }
            x+=8;
        }

        x = width/2-76, y += 8*4;
        frase = "PRESS ENTER TO PLAY";
        for (std::string::size_type i = 0; i < frase.size(); i++) {
            if(int(frase[i]) != 32){ // En caso de ser un espacio no se dibuja
                al_draw_bitmap_region(drawInit, (int(frase[i]) % 65) * 8, 8,
                                      8, 8, x, y, 0);
            }
            x+=8;
        }

        platillo.drawBitmap();
        qbert.drawBitmap();
    }

    void pantallaInstruc(){
        float x = width/2-165+20, y = height/2-140+20;
        // Cargar logo Q*Bert
        al_draw_bitmap_region(drawInit, 0, 4 * 8,
                              12*8, 2*8, x, y, 0);

        qbert.drawBitmap();

        // Cargar frase
        float xBase = width/2-165+70;
        y = height/2-140+60;
        if(timer > topTimer*1) {
            x = xBase;
            std::string frase = "JUMP ON SQUARES TO\\CHANGE THEM TO\\THE TARGET COLOR";
            for (std::string::size_type i = 0; i < frase.size(); i++) {
                if (int(frase[i]) == 92) { // '\' --> \n
                    x = xBase-8;
                    y += 9;
                } else if (int(frase[i]) != 32) { // En caso de ser un espacio no se dibuja
                    al_draw_bitmap_region(drawInit, (int(frase[i]) % 65) * 8, 9,
                                          8, 8, x, y, 0);
                }
                x += 8;
            }

            qbert.setJ(0);
        }

        if(timer > topTimer*2){
            x = xBase+8*1, y += 8*3;
            std::string frase = "STAY ON PLAYFIELD!\\JUMPING OFF RESULTS\\"
                                "IN A FATAL PLUMMET\\UNLESS A DISK IS THERE";
            for (std::string::size_type i = 0; i < frase.size(); i++) {
                if (int(frase[i]) == 92) { // '\' --> \n
                    x = xBase+8*1-8;
                    y += 9;
                } else if(int(frase[i]) == 33){ // '!'
                    al_draw_bitmap_region(drawInit, 16*8, 4*8,
                                          8, 8, x, y, 0);
                } else if (int(frase[i]) != 32) { // En caso de ser un espacio no se dibuja
                    al_draw_bitmap_region(drawInit, (int(frase[i]) % 65) * 8, 9,
                                          8, 8, x, y, 0);
                }
                x += 8;
            }

            qbert.setJ(1);
        }

        if(timer > topTimer*3){
            x = xBase+8*2, y += 8*3;
            std::string frase = "AVOID ALL OBJECTS\\AND CREATURES THAT\\"
                                "ARE NOT GREEN";
            for (std::string::size_type i = 0; i < frase.size(); i++) {
                if (int(frase[i]) == 92) { // '\' --> \n
                    x = xBase+8*2-8;
                    y += 9;
                } else if(int(frase[i]) == 33){ // '!'
                    al_draw_bitmap_region(drawInit, 16*8, 4*8,
                                          8, 8, x, y, 0);
                } else if (int(frase[i]) != 32) { // En caso de ser un espacio no se dibuja
                    al_draw_bitmap_region(drawInit, (int(frase[i]) % 65) * 8, 9,
                                          8, 8, x, y, 0);
                }
                x += 8;
            }

            qbert.setJ(2);
        }

        if(timer > topTimer*4){
            x = xBase+8*3, y += 8*3;
            std::string frase = "USE SPINNING DISKS\\TO LURE SNAKE TO\\HIS DEATH";
            for (std::string::size_type i = 0; i < frase.size(); i++) {
                if (int(frase[i]) == 92) { // '\' --> \n
                    x = xBase+8*3-8;
                    y += 9;
                } else if(int(frase[i]) == 33){ // '!'
                    al_draw_bitmap_region(drawInit, 16*8, 4*8,
                                          8, 8, x, y, 0);
                } else if (int(frase[i]) != 32) { // En caso de ser un espacio no se dibuja
                    al_draw_bitmap_region(drawInit, (int(frase[i]) % 65) * 8, 9,
                                          8, 8, x, y, 0);
                }
                x += 8;
            }

            qbert.setJ(3);
        }

        if(timer > topTimer*5){
            x = xBase+8*4, y += 8*3;
            std::string frase = "EXTRA LIFE AT\\8000 AND EACH\\ADDITIONAL 14000";
            for (std::string::size_type i = 0; i < frase.size(); i++) {
                if (int(frase[i]) == 92) { // '\' --> \n
                    x = xBase+8*4-8;
                    y += 9;
                } else if(int(frase[i]) == 33){ // '!'
                    al_draw_bitmap_region(drawInit, 16*8, 4*8,
                                          8, 8, x, y, 0);
                } else if(int(frase[i]) >= 48 && int(frase[i]) <= 57){ // Numero
                    al_draw_bitmap_region(drawInit, (int(frase[i]) % 48) * 8, 0,
                                          8, 8, x, y, 0);
                } else if (int(frase[i]) != 32) { // En caso de ser un espacio no se dibuja
                    al_draw_bitmap_region(drawInit, (int(frase[i]) % 65) * 8, 9,
                                          8, 8, x, y, 0);
                }
                x += 8;
            }

            qbert.setJ(4);
        }

        // En caso de querer programar la animacion que cae el blob rojo
        /*if(timer > topTimer*6){ // Cae el blob rojo

        }*/

        // despues de todas las animaciones volver a INICIO
        if(timer > topTimer*6){
            pant = INICIO;
        }
    }

    void resize(float _width, float _height){
        width = _width;
        height = _height;

        if(pant == INICIO){
            platillo.setX(width/2+platillo.getXRespectCube());
            platillo.setY(height/2+platillo.getYRespectCube());
            qbert.setY(platillo.getY()-6);
        } else { // INSTRUCCIONES
            qbert.setX(width/2-165+qbert.getXRespectCube());
            qbert.setY(height/2-140+qbert.getYRespectCube());
        }
    }

    void destroy(){
        qbert.destroy();
        platillo.destroy();
        al_destroy_bitmap(drawInit);
        al_destroy_sample(helloSound);
    }
};

#endif //QBERT_PANTALLAINICIAL_HPP
