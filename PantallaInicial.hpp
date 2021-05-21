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
        INICIO, INSTRUCCIONES, MENU, CONTROLES
    };

    enum Menu {
        DEFAULT, TIMECUBO, IACOILY, IABLOB, IASLICKSAM, LEVEL
    };
    
    enum Controls {
        NONE, ARRIBADER, ARRIBAIZQ, ABAJODER, ABAJOIZQ
    };

    float width, height;
    int timer = 0;

    ALLEGRO_BITMAP *drawInit = al_load_bitmap("../sprites/fonts.png");
    ALLEGRO_SAMPLE *helloSound = al_load_sample("../sounds/qbert-hello.ogg");
    QBert qbert = QBert();
    Platillo platillo = Platillo(width, height);
    int posMenu, posControles;

public:
    int cuboID, coilyID, slicksamID, blobID, level;
    std::string cubos[4] = {"OFF", "15 SEG", "30 SEG", "45 SEG"},
                coily[3] = {"NORMAL", "MEDIA", "AVANZADA"},
                slicksam[3] = {"NORMAL", "MEDIA", "AVANZADA"},
                blob[3] = {"NORMAL", "MEDIA", "AVANZADA"};
    int controls[4];
    std::string controlsName[4];
    Controls contr;

    Pantalla pant = INICIO;
    Menu menu;

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
        menu = DEFAULT;
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

        x = width/2-64, y += 8*2;
        frase = "PRESS F1 TO MENU";
        for (std::string::size_type i = 0; i < frase.size(); i++) {
            if (int(frase[i]) >= 48 && int(frase[i]) <= 57) { // Numero
                al_draw_bitmap_region(drawInit, (int(frase[i]) % 48) * 8 + 9*9, 0,
                                      8, 8, x, y, 0);
            } else if(int(frase[i]) != 32){ // En caso de ser un espacio no se dibuja
                al_draw_bitmap_region(drawInit, (int(frase[i]) % 65) * 8, 8,
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
        al_draw_bitmap_region(drawInit, 0, 4 * 8,
                              12*8, 2*8, x, y, 0);

        std::string frase;

        x = width / 2 + 8*4;
        frase = "ESC PARA VOLVER";                                                                                                                                                                             "EMPEZAR EN NIVEL     -" + std::to_string(level) + "-\\";
        for (std::string::size_type i = 0; i < frase.size(); i++) {
            if (int(frase[i]) != 32) { // En caso de ser un espacio no se dibuja
                al_draw_bitmap_region(drawInit, (int(frase[i]) % 65) * 8, 8,
                                      8, 8, x, y, 0);
            }
            x += 8;
        }

        x = width / 2 - 8*12, y = height/2 - 8*8-2;
        int naranja = 0;
        frase = "INSTRUCCIONES\\"
                "CONTROLES\\"
                "CAMBIO COLOR CUBOS   -" + cubos[cuboID] + "-\\"
                "IA COILY             -" + coily[coilyID] + "-\\"
                "IA SLICK-SAM         -" + slicksam[slicksamID] + "-\\"
                "IA BLOB ROJO         -" + blob[blobID] + "-\\"
                "EMPEZAR EN NIVEL     -" + std::to_string(level) + "-\\";
        for (std::string::size_type i = 0; i < frase.size(); i++) {
            if (int(frase[i]) == 92) { // '\' --> \n
                x = width / 2 - 8*12-8;
                y += 9*3;
                naranja++;
            }
            else if(int(frase[i]) == 45){ // '-'
                if(posMenu == naranja && menu != DEFAULT) {
                    al_draw_bitmap_region(drawInit, 10 * 8 + 9 * 9, 8 * 5,
                                          8, 8, x, y, 0);
                }
                else {
                    al_draw_bitmap_region(drawInit, 10 * 8 + 9 * 9, 8 * 4,
                                          8, 8, x, y, 0);
                }
            }
            else if (int(frase[i]) >= 48 && int(frase[i]) <= 57) { // Numero
                if(posMenu == naranja && menu != DEFAULT) {
                    al_draw_bitmap_region(drawInit, (int(frase[i]) % 48) * 8 + 9 * 9, 8*2,
                                          8, 8, x, y, 0);
                }
                else {
                    al_draw_bitmap_region(drawInit, (int(frase[i]) % 48) * 8 + 9 * 9, 0,
                                          8, 8, x, y, 0);
                }
            }
            else if (int(frase[i]) != 32) { // En caso de ser un espacio no se dibuja
                if(posMenu == naranja && menu != DEFAULT) {
                    al_draw_bitmap_region(drawInit, (int(frase[i]) % 65) * 8, 8*3,
                                          8, 8, x, y, 0);
                }
                else {
                    al_draw_bitmap_region(drawInit, (int(frase[i]) % 65) * 8, 8,
                                          8, 8, x, y, 0);
                }
            }
            x += 8;
        }

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
    }

    void pantallaControles(){
        float x = width/2-165+20, y = height/2-140+20;
        // Cargar logo Q*Bert
        al_draw_bitmap_region(drawInit, 0, 4 * 8,
                              12*8, 2*8, x, y, 0);

        std::string frase;

        x = width / 2 + 8*4;
        frase = "ESC PARA VOLVER";                                                                                                                                                                             "EMPEZAR EN NIVEL     -" + std::to_string(level) + "-\\";
        for (std::string::size_type i = 0; i < frase.size(); i++) {
            if (int(frase[i]) != 32) { // En caso de ser un espacio no se dibuja
                al_draw_bitmap_region(drawInit, (int(frase[i]) % 65) * 8, 8,
                                      8, 8, x, y, 0);
            }
            x += 8;
        }

        x = width / 2 - 8*12, y = height/2 - 8*8-2;
        translateControls();
        int naranja = 0;
        // ARRIBADER, ABAJOIZQ, ARRIBAIZQ, ABAJODER
        frase = "SALTO ARRIBA DERECHA   -" + controlsName[0] + "-\\"   //UP
                "SALTO ABAJO IZQUIERDA  -" + controlsName[1] + "-\\"   //DOWN
                "SALTO ARRIBA IZQUIERDA -" + controlsName[2] + "-\\"   //left
                "SALTO ABAJO DERECHA    -" + controlsName[3] + "-\\";  //right                                                                                                                                                                                 "EMPEZAR EN NIVEL     -" + std::to_string(level) + "-\\";
        for (std::string::size_type i = 0; i < frase.size(); i++) {
            if (int(frase[i]) == 92) { // '\' --> \n
                x = width / 2 - 8*12-8;
                y += 9*3;
                naranja++;
            }
            else if(int(frase[i]) == 45){ // '-'
                if(posControles == naranja && contr != NONE) {
                    al_draw_bitmap_region(drawInit, 10 * 8 + 9 * 9, 8 * 5,
                                          8, 8, x, y, 0);
                }
                else {
                    al_draw_bitmap_region(drawInit, 10 * 8 + 9 * 9, 8 * 4,
                                          8, 8, x, y, 0);
                }
            }
            else if (int(frase[i]) >= 48 && int(frase[i]) <= 57) { // Numero
                if(posControles == naranja && contr != NONE) {
                    al_draw_bitmap_region(drawInit, (int(frase[i]) % 48) * 8 + 9 * 9, 8*2,
                                          8, 8, x, y, 0);
                }
                else {
                    al_draw_bitmap_region(drawInit, (int(frase[i]) % 48) * 8 + 9 * 9, 0,
                                          8, 8, x, y, 0);
                }
            }
            else if (int(frase[i]) != 32) { // En caso de ser un espacio no se dibuja
                if(posControles == naranja && contr != NONE) {
                    al_draw_bitmap_region(drawInit, (int(frase[i]) % 65) * 8, 8*3,
                                          8, 8, x, y, 0);
                }
                else {
                    al_draw_bitmap_region(drawInit, (int(frase[i]) % 65) * 8, 8,
                                          8, 8, x, y, 0);
                }
            }
            x += 8;
        }

        qbert.drawBitmap();
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
        } else if(pant == INSTRUCCIONES) { // INSTRUCCIONES
            qbert.setX(width/2-165+qbert.getXRespectCube());
            qbert.setY(height/2-140+qbert.getYRespectCube());
        }
        else if(pant == MENU) {
            // TODO quitar?
//            qbert.setX(width/2-165+qbert.getXRespectCube());
//            qbert.setY(height/2-140+qbert.getYRespectCube());
        }
    }

    void destroy(){
        qbert.destroy();
        platillo.destroy();
        al_destroy_bitmap(drawInit);
        al_destroy_sample(helloSound);
    }

    void accionMenu(int tecla){
        if(pant == MENU){ // Inicio del menu
            if(tecla == 59){ // ESCAPE
                escenarioInit();
            }
            else if(tecla == 67){ // ENTER
                if(menu == DEFAULT) {
                    switch (posMenu) {
                        case 0: // Instrucciones
                            escenarioInstruc();
                            break;
                        case 1: // Controles
                            escenarioControles();
                            break;
                        case 2: // Time reverse cube color
                            menu = TIMECUBO;
                            break;
                        case 3: // IA Coily
                            menu = IACOILY;
                            break;
                        case 4: // IA Slick y Sam
                            menu = IASLICKSAM;
                            break;
                        case 5: // IA Blob rojo
                            menu = IABLOB;
                            break;
                        case 6: // backdoor
                            menu = LEVEL;
                            break;
                    }
                }
                else { // Hay algo seleccionado --> se deselecciona
                    menu = DEFAULT;
                }
            }
            else if(tecla == 84 && menu == DEFAULT){ // up
                if(posMenu == 0) {
                    posMenu = 6;
                    qbert.setY(qbert.getY()+9*3*6);
                }
                else{
                    posMenu--;
                    qbert.setY(qbert.getY()-9*3);
                }
            }
            else if(tecla == 85 && menu == DEFAULT){ // down
                if(posMenu == 6) {
                    posMenu = 0;
                    qbert.setY(height/2-120+qbert.getYRespectCube());
                }
                else{
                    posMenu++;
                    qbert.setY(qbert.getY()+9*3);
                }
            }
            else if(tecla == 82 && menu != DEFAULT){ // left
                if(menu == TIMECUBO){
                    if(cuboID == 0) cuboID = 3;
                    else cuboID--;
                }
                else if(menu == IACOILY) {
                    if(coilyID == 0) coilyID = 2;
                    else coilyID--;
                }
                else if(menu == IASLICKSAM) {
                    if(slicksamID == 0) slicksamID = 2;
                    else slicksamID--;
                }
                else if(menu == IABLOB) {
                    if(blobID == 0) blobID = 2;
                    else blobID--;
                }
                else if(menu == LEVEL){
                    if(level == 1) level = 9;
                    else level--;
                }
            }
            else if(tecla == 83 && menu != DEFAULT){ // right
                if(menu == TIMECUBO){
                    if(cuboID == 3) cuboID = 0;
                    else cuboID++;
                }
                else if(menu == IACOILY) {
                    if(coilyID == 2) coilyID = 0;
                    else coilyID++;
                }
                else if(menu == IASLICKSAM) {
                    if(slicksamID == 2) slicksamID = 0;
                    else slicksamID++;
                }
                else if(menu == IABLOB) {
                    if(blobID == 2) blobID = 0;
                    else blobID++;
                }
                else if(menu == LEVEL){
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
    }

};

#endif //QBERT_PANTALLAINICIAL_HPP
