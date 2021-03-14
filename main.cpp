#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <vector>
#include <iostream>

#include "Piramide.hpp"
#include "Character.hpp"
#include "Player.hpp"
#include "Enemy.hpp"
#include "Coily.hpp"

/* Q*BERT */
#define framePixels 16
#define airTime 6
#define movementX 4.0f
#define movementY 12.0f

float scale = 1.0f;

void must_init(bool test, const char *description) {
    if (test) return;

    printf("couldn't initialize %s\n", description);
    exit(1);
}

void destroy_all();

void movement(Player &qbert, Piramide &piramide, ALLEGRO_SAMPLE *jump, int &airTimer);
void movement2(Enemy &blob, Piramide &piramide, ALLEGRO_SAMPLE *jump, int &airTimer);


int main() {
    must_init(al_init(), "allegro");
    must_init(al_install_keyboard(), "keyboard");

    ALLEGRO_TRANSFORM camera;

    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 30.0);
    must_init(timer, "timer");

    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    must_init(queue, "queue");

    al_set_new_display_flags(ALLEGRO_RESIZABLE);
    float WIDTH = 640, HEIGHT = 480;
    ALLEGRO_DISPLAY *disp = al_create_display(WIDTH, HEIGHT);
    al_set_window_title(disp, "Q*Bert");
    int prev_disp[2];
    must_init(disp, "display");

    ALLEGRO_FONT *font = al_create_builtin_font();
    must_init(font, "font");

    ALLEGRO_KEYBOARD_STATE keyState;

    must_init(al_init_image_addon(), "image addon");
    ALLEGRO_BITMAP *player = al_load_bitmap("../sprites/qbert.png");
    ALLEGRO_BITMAP *Redblob = al_load_bitmap("../sprites/Redblob.png");
    must_init(player, "player");
    must_init(Redblob, "redblob");
    Player qbert = Player(WIDTH/2-6, 144-8, player, 0, 0, DOWNRIGHT);
    Enemy redblob = Enemy(WIDTH/2-6, 144-8, Redblob, 0, 0, DOWNRIGHT);

    int sourceY = 2;

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    // TODO: crear el resto de .txt de todos los niveles y sus rondas
    // CARGAR MAPA
    Piramide piramide;
    piramide.loadMap(2, 4, WIDTH, HEIGHT);
    // END MAPA

    // CARGAR SONIDO
    al_install_audio();
    al_init_acodec_addon();
    al_reserve_samples(10);
    ALLEGRO_SAMPLE *jump = NULL;
    jump = al_load_sample("../sounds/jump-3.ogg");

    std::vector<int> source;
    std::vector<int> width;

    ALLEGRO_COLOR pixel, lastPixel, separatorColor;

    source.push_back(0);

    for (int i = 0; i < al_get_bitmap_width(qbert.getDraw()); i++) {
        separatorColor = al_map_rgba(0, 0, 255, 255);
        pixel = al_get_pixel(qbert.getDraw(), i, 0);

        if (memcmp(&pixel, &lastPixel, sizeof(ALLEGRO_COLOR))) {
            if (!memcmp(&pixel, &separatorColor, sizeof(ALLEGRO_COLOR))) {
                source.push_back(i);
                if (source.size() == 2)
                    width.push_back(i);
                else
                    width.push_back(i - width[width.size() - 1]);
            }
        } else if (i == al_get_bitmap_width(qbert.getDraw()) - 1) {
            width.push_back(i - width[width.size() - 1]);
            lastPixel = pixel;
        }
    }

    bool done = false, redraw = true;
    int airTimer = 0;
    int enemyTimer = 0;
    ALLEGRO_EVENT event;

    //GAME LOOP

    al_start_timer(timer);
    while (!done) {
        al_wait_for_event(queue, &event);
        al_get_keyboard_state(&keyState);

        switch (event.type) {
            case ALLEGRO_EVENT_TIMER:

                redraw = true;
                if(enemyTimer % 120 == 0){
                    redblob.randomMovement();
                }
                movement(qbert, piramide, jump, airTimer);
                movement2(redblob, piramide, jump, airTimer);
                enemyTimer++;
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

                std::cout << WIDTH << " + " << al_get_display_width(disp) << std::endl;

                WIDTH = al_get_display_width(disp);
                HEIGHT = al_get_display_height(disp);

                piramide.resizeMap(WIDTH/scale, HEIGHT/scale);
                // TODO: establecer bien la posicion de Q*Bert (y futuros objetos) despues del resize

                break;

            case ALLEGRO_EVENT_DISPLAY_CLOSE:

                done = true;
                break;

        }
        if (redraw && al_is_event_queue_empty(queue)) {
            //REDRAW THE IMAGE WITH EVERYTHING
            al_clear_to_color(al_map_rgb(0, 0, 0));
            piramide.drawMap();
            al_draw_bitmap_region(qbert.getDraw(), qbert.getSourceX() + (qbert.getDir() * 2 * framePixels), 0, framePixels, framePixels,
                                  qbert.getX(), qbert.getY(), 0);

            al_draw_bitmap_region(redblob.getDraw(), redblob.getSourceX(), 0, framePixels, framePixels,
                                  redblob.getX(), redblob.getY(), 0);
            al_flip_display();

            redraw = false;
        }
    }

    piramide.destroy();
    al_destroy_sample(jump);
    al_destroy_bitmap(qbert.getDraw());
    al_destroy_bitmap(redblob.getDraw());
    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}

void movement(Player &qbert, Piramide &piramide, ALLEGRO_SAMPLE *jump, int &airTimer) {
    if (qbert.isJumping()) {
        airTimer++;
        if(airTimer < airTime/2){
            //GO UP AND DIRECTION
            if(qbert.getDir() == TOPRIGHT || qbert.getDir() == DOWNRIGHT)
                qbert.setX(movementX + qbert.getX());
            else
                qbert.setX(qbert.getX() - movementX);
            if(qbert.getDir() != DOWNRIGHT && qbert.getDir() != DOWNLEFT)
                qbert.setY(qbert.getY()-movementY);
        }else if (airTimer > airTime/2 && airTimer < airTime){
            //GO DOWN AND DIRECTION
            if(qbert.getDir() == TOPRIGHT || qbert.getDir() == DOWNRIGHT)
                qbert.setX(movementX + qbert.getX());
            else
                qbert.setX(qbert.getX() - movementX);
            if(qbert.getDir() == DOWNRIGHT || qbert.getDir() == DOWNLEFT)
                qbert.setY(qbert.getY() + movementY);
        }else if (airTimer > airTime) {
            //WE LANDED
            al_play_sample(jump, 1.0, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
            piramide.changeCube(qbert.getI(), qbert.getJ());

            airTimer = 0;
            qbert.setJumping(false);
            qbert.setSourceX(qbert.getSourceX() - 16);
        }
    }
}

void movement2(Enemy &blob, Piramide &piramide, ALLEGRO_SAMPLE *jump, int &airTimer) {
    if (blob.isJumping()) {
        airTimer++;
        if(airTimer < airTime/2){
            //GO UP AND DIRECTION
            if(blob.getDir() == TOPRIGHT || blob.getDir() == DOWNRIGHT)
                blob.setX(movementX + blob.getX());
            else
                blob.setX(blob.getX() - movementX);
            if(blob.getDir() != DOWNRIGHT && blob.getDir() != DOWNLEFT)
                blob.setY(blob.getY()-movementY);
        }else if (airTimer > airTime/2 && airTimer < airTime){
            //GO DOWN AND DIRECTION
            if(blob.getDir() == TOPRIGHT || blob.getDir() == DOWNRIGHT)
                blob.setX(movementX + blob.getX());
            else
                blob.setX(blob.getX() - movementX);
            if(blob.getDir() == DOWNRIGHT || blob.getDir() == DOWNLEFT)
                blob.setY(blob.getY() + movementY);
        }else if (airTimer > airTime) {
            //WE LANDED
            al_play_sample(jump, 1.0, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
            piramide.changeCube(blob.getI(), blob.getJ());

            airTimer = 0;
            blob.setJumping(false);
            blob.setSourceX(blob.getSourceX() + 16);
        }
    }
}


void destroy_all(){

}