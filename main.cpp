#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
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

enum Direction {
    TOPRIGHT, TOPLEFT, DOWNRIGHT, DOWNLEFT
};

struct Character {
    float x;
    float y;
    Direction dir;
    ALLEGRO_BITMAP *draw;
    int i, j; // coordenada cubo
};

float scale = 1.0f;



void must_init(bool test, const char *description) {
    if (test) return;

    printf("couldn't initialize %s\n", description);
    exit(1);
}

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
    must_init(player, "player");
    Character qbert{WIDTH/2-6, 100-8, DOWNRIGHT, player, 0,0};
    int sourceX = 0, sourceY = 2;

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    // CARGAR MAPA
    Piramide piramide;
    piramide.loadMap(1, 1, WIDTH, HEIGHT);
    // END MAPA

    std::vector<int> source;
    std::vector<int> width;

    ALLEGRO_COLOR pixel, lastPixel, separatorColor;

    source.push_back(0);

    for (int i = 0; i < al_get_bitmap_width(qbert.draw); i++) {
        separatorColor = al_map_rgba(0, 0, 255, 255);
        pixel = al_get_pixel(qbert.draw, i, 0);

        if (memcmp(&pixel, &lastPixel, sizeof(ALLEGRO_COLOR))) {
            if (!memcmp(&pixel, &separatorColor, sizeof(ALLEGRO_COLOR))) {
                source.push_back(i);
                if (source.size() == 2)
                    width.push_back(i);
                else
                    width.push_back(i - width[width.size() - 1]);
            }
        } else if (i == al_get_bitmap_width(qbert.draw) - 1) {
            width.push_back(i - width[width.size() - 1]);
            lastPixel = pixel;
        }
    }

    bool done = false, redraw = true, jumping = false;
    int airTimer = 0;
    ALLEGRO_EVENT event;

    //GAME LOOP

    al_start_timer(timer);
    while (!done) {
        al_wait_for_event(queue, &event);
        al_get_keyboard_state(&keyState);

        switch (event.type) {
            case ALLEGRO_EVENT_TIMER:
                redraw = true;
                if (jumping) {
                    airTimer++;
                    if(airTimer < airTime/2){
                        //GO UP AND DIRECTION
                        if(qbert.dir == TOPRIGHT || qbert.dir == DOWNRIGHT)
                            qbert.x += movementX;
                        else
                            qbert.x -= movementX;
                        if(qbert.dir != DOWNRIGHT && qbert.dir != DOWNLEFT)
                            qbert.y -= movementY;
                    }else if (airTimer > airTime/2 && airTimer < airTime){
                        //GO DOWN AND DIRECTION
                        if(qbert.dir == TOPRIGHT || qbert.dir == DOWNRIGHT)
                            qbert.x += movementX;
                        else
                            qbert.x -= movementX;
                        if(qbert.dir == DOWNRIGHT || qbert.dir == DOWNLEFT)
                            qbert.y += movementY;
                    }else if (airTimer > airTime) {
                        //WE LANDED
                        piramide.changeCube(qbert.i, qbert.j);
                        airTimer = 0;
                        jumping = false;
                        sourceX -= 16;
                    }
                }
                break;

            case ALLEGRO_EVENT_KEY_DOWN:
                if (!jumping) {
                    switch (event.keyboard.keycode) {
                        case ALLEGRO_KEY_LEFT:
                            qbert.dir = TOPLEFT;
                            qbert.i--, qbert.j--;
                            break;
                        case ALLEGRO_KEY_RIGHT:
                            qbert.dir = DOWNRIGHT;
                            qbert.i++, qbert.j++;
                            break;
                        case ALLEGRO_KEY_UP:
                            qbert.dir = TOPRIGHT;
                            qbert.i--;
                            break;
                        case ALLEGRO_KEY_DOWN:
                            qbert.dir = DOWNLEFT;
                            qbert.i++;
                            break;
                    }
                    jumping = true;
                    sourceX += 16;
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

                piramide.resizeMap(WIDTH, HEIGHT);

                break;

            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                done = true;
                break;

        }
        if (redraw && al_is_event_queue_empty(queue)) {
            //REDRAW THE IMAGE WITH EVERYTHING
            al_clear_to_color(al_map_rgb(0, 0, 0));

            //TODO MAP FUNCTION OR SOMETHING

            piramide.drawMap();
            al_draw_bitmap_region(qbert.draw, sourceX + (qbert.dir * 2 * framePixels), 0, framePixels, framePixels,
                                  qbert.x, qbert.y, 0);


            al_flip_display();

            redraw = false;
        }
    }

    al_destroy_bitmap(qbert.draw);
    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}