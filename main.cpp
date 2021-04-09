#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <vector>
#include <iostream>
#include <list>

#include "Piramide.hpp"
#include "Character.hpp"
#include "QBert.hpp"
#include "Enemy.hpp"
#include "Coily.hpp"

/* Q*BERT */

#define framePixels 16
#define airTime 6
#define movementX 4.0f
#define movementY 12.0f

// airtime 6 --> se ejecuta 4 veces en X (16 pixeles mas en x) y 2 (24 pixeles mas en y)
// airtime 10 --> en x 8 y 4 en y
// airtime 12 --> 10 y 7
/*#define framePixels 16
#define airTime 16
#define movementX 2.0f // pixelesX/nºejecucionesX
#define movementY 3.42f // pixelesY/nºejecucionesY*/

float scale = 1.0f;
float WIDTH = 640, HEIGHT = 480;

void must_init(bool test, const char *description) {
    if (test) return;

    printf("couldn't initialize %s\n", description);
    exit(1);
}

void checkRandMovementEnemies(std::list<Enemy> &enemies);

void movementPlayer(QBert &qbert, Piramide &piramide, ALLEGRO_SAMPLE *jumpSound, ALLEGRO_SAMPLE *qbertFallingSound);
void movementEnemies(std::list<Enemy> &enemies, Piramide &piramide, ALLEGRO_SAMPLE *jumpSound);
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
    int prev_disp[2];

    ALLEGRO_FONT *font = al_create_builtin_font();
    must_init(font, "font");

    ALLEGRO_KEYBOARD_STATE keyState;
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    must_init(al_init_image_addon(), "image addon");

    // TODO: crear el resto de .txt de todos los niveles y sus rondas
    // CARGAR MAPA
    Piramide piramide;
    piramide.loadMap(1, 4, WIDTH, HEIGHT);
    // END MAPA

    // CARGAR PERSONAJES
    QBert qbert = QBert(piramide);
    std::list <Enemy> enemies;
    Enemy redblob = Enemy(piramide, "Redblob", 1, 0, 9, 0); // X e Y posicion respecto al cubo[i,j]
    enemies.push_back(redblob);
    // FIN PERSONAJES

    // CARGAR SONIDO
    al_install_audio();
    al_init_acodec_addon();
    al_reserve_samples(10);

    ALLEGRO_SAMPLE *jumpSound = al_load_sample("../sounds/jump-3.ogg");

    ALLEGRO_SAMPLE *playerJumpSound = al_load_sample("../sounds/jump.ogg");
    must_init(playerJumpSound, "playerJumpSound");

    ALLEGRO_SAMPLE *qbertFallingSound = al_load_sample("../sounds/qbert-falling.ogg");
    must_init(qbertFallingSound, "qbertFallingSound");
    // FIN SONIDO

    qbert.managementSpriteQbert();

    bool done = false, redraw = true;
    ALLEGRO_EVENT event;

    //GAME LOOP

    al_start_timer(timer);
    while (!done) {
        al_wait_for_event(queue, &event);
        al_get_keyboard_state(&keyState);

        switch (event.type) {
            case ALLEGRO_EVENT_TIMER:

                redraw = true;
                checkRandMovementEnemies(enemies);
                movementPlayer(qbert, piramide, playerJumpSound, qbertFallingSound);
                movementEnemies(enemies, piramide, jumpSound);
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

                prev_disp[0] = al_get_display_width(disp);
                al_acknowledge_resize(disp);
                scale +=((float)al_get_display_width(disp) - (float)prev_disp[0])*0.001f;
                al_scale_transform(&camera, scale, scale);
                al_use_transform(&camera);

                WIDTH = al_get_display_width(disp);
                HEIGHT = al_get_display_height(disp);

                // Resize all
                piramide.resizeMap(WIDTH/scale, HEIGHT/scale);
                qbert.resize(&piramide);
                redblob.resize(&piramide);

                break;

            case ALLEGRO_EVENT_DISPLAY_CLOSE:

                done = true;
                break;

        }
        if (redraw && al_is_event_queue_empty(queue)) {
            //REDRAW THE IMAGE WITH EVERYTHING
            al_clear_to_color(al_map_rgb(0, 0, 0));
            if(qbert.isFalling()) { // Si QBert esta cayendo primero se dibuja la piramide y luego a QBert
                al_draw_bitmap_region(qbert.getDraw(), qbert.getSourceX() + (qbert.getDir() * 2 * framePixels), 0,
                                      framePixels, framePixels, qbert.getX(), qbert.getY(), 0);
                piramide.drawMap();
            } else {
                piramide.drawMap();
                al_draw_bitmap_region(qbert.getDraw(), qbert.getSourceX() + (qbert.getDir() * 2 * framePixels), 0,
                                      framePixels, framePixels, qbert.getX(), qbert.getY(), 0);
                al_draw_bitmap_region(redblob.getDraw(), redblob.getSourceX(), 0, framePixels, framePixels,
                                      redblob.getX(), redblob.getY(), 0);
            }

            al_flip_display();

            redraw = false;
        }
    }

    piramide.destroy();
    al_destroy_sample(playerJumpSound);
    al_destroy_sample(qbertFallingSound);
    al_destroy_sample(jumpSound);
    al_destroy_bitmap(qbert.getDraw());
    al_destroy_bitmap(redblob.getDraw());
    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}

void checkRandMovementEnemies(std::list<Enemy> &enemies) {

    for (std::_List_iterator<Enemy> it = enemies.begin(); it != enemies.end(); it++)
    {
        if(it->getRandMoveTimer() == it->getRandMovePeriod()){
            it->randomMovement();
            it->resetRandomMoveTimer();
        }

        it->randomMoveTimerplusplus();

    }
}

void movementPlayer(QBert &qbert, Piramide &piramide, ALLEGRO_SAMPLE *playerJumpSound, ALLEGRO_SAMPLE *qbertFallingSound) {
    if (qbert.isJumping()) {
        qbert.airTimerplusplus();
        if(!qbert.isFalling()) {
            if (qbert.getAirTimer() < airTime / 2) {
                //GO UP AND DIRECTION
                if (qbert.getDir() == TOPRIGHT || qbert.getDir() == DOWNRIGHT)
                    qbert.setX(movementX + qbert.getX());
                else
                    qbert.setX(qbert.getX() - movementX);
                if (qbert.getDir() != DOWNRIGHT && qbert.getDir() != DOWNLEFT)
                    qbert.setY(qbert.getY() - movementY);
            } else if (qbert.getAirTimer() > airTime / 2 && qbert.getAirTimer() < airTime) {
                //GO DOWN AND DIRECTION
                if (qbert.getDir() == TOPRIGHT || qbert.getDir() == DOWNRIGHT)
                    qbert.setX(movementX + qbert.getX());
                else
                    qbert.setX(qbert.getX() - movementX);
                if (qbert.getDir() == DOWNRIGHT || qbert.getDir() == DOWNLEFT)
                    qbert.setY(qbert.getY() + movementY);
            } else if (qbert.getAirTimer() > airTime) {
                if(qbert.getJ()<0 || qbert.getJ() > qbert.getI() || qbert.getI()>=7){
                    // TODO: Si no hay platillo Q*Bert cae al vacio y pierde 1 vida
                    qbert.setFalling(true);
                    // Reproducir sonido caida
                    al_play_sample(qbertFallingSound, 1.0, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
                } else {
                    //WE LANDED
                    al_play_sample(playerJumpSound, 1.0, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
                    piramide.changeCube(qbert.getI(), qbert.getJ());

                    //qbert.setX(piramide.map[qbert.getI()][qbert.getJ()].x+qbert.getXRespectCube());
                    //qbert.setY(piramide.map[qbert.getI()][qbert.getJ()].y+qbert.getYRespectCube());
                    qbert.setAirTimer(0);
                    qbert.setJumping(false);
                    qbert.setSourceX(qbert.getSourceX() - 16);
                }
            }
        } else { // Esta cayendo
            if(qbert.getY() <= HEIGHT){
                qbert.setY(qbert.getY() + movementY);
            } else {
                qbert.reset(&piramide);
            }
        }
    }
}

void movementEnemies(std::list<Enemy> &enemies, Piramide &piramide, ALLEGRO_SAMPLE *jumpSound) {
    for (std::_List_iterator<Enemy> it = enemies.begin(); it != enemies.end(); it++) {
        if (it->isJumping()) {
            it->airTimerplusplus();
            if (it->getAirTimer() < airTime / 2) {
                //GO UP AND DIRECTION
                if (it->getDir() == TOPRIGHT || it->getDir() == DOWNRIGHT)
                    it->setX(movementX + it->getX());
                else
                    it->setX(it->getX() - movementX);
                if (it->getDir() != DOWNRIGHT && it->getDir() != DOWNLEFT)
                    it->setY(it->getY() - movementY);
            } else if (it->getAirTimer() > airTime / 2 && it->getAirTimer() < airTime) {
                //GO DOWN AND DIRECTION
                if (it->getDir() == TOPRIGHT || it->getDir() == DOWNRIGHT)
                    it->setX(movementX + it->getX());
                else
                    it->setX(it->getX() - movementX);
                if (it->getDir() == DOWNRIGHT || it->getDir() == DOWNLEFT)
                    it->setY(it->getY() + movementY);
            } else if (it->getAirTimer() > airTime) {
                //WE LANDED
                al_play_sample(jumpSound, 1.0, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
                if (it->hasChangingGroundPower()) {
                    piramide.changeCube(it->getI(), it->getJ());
                }
                it->setX(piramide.map[it->getI()][it->getJ()].x + it->getXRespectCube());
                it->setY(piramide.map[it->getI()][it->getJ()].y + it->getYRespectCube());
                it->setAirTimer(0);
                it->setJumping(false);
                it->assignIJ();
                it->setSourceX(it->getSourceX() + 16);
            }
        }
    }
}