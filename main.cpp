#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_primitives.h>
#include <list>
#include <fstream>
#include <iostream>

#include "Piramide.hpp"
#include "Enemy.hpp"
#include "Coily.hpp"
#include "PantallaInicial.hpp"
#include "PantallaInfoNivel.hpp"
#include "Escena.hpp"
#include "Creditos.hpp"

#define NUMERODESAMPLES 10

float scale;
float WIDTH, HEIGHT;

int posPause;

void must_init(bool test, const char *description);
void guardarResize();

int main() {
    must_init(al_init(), "allegro");
    must_init(al_install_keyboard(), "keyboard");

    // Memoria a la hora del reescalado
    std::ifstream file("../pantalla.txt");
    file >> WIDTH >> HEIGHT >> scale;
    file.close();

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
    al_reserve_samples(NUMERODESAMPLES);

    ALLEGRO_SAMPLE *intro = al_load_sample("../sounds/intro.ogg");
    ALLEGRO_SAMPLE_INSTANCE *introInstance = al_create_sample_instance(intro);
    al_set_sample_instance_playmode(introInstance, ALLEGRO_PLAYMODE_LOOP);
    al_attach_sample_instance_to_mixer(introInstance, al_get_default_mixer());
    ALLEGRO_SAMPLE *juego = al_load_sample("../sounds/juego.ogg");
    ALLEGRO_SAMPLE_INSTANCE *juegoInstance = al_create_sample_instance(juego);
    al_set_sample_instance_playmode(juegoInstance, ALLEGRO_PLAYMODE_LOOP);
    al_attach_sample_instance_to_mixer(juegoInstance, al_get_default_mixer());

    al_identity_transform(&camera);

    al_acknowledge_resize(disp);

    al_resize_display(disp, WIDTH, HEIGHT);

    al_scale_transform(&camera, scale, scale);
    al_use_transform(&camera);

    ALLEGRO_BITMAP *pauseBitmap = al_load_bitmap("../sprites/escape.png");
    ALLEGRO_BITMAP *fonts = al_load_bitmap("../sprites/fonts.png");

    Creditos credit = Creditos();
    Escena escena = Escena(WIDTH/scale, HEIGHT/scale);
    bool redraw = true;
    ALLEGRO_EVENT event;
    al_start_timer(timer);

    /*************************
     *       GAME LOOP       *
     *************************/

    int cuboID = 0, coilyID = 0, slicksamID = 0, blobID = 0, level = 1;
    // UP, DOWN, LEFT, RIGHT (inicialmente)
    // ARRIBADER, ABAJOIZQ, ARRIBAIZQ, ABAJODER
    int controls[4] = {84, 85, 82, 83};
    bool backdoor;

    // Pantallas/Estados:
    // INICIO, INFONIVEL, JUEGO, CREDITOS(registro nombre y highscore), CLOSE
    bool pause = false;

    inicioIntro:
    {
        al_stop_sample_instance(juegoInstance);
        al_play_sample_instance(introInstance);
        // Pantalla inicial e instrucciones
        PantallaInicial init = PantallaInicial(WIDTH/scale, HEIGHT/scale,
                                cuboID, coilyID, slicksamID, blobID, controls, level);
        inicio:
        {
//            goto juegoIntro; // Descomentar para ir directamente al juego
            //goto creditosIntro; // Descomentar ir directamente a los creditos
            al_wait_for_event(queue, &event);
            al_get_keyboard_state(&keyState);

            switch (event.type) {
                case ALLEGRO_EVENT_TIMER:

                    redraw = true;
                    if(!pause) init.movement();
                    break;

                case ALLEGRO_EVENT_KEY_DOWN:

                    if(!pause) {
                        switch (event.keyboard.keycode) {
                            case ALLEGRO_KEY_ENTER:
                                if (init.pant == 0) { // INICIO
                                    init.destroy();

                                    level = init.level;
                                    cuboID = init.cuboID, coilyID = init.coilyID,
                                    slicksamID = init.slicksamID, blobID = init.blobID;
                                    // UP, DOWN, LEFT, RIGHT
                                    controls[0] = init.controls[0], controls[1] = init.controls[1],
                                    controls[2] = init.controls[2], controls[3] = init.controls[3];

                                    escena.setLevel(level);
                                    backdoor = level != 1;

                                    al_stop_sample_instance(introInstance);

//                                goto infonivelIntro;
                                    goto juegoIntro;

                                } else { // Menu/controles
                                    init.accionPantallas(event.keyboard.keycode);
                                }

                                break;

                            case ALLEGRO_KEY_F1:
                                init.escenarioMenu();
                                break;

                            default:
                                if (init.pant != 0) {
                                    init.accionPantallas(event.keyboard.keycode);
                                }
                        }
                    }
                    else{
                        switch (event.keyboard.keycode) {
                            case ALLEGRO_KEY_DOWN:
                                if(posPause==2) posPause = 0;
                                else posPause++;
                                break;
                            case ALLEGRO_KEY_UP:
                                if(posPause==0) posPause = 2;
                                else posPause--;
                                break;
                            case ALLEGRO_KEY_ENTER:
                                pause = !pause;
                                if(posPause == 1){
                                    goto inicioIntro;
                                }
                                else if(posPause == 2){
                                    init.destroy();
                                    goto close;
                                }
                                break;
                        }
                    }

                    break;

                case ALLEGRO_EVENT_DISPLAY_RESIZE:

                    al_identity_transform(&camera);

                    // datos previos al resize
                    WIDTH = al_get_display_width(disp);
                    HEIGHT = al_get_display_height(disp);

                    al_acknowledge_resize(disp);

                    if(HEIGHT != (float)al_get_display_height(disp)){
                        WIDTH = (330.0/280.0)*(float)al_get_display_height(disp);
                        al_resize_display(disp, WIDTH, (float)al_get_display_height(disp));
                        scale += ((float) al_get_display_height(disp) - (float) HEIGHT) * 0.0035f;
                    } else if(WIDTH != (float)al_get_display_width(disp)){
                        HEIGHT = (float)al_get_display_width(disp)/(330.0/280.0);
                        al_resize_display(disp, (float)al_get_display_width(disp), HEIGHT);
                        scale += ((float) al_get_display_width(disp) - (float) WIDTH) * 0.0035f;
                    }

                    al_scale_transform(&camera, scale, scale);
                    al_use_transform(&camera);

                    WIDTH = al_get_display_width(disp);
                    HEIGHT = al_get_display_height(disp);

                    // RESIZE ALL ITEMS
                    init.resize(WIDTH / scale, HEIGHT / scale);

                    guardarResize();

                    break;

                case ALLEGRO_EVENT_DISPLAY_CLOSE:
                    if(!pause) {
                        posPause = 0;
                        pause = !pause;
                    }
                    break;

            }
            if (redraw && al_is_event_queue_empty(queue)) {
                //REDRAW THE IMAGE WITH EVERYTHING
                al_clear_to_color(al_map_rgb(49, 33, 121));

                init.drawBitmap();
                if(pause) init.drawPause(pauseBitmap, fonts, posPause);

                al_flip_display();
                redraw = false;
            }

            goto inicio;
        };
    };

    // PREVIO AL NIVEL
    infonivelIntro:
    {
        al_stop_sample_instance(juegoInstance);
        if(escena.getLevel()>9) goto creditos;

        PantallaInfoNivel infonivel = PantallaInfoNivel(WIDTH/scale, HEIGHT/scale);
        if(escena.getLevel() == 1) infonivel.sonidoMoneda();
        infonivel.startLevel(escena.getLevel());

        infonivel:
        {
            al_wait_for_event(queue, &event);
            al_get_keyboard_state(&keyState);

            switch (event.type) {
                case ALLEGRO_EVENT_TIMER:

                    redraw = true;
                    if(!pause) infonivel.movement();

                    if (infonivel.fin()) {
                        goto juegoIntro;
                    }

                    break;

                case ALLEGRO_KEY_DOWN:
                    if(pause){
                        switch (event.keyboard.keycode) {
                            case ALLEGRO_KEY_DOWN:
                                if(posPause==2) posPause = 0;
                                else posPause++;
                                break;
                            case ALLEGRO_KEY_UP:
                                if(posPause==0) posPause = 2;
                                else posPause--;
                                break;
                            case ALLEGRO_KEY_ENTER:
                                pause = !pause;
                                if(posPause == 1){
                                    goto inicioIntro;
                                }
                                else if(posPause == 2){
                                    infonivel.destroy();
                                    goto close;
                                }
                                break;
                        }
                    }
                    break;

                case ALLEGRO_EVENT_DISPLAY_RESIZE:

                    al_identity_transform(&camera);

                    // datos previos al resize
                    WIDTH = al_get_display_width(disp);
                    HEIGHT = al_get_display_height(disp);

                    al_acknowledge_resize(disp);

                    if(HEIGHT != (float)al_get_display_height(disp)){
                        WIDTH = (330.0/280.0)*(float)al_get_display_height(disp);
                        al_resize_display(disp, WIDTH, (float)al_get_display_height(disp));
                        scale += ((float) al_get_display_height(disp) - (float) HEIGHT) * 0.0035f;
                    } else if(WIDTH != (float)al_get_display_width(disp)){
                        HEIGHT = (float)al_get_display_width(disp)/(330.0/280.0);
                        al_resize_display(disp, (float)al_get_display_width(disp), HEIGHT);
                        scale += ((float) al_get_display_width(disp) - (float) WIDTH) * 0.0035f;
                    }

                    al_scale_transform(&camera, scale, scale);
                    al_use_transform(&camera);

                    WIDTH = al_get_display_width(disp);
                    HEIGHT = al_get_display_height(disp);

                    // RESIZE ALL ITEMS
                    infonivel.resize(WIDTH / scale, HEIGHT / scale);

                    guardarResize();

                    break;

                case ALLEGRO_EVENT_DISPLAY_CLOSE:
                    if(!pause) {
                        posPause = 0;
                        pause = !pause;
                    }
                    break;

            }
            if (redraw && al_is_event_queue_empty(queue)) {
                //REDRAW THE IMAGE WITH EVERYTHING
                al_clear_to_color(al_map_rgb(0, 0, 0));

                infonivel.drawBitmap();
                if(pause) infonivel.drawPause(pauseBitmap, fonts, posPause);

                al_flip_display();
                redraw = false;
            }
        };

        goto infonivel;
    };

    // GAME LOOP
    juegoIntro:
    {
        al_play_sample_instance(juegoInstance);
        escena.load(WIDTH/scale, HEIGHT/scale, backdoor, cuboID, coilyID, slicksamID, blobID);

        juego:
        {
            if(escena.piramideCompleta()) {
                escena.setRound(escena.getRound()+1);
                if(escena.getRound() > 4){
                    escena.setRound(1);
                    escena.setLevel(escena.getLevel()+1);

                    if(escena.getLevel() > 9){
                        goto creditosIntro;
                    }

                    goto infonivelIntro;
                }

                goto juegoIntro;
            }
            else if(escena.isGameover()){
                goto creditosIntro;
            }

            al_wait_for_event(queue, &event);
            al_get_keyboard_state(&keyState);

            switch (event.type) {
                case ALLEGRO_EVENT_TIMER:

                    redraw = true;
                    if(!pause) {
                        escena.movementAll();
                        escena.generarEnemigos();
                        escena.changeCube();
                    }

                    break;

                case ALLEGRO_EVENT_KEY_DOWN:

                    if (!escena.qbert.isJumping() && !pause) {
                        if(event.keyboard.keycode == controls[2]){
                            escena.setMoveQBert(TOPLEFT);
                        }
                        else if(event.keyboard.keycode == controls[3]) {
                            escena.setMoveQBert(DOWNRIGHT);
                        }
                        else if(event.keyboard.keycode == controls[0]) {
                            escena.setMoveQBert(TOPRIGHT);
                        }
                        else if(event.keyboard.keycode == controls[1]) {
                            escena.setMoveQBert(DOWNLEFT);
                        }
                        else if(event.keyboard.keycode == ALLEGRO_KEY_PAD_PLUS ||
                                event.keyboard.keycode == ALLEGRO_KEY_EQUALS) {
                            escena.qbert.setLives(escena.qbert.getLives() + 1);
                        }
                        else if(event.keyboard.keycode == ALLEGRO_KEY_ENTER){
                            escena.setPiramideCompleta();
                        }
                    }
                    else if(pause){
                        switch (event.keyboard.keycode) {
                            case ALLEGRO_KEY_DOWN:
                                if(posPause==2) posPause = 0;
                                else posPause++;
                                break;
                            case ALLEGRO_KEY_UP:
                                if(posPause==0) posPause = 2;
                                else posPause--;
                                break;
                            case ALLEGRO_KEY_ENTER:
                                pause = !pause;
                                if(posPause == 1){
                                    goto inicioIntro;
                                }
                                else if(posPause == 2){
                                    goto close;
                                }
                                break;
                        }
                    }

                    if(event.keyboard.keycode == ALLEGRO_KEY_ESCAPE){ // ESC
                        posPause = 0;
                        pause = !pause;
                    }

                    break;

                case ALLEGRO_EVENT_DISPLAY_RESIZE:

                    al_identity_transform(&camera);

                    // datos previos al resize
                    WIDTH = al_get_display_width(disp);
                    HEIGHT = al_get_display_height(disp);

                    al_acknowledge_resize(disp);

                    if(HEIGHT != (float)al_get_display_height(disp)){
                        WIDTH = (330.0/280.0)*(float)al_get_display_height(disp);
                        al_resize_display(disp, WIDTH, (float)al_get_display_height(disp));
                        scale += ((float) al_get_display_height(disp) - (float) HEIGHT) * 0.0035f;
                    } else if(WIDTH != (float)al_get_display_width(disp)){
                        HEIGHT = (float)al_get_display_width(disp)/(330.0/280.0);
                        al_resize_display(disp, (float)al_get_display_width(disp), HEIGHT);
                        scale += ((float) al_get_display_width(disp) - (float) WIDTH) * 0.0035f;
                    }

                    al_scale_transform(&camera, scale, scale);
                    al_use_transform(&camera);

                    WIDTH = al_get_display_width(disp);
                    HEIGHT = al_get_display_height(disp);

                    // RESIZE ALL ITEMS
                    escena.resizAll(WIDTH/scale, HEIGHT/scale);

                    guardarResize();

                    break;

                case ALLEGRO_EVENT_DISPLAY_CLOSE:
                    if(!pause) {
                        posPause = 0;
                        pause = !pause;
                    }
                    break;

            }
            if (redraw && al_is_event_queue_empty(queue)) {
                //REDRAW THE IMAGE WITH EVERYTHING
                al_clear_to_color(al_map_rgb(0, 0, 0));

                escena.drawAll();
                if(pause) escena.drawPause(pauseBitmap, fonts, posPause);

                al_flip_display();
                redraw = false;
            }

            goto juego;
        };
    };

    // CREDITOS
    creditosIntro:
    {   escena.setLevel(1);
        escena.setRound(1);
        credit.loadPantalla(escena.getPuntuacion(), WIDTH/scale, HEIGHT/scale);
        creditos:
        {
            if(credit.isFinish()) goto inicioIntro;

            al_wait_for_event(queue, &event);
            al_get_keyboard_state(&keyState);

            switch (event.type) {
                case ALLEGRO_EVENT_TIMER:

                    redraw = true;
                    if(!pause) credit.movement();

                    break;

                case ALLEGRO_EVENT_KEY_DOWN:

                    //std::cout << event.keyboard.keycode << std::endl;
                    if(!pause) credit.write(event.keyboard.keycode);
                    else {
                        switch (event.keyboard.keycode) {
                            case ALLEGRO_KEY_DOWN:
                                if(posPause==2) posPause = 0;
                                else posPause++;
                                break;
                            case ALLEGRO_KEY_UP:
                                if(posPause==0) posPause = 2;
                                else posPause--;
                                break;
                            case ALLEGRO_KEY_ENTER:
                                pause = !pause;
                                if(posPause == 1){
                                    goto inicioIntro;
                                }
                                else if(posPause == 2){
                                    credit.destroy();
                                    goto close;
                                }
                                break;
                        }
                    }

                    break;

                case ALLEGRO_EVENT_DISPLAY_RESIZE:

                    al_identity_transform(&camera);

                    // datos previos al resize
                    WIDTH = al_get_display_width(disp);
                    HEIGHT = al_get_display_height(disp);

                    al_acknowledge_resize(disp);

                    if(HEIGHT != (float)al_get_display_height(disp)){
                        WIDTH = (330.0/280.0)*(float)al_get_display_height(disp);
                        al_resize_display(disp, WIDTH, (float)al_get_display_height(disp));
                        scale += ((float) al_get_display_height(disp) - (float) HEIGHT) * 0.003f;
                    } else if(WIDTH != (float)al_get_display_width(disp)){
                        HEIGHT = (float)al_get_display_width(disp)/(330.0/280.0);
                        al_resize_display(disp, (float)al_get_display_width(disp), HEIGHT);
                        scale += ((float) al_get_display_width(disp) - (float) WIDTH) * 0.003f;
                    }

                    al_scale_transform(&camera, scale, scale);
                    al_use_transform(&camera);

                    WIDTH = al_get_display_width(disp);
                    HEIGHT = al_get_display_height(disp);

                    // RESIZE ALL ITEMS
                    credit.resize(WIDTH / scale, HEIGHT / scale);

                    guardarResize();

                    break;

                case ALLEGRO_EVENT_DISPLAY_CLOSE:
                    if(!pause) {
                        posPause = 0;
                        pause = !pause;
                    }
                    break;

            }
            if (redraw && al_is_event_queue_empty(queue)) {
                //REDRAW THE IMAGE WITH EVERYTHING
                al_clear_to_color(al_map_rgb(0, 0, 0));

                credit.drawBitmap();
                if(pause) credit.drawPause(pauseBitmap, fonts, posPause);

                al_flip_display();
                redraw = false;
            }

            goto creditos;
        };
    };

    close:
    /*************************
     *     END GAME LOOP     *
     *************************/

    // DESTRUIR OBJETOS ESCENA
    escena.destroyAll();
    credit.destroy();

    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
    al_destroy_sample(intro);
    al_destroy_sample_instance(introInstance);
    al_destroy_sample(juego);
    al_destroy_sample_instance(juegoInstance);

    al_destroy_bitmap(pauseBitmap);
    al_destroy_bitmap(fonts);

    return 0;
}

void must_init(bool test, const char *description) {
    if (test) return;

    printf("No se ha podido inicializar: %s\n", description);
    exit(1);
}

void guardarResize(){
    // Guardar en memoria a la hora del reescalado
    std::ofstream file("../pantalla.txt");
    file << WIDTH << " " << HEIGHT << " " << scale;
    file.close();
}