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
        INICIO, INSTRUCCIONES, MENU, CONTROLES, DIFICULTAD
    };
    
    enum Controls {
        NONE, ARRIBADER, ARRIBAIZQ, ABAJODER, ABAJOIZQ
    };

    float width, height;
    int timer = 0;

    ALLEGRO_BITMAP *coilyBitmap = al_load_bitmap("../sprites/coilyEstirado.png");
    ALLEGRO_BITMAP *samBitmap = al_load_bitmap("../sprites/Sam.png");
    ALLEGRO_BITMAP *slickMap = al_load_bitmap("../sprites/Slick.png");
    ALLEGRO_BITMAP *redblobBitmap = al_load_bitmap("../sprites/RedBlob.png");

    ALLEGRO_BITMAP *fonts = al_load_bitmap("../sprites/fonts.png");
    ALLEGRO_SAMPLE *helloSound = al_load_sample("../sounds/qbert-hello.ogg");
    QBert qbert = QBert();
    Platillo platillo = Platillo(width, height);
    int posMenu, posControles, posDificultad;

public:
    int cuboID, coilyID, slicksamID, blobID, level;
    std::string cubos[4] = {"OFF", "45 SEC", "30 SEC", "15 SEC"},
                coily[3] = {"CLASSIC", "MEDIUM", "HARD"},
                slicksam[3] = {"CLASSIC", "MEDIUM", "HARD"},
                blob[3] = {"CLASSIC", "MEDIUM", "HARD"};
    int controls[4];
    std::string controlsName[4];
    Controls contr;

    Pantalla pant = INICIO;

    PantallaInicial(float width, float height,
                    int _cuboID, int _coilyID, int _slicksamID, int _blobID,
                    int _controls[], int _level) : width(width), height(height) {
        escenarioInit();
        cuboID = _cuboID, coilyID = _coilyID, slicksamID = _slicksamID, blobID = _blobID;
        controls[0] = _controls[0], controls[1] = _controls[1], controls[2] = _controls[2], controls[3] = _controls[3];
        level = _level;

        al_play_sample(helloSound, 1.0, 1, 1, ALLEGRO_PLAYMODE_ONCE, 0);
    }

    void escenarioInit(){
        pant = INICIO;
        platillo.setXRespectCube(-175);
        platillo.setYRespectCube(100);
        platillo.setX(width/2+platillo.getXRespectCube());
        platillo.setY(height/2+platillo.getYRespectCube());
        qbert.setX(platillo.getX());
        qbert.setY(platillo.getY()-6);
    }

    void escenarioMenu(){
        pant = MENU;
        posMenu = 0;
        qbert.setJ(0);
        qbert.setXRespectCube(20);
        qbert.setYRespectCube(50);
        qbert.setX(width/2-155+qbert.getXRespectCube());
        qbert.setY(height/2-120+qbert.getYRespectCube());
    }

    void escenarioInstruc(){
        pant = INSTRUCCIONES;
        timer = 0;
        qbert.setJ(0);
        qbert.setXRespectCube(20);
        qbert.setYRespectCube(50);
        qbert.setX(width/2-165+qbert.getXRespectCube());
        qbert.setY(height/2-140+qbert.getYRespectCube());
    }

    void escenarioControles(){
        pant = CONTROLES;
        contr = NONE;
        posControles = 0;
        qbert.setJ(0);
        qbert.setXRespectCube(20);
        qbert.setYRespectCube(50);
        qbert.setX(width/2-155+qbert.getXRespectCube());
        qbert.setY(height/2-120+qbert.getYRespectCube());
    }

    void escenarioDificultad(){
        pant = DIFICULTAD;
        posDificultad = 0;
        qbert.setJ(0);
        qbert.setXRespectCube(20);
        qbert.setYRespectCube(50);
        qbert.setX(width/2-155+qbert.getXRespectCube());
        qbert.setY(height/2-120+qbert.getYRespectCube());

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

            if(platillo.getX() > width/2+4*33) escenarioInit();
        }
        else if(pant == INSTRUCCIONES) {
            if(timer!=0 && timer%topTimer == 0 && timer <= topTimer*5) qbert.setJumping(true);
//            if(timer >= topTimer*6) escenarioInit();
            qbert.movementInstr();
            timer++;
        }
    }

    void drawBitmap() {
        if(pant == INICIO) {
            pantallaInicial();
        }
        else if(pant == INSTRUCCIONES) {
            pantallaInstruc();
        }
        else if(pant == MENU) {
            pantallaMenu();
        }
        else if(pant == CONTROLES) {
            pantallaControles();
        }
        else if(pant = DIFICULTAD) {
            pantallaDificultad();
        }
    }

    void pantallaInicial(){
        float x = width/2-48, y = height/2-8*3;
        // Cargar logo Q*Bert
        al_draw_bitmap_region(fonts, 0, 4 * 8,
                              12*8, 2*8, x, y, 0);

        // Cargar (C)
        x = width/2-88, y = height/2;
        al_draw_bitmap_region(fonts, 18 * 8, 4 * 8,
                              8, 8, x, y, 0);
        x += 8;

        // Cargar anyo
        std::string frase = "2021";
        for (std::string::size_type i = 0; i < frase.size(); i++) {
            al_draw_bitmap_region(fonts, (int(frase[i]) % 48) * 8, 0,
                                  8, 8, x, y, 0);
            x+=8;
        }

        // Cargar frase
        y+=2;
        frase = " BY PATRI Y DIEGO\\ALL RIGHTS RESERVED";
        for (std::string::size_type i = 0; i < frase.size(); i++) {
            if(int(frase[i]) == 92) { // '\' --> \n
                x = width/2-76-8;
                y+=9;
            } else if(int(frase[i]) != 32){ // En caso de ser un espacio no se dibuja
                al_draw_bitmap_region(fonts, (int(frase[i]) % 65) * 8, 9,
                                      8, 8, x, y, 0);
            }
            x+=8;
        }

        x = width/2-76, y += 8*4;
        frase = "PRESS ENTER TO PLAY";
        for (std::string::size_type i = 0; i < frase.size(); i++) {
            if(int(frase[i]) != 32){ // En caso de ser un espacio no se dibuja
                al_draw_bitmap_region(fonts, (int(frase[i]) % 65) * 8, 8,
                                      8, 8, x, y, 0);
            }
            x+=8;
        }

        x = width/2-64, y += 8*2;
        frase = "PRESS F1 FOR MENU";
        for (std::string::size_type i = 0; i < frase.size(); i++) {
            if (int(frase[i]) >= 48 && int(frase[i]) <= 57) { // Numero
                al_draw_bitmap_region(fonts, (int(frase[i]) % 48) * 8 + 9 * 9, 0,
                                      8, 8, x, y, 0);
            } else if(int(frase[i]) != 32){ // En caso de ser un espacio no se dibuja
                al_draw_bitmap_region(fonts, (int(frase[i]) % 65) * 8, 8,
                                      8, 8, x, y, 0);
            }
            x+=8;
        }

        platillo.drawBitmap();
        qbert.drawBitmap();
    }

    void pantallaMenu(){

        float x = width/2-165+20, y = height/2-140+20;
        // Cargar logo Q*Bert
        al_draw_bitmap_region(fonts, 0, 4 * 8,
                              12*8, 2*8, x, y, 0);

        std::string frase;

        x = width / 2 + 8*4;
        frase = "ESC TO RETURN";                                                                                                                                                                             "EMPEZAR EN NIVEL     -" + std::to_string(level) + "-\\";
        for (std::string::size_type i = 0; i < frase.size(); i++) {
            if (int(frase[i]) != 32) { // En caso de ser un espacio no se dibuja
                al_draw_bitmap_region(fonts, (int(frase[i]) % 65) * 8, 8,
                                      8, 8, x, y, 0);
            }
            x += 8;
        }

        x = width / 2 - 8*14, y = height/2 - 8*8-2;
        frase = "INSTRUCTIONS\\"
                "DEFINE GAME KEYS\\"
                "DIFFICULTY\\"
                "START LEVEL     -" + std::to_string(level) + "-\\";
        for (std::string::size_type i = 0; i < frase.size(); i++) {
            if (int(frase[i]) == 92) { // '\' --> \n
                x = width / 2 - 8*14-8;
                y += 9*3;
            }
            else if(int(frase[i]) == 45){ // '-'
                al_draw_bitmap_region(fonts, 10 * 8 + 9 * 9, 8 * 4,
                                      8, 8, x, y, 0);
            }
            else if (int(frase[i]) >= 48 && int(frase[i]) <= 57) { // Numero
                al_draw_bitmap_region(fonts, (int(frase[i]) % 48) * 8 + 9 * 9, 0,
                                      8, 8, x, y, 0);
            }
            else if (int(frase[i]) != 32) { // En caso de ser un espacio no se dibuja
                al_draw_bitmap_region(fonts, (int(frase[i]) % 65) * 8, 8,
                                      8, 8, x, y, 0);
            }
            x += 8;
        }

        frase = "USE ARROWS TO MOVE";
        x = width/2-18*8/2, y = height/2+8*12;
        for (std::string::size_type i = 0; i < frase.size(); i++) {
            if (int(frase[i]) != 32) { // En caso de ser un espacio no se dibuja
                al_draw_bitmap_region(fonts, (int(frase[i]) % 65) * 8, 8 * 3,
                                      8, 8, x, y, 0);
            }
            x += 8;
        }

        qbert.drawBitmap();

    }

    void pantallaInstruc(){
        float x = width/2-165+20, y = height/2-140+20;
        // Cargar logo Q*Bert
        al_draw_bitmap_region(fonts, 0, 4 * 8,
                              12*8, 2*8, x, y, 0);

        std::string frase;
        x = width / 2 + 8*4;
        frase = "ESC TO RETURN";                                                                                                                                                                             "EMPEZAR EN NIVEL     -" + std::to_string(level) + "-\\";
        for (std::string::size_type i = 0; i < frase.size(); i++) {
            if (int(frase[i]) != 32) { // En caso de ser un espacio no se dibuja
                al_draw_bitmap_region(fonts, (int(frase[i]) % 65) * 8, 8,
                                      8, 8, x, y, 0);
            }
            x += 8;
        }

        qbert.drawBitmap();

        // Cargar frase
        float xBase = width/2-165+70;
        y = height/2-140+60;
        if(timer > topTimer*1) {
            x = xBase;
            frase = "JUMP ON SQUARES TO\\CHANGE THEM TO\\THE TARGET COLOR";
            for (std::string::size_type i = 0; i < frase.size(); i++) {
                if (int(frase[i]) == 92) { // '\' --> \n
                    x = xBase-8;
                    y += 9;
                } else if (int(frase[i]) != 32) { // En caso de ser un espacio no se dibuja
                    al_draw_bitmap_region(fonts, (int(frase[i]) % 65) * 8, 9,
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
                    al_draw_bitmap_region(fonts, 16 * 8, 4 * 8,
                                          8, 8, x, y, 0);
                } else if (int(frase[i]) != 32) { // En caso de ser un espacio no se dibuja
                    al_draw_bitmap_region(fonts, (int(frase[i]) % 65) * 8, 9,
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
                    al_draw_bitmap_region(fonts, 16 * 8, 4 * 8,
                                          8, 8, x, y, 0);
                } else if (int(frase[i]) != 32) { // En caso de ser un espacio no se dibuja
                    al_draw_bitmap_region(fonts, (int(frase[i]) % 65) * 8, 9,
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
                    al_draw_bitmap_region(fonts, 16 * 8, 4 * 8,
                                          8, 8, x, y, 0);
                } else if (int(frase[i]) != 32) { // En caso de ser un espacio no se dibuja
                    al_draw_bitmap_region(fonts, (int(frase[i]) % 65) * 8, 9,
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
                    al_draw_bitmap_region(fonts, 16 * 8, 4 * 8,
                                          8, 8, x, y, 0);
                } else if(int(frase[i]) >= 48 && int(frase[i]) <= 57){ // Numero
                    al_draw_bitmap_region(fonts, (int(frase[i]) % 48) * 8, 0,
                                          8, 8, x, y, 0);
                } else if (int(frase[i]) != 32) { // En caso de ser un espacio no se dibuja
                    al_draw_bitmap_region(fonts, (int(frase[i]) % 65) * 8, 9,
                                          8, 8, x, y, 0);
                }
                x += 8;
            }

            qbert.setJ(4);
        }
    }

    void pantallaControles(){
        float x = width/2-165+20, y = height/2-140+20;
        // Cargar logo Q*Bert
        al_draw_bitmap_region(fonts, 0, 4 * 8,
                              12*8, 2*8, x, y, 0);

        std::string frase;

        x = width / 2 + 8*4;
        frase = "ESC TO RETURN";                                                                                                                                                                             "EMPEZAR EN NIVEL     -" + std::to_string(level) + "-\\";
        for (std::string::size_type i = 0; i < frase.size(); i++) {
            if (int(frase[i]) != 32) { // En caso de ser un espacio no se dibuja
                al_draw_bitmap_region(fonts, (int(frase[i]) % 65) * 8, 8,
                                      8, 8, x, y, 0);
            }
            x += 8;
        }

        x = width / 2 - 8*14, y = height/2 - 8*8-2;
        translateControls();
        int naranja = 0;
        // ARRIBADER, ABAJOIZQ, ARRIBAIZQ, ABAJODER
        frase = "JUMP UP RIGHT     -" + controlsName[0] + "-\\"   //UP
                "JUMP DOWN LEFT    -" + controlsName[1] + "-\\"   //DOWN
                "JUMP UP LEFT      -" + controlsName[2] + "-\\"   //left
                "JUMP DOWN RIGHT   -" + controlsName[3] + "-\\";  //right                                                                                                                                                                                 "EMPEZAR EN NIVEL     -" + std::to_string(level) + "-\\";
        for (std::string::size_type i = 0; i < frase.size(); i++) {
            if (int(frase[i]) == 92) { // '\' --> \n
                x = width / 2 - 8*14-8;
                y += 9*3;
                naranja++;
            }
            else if(int(frase[i]) == 45){ // '-'
                if(posControles == naranja && contr != NONE) {
                    al_draw_bitmap_region(fonts, 10 * 8 + 9 * 9, 8 * 5,
                                          8, 8, x, y, 0);
                }
                else {
                    al_draw_bitmap_region(fonts, 10 * 8 + 9 * 9, 8 * 4,
                                          8, 8, x, y, 0);
                }
            }
            else if (int(frase[i]) >= 48 && int(frase[i]) <= 57) { // Numero
                if(posControles == naranja && contr != NONE) {
                    al_draw_bitmap_region(fonts, (int(frase[i]) % 48) * 8 + 9 * 9, 8 * 2,
                                          8, 8, x, y, 0);
                }
                else {
                    al_draw_bitmap_region(fonts, (int(frase[i]) % 48) * 8 + 9 * 9, 0,
                                          8, 8, x, y, 0);
                }
            }
            else if (int(frase[i]) != 32) { // En caso de ser un espacio no se dibuja
                if(posControles == naranja && contr != NONE) {
                    al_draw_bitmap_region(fonts, (int(frase[i]) % 65) * 8, 8 * 3,
                                          8, 8, x, y, 0);
                }
                else {
                    al_draw_bitmap_region(fonts, (int(frase[i]) % 65) * 8, 8,
                                          8, 8, x, y, 0);
                }
            }
            x += 8;
        }

        frase = "USE ARROWS TO MOVE";
        x = width/2-18*8/2, y = height/2+8*12;
        for (std::string::size_type i = 0; i < frase.size(); i++) {
            if (int(frase[i]) != 32) { // En caso de ser un espacio no se dibuja
                al_draw_bitmap_region(fonts, (int(frase[i]) % 65) * 8, 8 * 3,
                                      8, 8, x, y, 0);
            }
            x += 8;
        }

        frase = "PRESS ENTER TO CHANGE THE KEY";
        x = width/2-29*8/2, y += 8*2;
        for (std::string::size_type i = 0; i < frase.size(); i++) {
            if (int(frase[i]) != 32) { // En caso de ser un espacio no se dibuja
                al_draw_bitmap_region(fonts, (int(frase[i]) % 65) * 8, 8 * 3,
                                      8, 8, x, y, 0);
            }
            x += 8;
        }

        qbert.drawBitmap();
    }

    void pantallaDificultad(){

        float x = width/2-165+20, y = height/2-140+20;
        // Cargar logo Q*Bert
        al_draw_bitmap_region(fonts, 0, 4 * 8,
                              12*8, 2*8, x, y, 0);

        std::string frase;

        x = width / 2 + 8*4;
        frase = "ESC TO RETURN";                                                                                                                                                                             "EMPEZAR EN NIVEL     -" + std::to_string(level) + "-\\";
        for (std::string::size_type i = 0; i < frase.size(); i++) {
            if (int(frase[i]) != 32) { // En caso de ser un espacio no se dibuja
                al_draw_bitmap_region(fonts, (int(frase[i]) % 65) * 8, 8,
                                      8, 8, x, y, 0);
            }
            x += 8;
        }

        x = width / 2 - 8*14, y = height/2 - 8*8-2;
        int naranja = 0;
        frase = "CHANGE CUBE COLORS   -" + cubos[cuboID] + "-\\"
                "IA COILY             -" + coily[coilyID] + "-\\"
                "IA SLICK-SAM         -" + slicksam[slicksamID] + "-\\"
                "IA RED BLOB          -" + blob[blobID] + "-\\";
        for (std::string::size_type i = 0; i < frase.size(); i++) {
            if (int(frase[i]) == 92) { // '\' --> \n
                x = width / 2 - 8*14-8;
                y += 9*3;
                naranja++;
            }
            else if(int(frase[i]) == 45){ // '-'
                al_draw_bitmap_region(fonts, 10 * 8 + 9 * 9, 8 * 4,
                                      8, 8, x, y, 0);
            }
            else if (int(frase[i]) >= 48 && int(frase[i]) <= 57) { // Numero
                al_draw_bitmap_region(fonts, (int(frase[i]) % 48) * 8 + 9 * 9, 0,
                                      8, 8, x, y, 0);
            }
            else if (int(frase[i]) != 32) { // En caso de ser un espacio no se dibuja
                al_draw_bitmap_region(fonts, (int(frase[i]) % 65) * 8, 8,
                                      8, 8, x, y, 0);
            }
            x += 8;
        }

        frase = "USE ARROWS TO MOVE";
        x = width/2-18*8/2, y = height/2+8*12;
        for (std::string::size_type i = 0; i < frase.size(); i++) {
            if (int(frase[i]) != 32) { // En caso de ser un espacio no se dibuja
                al_draw_bitmap_region(fonts, (int(frase[i]) % 65) * 8, 8 * 3,
                                      8, 8, x, y, 0);
            }
            x += 8;
        }

        qbert.drawBitmap();
        al_draw_bitmap_region(coilyBitmap, 16*5-2,0,16,16*2,width/2-40,height/2-50, 0);
        al_draw_bitmap_region(samBitmap, 16*5-2,0,16,16*2,width/2-15,height/2-20, 0);
        al_draw_bitmap_region(slickMap, 16*5-2,0,16,16*2,width/2,height/2-20, 0);
        al_draw_bitmap_region(redblobBitmap, 16,0,16,16,width/2-10,height/2+15, 0);

    }
    
    void translateControls(){
        char c;
        for(int i=0; i<4; i++){
            switch (controls[i]) {
                case 27 ... 36: // key_number
                    controlsName[i] = "KEY " + std::to_string(controls[i]%27);
                    break;
                case 37 ... 46: // pad_number
                    controlsName[i] = "PAD " + std::to_string(controls[i]%37);
                    break;
                case 1 ... 26: // key_letter
                    c = (controls[i] + 64) + 'A';
                    controlsName[i] = "KEY " + std::string(1,char(controls[i] + 64));
                    break;
                case 82 ... 85: // flechas
                    // UP, DOWN, LEFT, RIGHT
                    if(controls[i]==84){
                        controlsName[i] = "UP";
                    }
                    else if(controls[i]==85){
                        controlsName[i] = "DOWN";
                    }
                    else if(controls[i]==82){
                        controlsName[i] = "LEFT";
                    }
                    else if(controls[i]==83){
                        controlsName[i] = "RIGHT";
                    }
                    break;
            }
        }
    }

    void resize(float _width, float _height){
        width = _width;
        height = _height;

        if(pant == INICIO){
            platillo.setX(width/2+platillo.getXRespectCube());
            platillo.setY(height/2+platillo.getYRespectCube());
            qbert.setY(platillo.getY()-6);
        }
        else if(pant == INSTRUCCIONES) { // INSTRUCCIONES
            qbert.setX(width/2-165+qbert.getXRespectCube());
            qbert.setY(height/2-140+qbert.getYRespectCube());
        }
        else {
            qbert.setX(width/2-155+qbert.getXRespectCube());
            qbert.setY(height/2-120+qbert.getYRespectCube());
        }
    }

    void destroy(){
        qbert.destroy();
        platillo.destroy();
        al_destroy_bitmap(fonts);
        al_destroy_sample(helloSound);
        al_destroy_bitmap(coilyBitmap);
        al_destroy_bitmap(samBitmap);
        al_destroy_bitmap(slickMap);
        al_destroy_bitmap(redblobBitmap);
    }

    void accionPantallas(int tecla){
        if(pant == MENU){ // Inicio del menu
            if(tecla == 59){ // ESCAPE
                escenarioInit();
            }
            else if(tecla == 67){ // ENTER
                switch (posMenu) {
                    case 0: // Instrucciones
                        escenarioInstruc();
                        break;
                    case 1: // Controles
                        escenarioControles();
                        break;
                    case 2:
                        escenarioDificultad();
                        break;
                }
            }
            else if(tecla == 84){ // up
                if(posMenu == 0) {
                    posMenu = 3;
                    qbert.setY(qbert.getY()+9*3*3);
                }
                else{
                    posMenu--;
                    qbert.setY(qbert.getY()-9*3);
                }
            }
            else if(tecla == 85){ // down
                if(posMenu == 3) {
                    posMenu = 0;
                    qbert.setY(height/2-120+qbert.getYRespectCube());
                }
                else{
                    posMenu++;
                    qbert.setY(qbert.getY()+9*3);
                }
            }
            else if(tecla == 82){ // left
                if(posMenu == 3){
                    if(level == 1) level = 9;
                    else level--;
                }
            }
            else if(tecla == 83){ // right
                if(posMenu == 3){
                    if(level == 9) level = 1;
                    else level++;
                }
            }
        }
        else if(pant == CONTROLES){ // Escape
            if(tecla == 59){ // Escape
                escenarioMenu();
            }
            else if(tecla == 67){ // ENTER
                if(contr == NONE) {
                    switch (posControles) {
                        // ARRIBADER, ABAJOIZQ, ARRIBAIZQ, ABAJODER
                        case 0: // ARRIBADER
                            contr = ARRIBADER;
                            break;
                        case 1: // ABAJOIZQ
                            contr = ABAJOIZQ;
                            break;
                        case 2: // ARRIBAIZQ
                            contr = ARRIBAIZQ;
                            break;
                        case 3: // ABAJODER
                            contr = ABAJODER;
                            break;
                    }
                }
                else { // Hay algo seleccionado --> se deselecciona
                    contr = NONE;
                }
            }
            else if(tecla == 84 && contr == NONE){ // up
                if(posControles == 0) {
                    posControles = 3;
                    qbert.setY(qbert.getY()+9*3*3);
                }
                else{
                    posControles--;
                    qbert.setY(qbert.getY()-9*3);
                }
            }
            else if(tecla == 85 && contr == NONE){ // down
                if(posControles == 3) {
                    posControles = 0;
                    qbert.setY(height/2-120+qbert.getYRespectCube());
                }
                else{
                    posControles++;
                    qbert.setY(qbert.getY()+9*3);
                }
            }
            else if(contr != NONE){
                switch (tecla) {
                    case 27 ... 36: // key_number
                    case 37 ... 46: // pad_number
                    case 1 ... 26: // key_letter
                    case 82 ... 85: // flechas
                        controls[posControles] = tecla;
                }
            }
            
        }
        else if(pant == DIFICULTAD){ // IA's
            if(tecla == 59){ // ESCAPE
                escenarioMenu();
            }
            else if(tecla == 84){ // up
                if(posDificultad == 0) {
                    posDificultad = 3;
                    qbert.setY(qbert.getY()+9*3*3);
                }
                else{
                    posDificultad--;
                    qbert.setY(qbert.getY()-9*3);
                }
            }
            else if(tecla == 85){ // down
                if(posDificultad == 3) {
                    posDificultad = 0;
                    qbert.setY(height/2-120+qbert.getYRespectCube());
                }
                else{
                    posDificultad++;
                    qbert.setY(qbert.getY()+9*3);
                }
            }
            else if(tecla == 82){ // left
                if(posDificultad == 0){ // TIMECUBO
                    if(cuboID == 0) cuboID = 3;
                    else cuboID--;
                }
                else if(posDificultad == 1) { // IACOILY
                    if(coilyID == 0) coilyID = 2;
                    else coilyID--;
                }
                else if(posDificultad == 2) { // IASLICKSAM
                    if(slicksamID == 0) slicksamID = 2;
                    else slicksamID--;
                }
                else if(posDificultad == 3) { // IABLOB
                    if(blobID == 0) blobID = 2;
                    else blobID--;
                }
            }
            else if(tecla == 83){ // right
                if(posDificultad == 0){ // TIMECUBO
                    if(cuboID == 3) cuboID = 0;
                    else cuboID++;
                }
                else if(posDificultad == 1) { // IACOILY
                    if(coilyID == 2) coilyID = 0;
                    else coilyID++;
                }
                else if(posDificultad == 2) { // IASLICKSAM
                    if(slicksamID == 2) slicksamID = 0;
                    else slicksamID++;
                }
                else if(posDificultad == 3) { // IABLOB
                    if(blobID == 2) blobID = 0;
                    else blobID++;
                }
            }
        }
        else if(pant = INSTRUCCIONES){
            if(tecla == 59){ // ESCAPE
                escenarioMenu();
            }
        }
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

#endif //QBERT_PANTALLAINICIAL_HPP
