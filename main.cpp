#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <list>
//#include <vector>
//#include <iostream>

#include "Piramide.hpp"
#include "QBert.hpp"
#include "Enemy.hpp"
#include "Coily.hpp"
#include "Platillo.hpp"


float scale = 1.0f;
float WIDTH = 640, HEIGHT = 480;

void checkRandMovementEnemies(std::list<Enemy> &enemies);
void movementEnemies(std::list<Enemy> &enemies, Piramide &piramide);
void resizeEnemies(std::list<Enemy> &enemies, Piramide &piramide);
void drawEnemies(std::list<Enemy> &enemies);
void destroyEnemies(std::list<Enemy> &enemies);

void must_init(bool test, const char *description);

int main() {
    must_init(al_init(), "allegro");
    must_init(al_install_keyboard(), "keyboard");

    ALLEGRO_TRANSFORM camera;

    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 30.0);
    must_init(timer, "timer");

    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    must_init(queue, "queue");

    al_set_new_display_flags(ALLEGRO_RESIZABLE);
    ALLEGRO_DISPLAY *disp = al_create_display(WIDTH, HEIGHT);
    al_set_window_title(disp, "Q*Bert");
    must_init(disp, "display");

    ALLEGRO_FONT *font = al_create_builtin_font();
    must_init(font, "font");

    ALLEGRO_KEYBOARD_STATE keyState;
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    must_init(al_init_image_addon(), "image addon");

    al_install_audio();
    al_init_acodec_addon();
    al_reserve_samples(10); // TODO: tener en cuenta a la hora de cuantos audios vamos a usar

    // TODO: crear el resto de .txt de todos los niveles y sus rondas
    // CARGAR MAPA
    Piramide piramide;
    piramide.loadMap(1, 4, WIDTH, HEIGHT);
    // END MAPA

    // CARGAR PERSONAJES
    QBert qbert = QBert(piramide);
    std::list <Enemy> enemies;
    Enemy redblob = Enemy(piramide, "Redblob", 1, 0, 9, 0); // X e Y (pixeles) posicion respecto al cubo[i,j]
    enemies.push_back(redblob);
    // FIN PERSONAJES

    // CARGAR COMPONENTES RESTANTES
    // TODO: guardarlo todos en una lista (como enemies)
    Platillo plato = Platillo(piramide, 1, DER);
    // platillos
    // letras
    // puntos
    // vidas
    // etc
    // FIN COMPONENTES RESTANTES



    bool done = false, redraw = true;
    ALLEGRO_EVENT event;
    al_start_timer(timer);

    /*************************
     *       GAME LOOP       *
     *************************/


    while (!done) {
        al_wait_for_event(queue, &event);
        al_get_keyboard_state(&keyState);

        switch (event.type) {
            case ALLEGRO_EVENT_TIMER:

                redraw = true;
                checkRandMovementEnemies(enemies);
                qbert.movement(&piramide, HEIGHT);
                movementEnemies(enemies, piramide);
                break;
                
            case ALLEGRO_EVENT_KEY_DOWN:

                if (!qbert.isJumping()) {
                    switch (event.keyboard.keycode) {
                        case ALLEGRO_KEY_LEFT:
                            qbert.setDir(TOPLEFT);
                            qbert.setI(qbert.getI()-1), qbert.setJ(qbert.getJ()-1);
                            break;
                        case ALLEGRO_KEY_RIGHT:
                            qbert.setDir(DOWNRIGHT);
                            qbert.setI(qbert.getI()+1), qbert.setJ(qbert.getJ()+1);
                            break;
                        case ALLEGRO_KEY_UP:
                            qbert.setDir(TOPRIGHT) ;
                            qbert.setI(qbert.getI()-1);
                            break;
                        case ALLEGRO_KEY_DOWN:
                            qbert.setDir(DOWNLEFT);
                            qbert.setI(qbert.getI()+1);
                            break;
                    }

                    qbert.setJumping(true);
                    qbert.setSourceX(qbert.getSourceX()+16);
                }
                break;

            case ALLEGRO_EVENT_DISPLAY_RESIZE:

                al_identity_transform(&camera);

                int prev_disp;
                prev_disp = al_get_display_width(disp);
                al_acknowledge_resize(disp);
                scale +=((float)al_get_display_width(disp) - (float)prev_disp)*0.001f;
                al_scale_transform(&camera, scale, scale);
                al_use_transform(&camera);

                WIDTH = al_get_display_width(disp);
                HEIGHT = al_get_display_height(disp);

                // TODO: RESIZE ALL ITEMS (+ para un futuro)
                piramide.resizeMap(WIDTH/scale, HEIGHT/scale);
                qbert.resize(piramide);
                resizeEnemies(enemies, piramide);
                plato.resize(piramide);

                break;

            case ALLEGRO_EVENT_DISPLAY_CLOSE:

                done = true;
                break;

        }
        if (redraw && al_is_event_queue_empty(queue)) {
            //REDRAW THE IMAGE WITH EVERYTHING
            al_clear_to_color(al_map_rgb(0, 0, 0));

            if(qbert.isFalling()) { // Si QBert esta cayendo primero se dibuja a QBert y luego la piramide
                qbert.drawBitmap();
                piramide.drawBitmap();
                // TODO: dibujar resto de componentes de la pantalla MENOS los enemigos
                // Recordar que mientras cae el resto de elementos dejan de moverse
                enemies.clear();
            } else {
                piramide.drawBitmap();
                qbert.drawBitmap();
                drawEnemies(enemies);
                // TODO: dibujar resto de componentes de la pantalla
                plato.drawBitmap();
            }

            al_flip_display();
            redraw = false;
        }
    }


    /*************************
     *     END GAME LOOP     *
     *************************/

    // DESTRUIR OBJETOS ESCENA
    piramide.destroy();
    qbert.destroy();
    destroyEnemies(enemies);
    // destroy resto elementos
    //**************************

    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}


void checkRandMovementEnemies(std::list<Enemy> &enemies) {
    for (std::_List_iterator<Enemy> it = enemies.begin(); it != enemies.end(); it++){
        if(it->getRandMoveTimer() == it->getRandMovePeriod()){
            it->randomMovement();
            it->resetRandomMoveTimer();
        }

        it->randomMoveTimerplusplus();
    }
}

void movementEnemies(std::list<Enemy> &enemies, Piramide &piramide) {
    for (std::_List_iterator<Enemy> it = enemies.begin(); it != enemies.end(); it++) {
        it->movement(&piramide, HEIGHT);
    }
}

void resizeEnemies(std::list<Enemy> &enemies, Piramide &piramide){
    for (std::_List_iterator<Enemy> it = enemies.begin(); it != enemies.end(); it++) {
        it->resize(piramide);
    }
}

void drawEnemies(std::list<Enemy> &enemies){
    for (std::_List_iterator<Enemy> it = enemies.begin(); it != enemies.end(); it++){
        it->drawBitmap();
    }
}

void destroyEnemies(std::list<Enemy> &enemies){
    for (std::_List_iterator<Enemy> it = enemies.begin(); it != enemies.end(); it++){
        it->destroy();
    }
}

void must_init(bool test, const char *description) {
    if (test) return;

    printf("No se ha podido inicializar: %s\n", description);
    exit(1);
}