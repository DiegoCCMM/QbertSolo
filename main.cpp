#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <list>

#include "Piramide.hpp"
#include "Enemy.hpp"
#include "Coily.hpp"
#include "PantallaInicial.hpp"
#include "PantallaInfoNivel.hpp"
#include "Escena.hpp"
#include "Creditos.hpp"

#define NUMERODESAMPLES 10

float scale = 1.0f;
float WIDTH = 330, HEIGHT = 280;

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
    al_reserve_samples(NUMERODESAMPLES);

    Creditos credit = Creditos();
    Escena escena = Escena(WIDTH/scale, HEIGHT/scale);
    bool redraw = true;
    ALLEGRO_EVENT event;
    al_start_timer(timer);

    /*************************
     *       GAME LOOP       *
     *************************/

    // Pantallas:
    // INICIO, INFONIVEL, JUEGO, CREDITOS(registro nombre y highscore), CLOSE

    inicioIntro:
    {
        // Pantalla inicial e instrucciones
        PantallaInicial init = PantallaInicial(WIDTH/scale, HEIGHT/scale);
        inicio:
        {
            //goto juegoIntro; // Descomentar para ir directamente al juego
            //goto creditosIntro; // Descomentar ir directamente a los creditos
            al_wait_for_event(queue, &event);
            al_get_keyboard_state(&keyState);

            switch (event.type) {
                case ALLEGRO_EVENT_TIMER:

                    redraw = true;
                    init.movement();
                    break;

                case ALLEGRO_EVENT_KEY_DOWN:

                    if (event.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                        init.destroy();

                        goto infonivelIntro;
                    }

                    break;

                case ALLEGRO_EVENT_DISPLAY_RESIZE:

                    al_identity_transform(&camera);

                    int prev_disp;
                    prev_disp = al_get_display_width(disp);
                    al_acknowledge_resize(disp);
                    scale += ((float) al_get_display_width(disp) - (float) prev_disp) * 0.001f;
                    al_scale_transform(&camera, scale, scale);
                    al_use_transform(&camera);

                    WIDTH = al_get_display_width(disp);
                    HEIGHT = al_get_display_height(disp);

                    // RESIZE ALL ITEMS
                    init.resize(WIDTH / scale, HEIGHT / scale);

                    break;

                case ALLEGRO_EVENT_DISPLAY_CLOSE:
                    init.destroy();
                    goto close;

            }
            if (redraw && al_is_event_queue_empty(queue)) {
                //REDRAW THE IMAGE WITH EVERYTHING
                al_clear_to_color(al_map_rgb(49, 33, 121));

                init.drawBitmap();

                al_flip_display();
                redraw = false;
            }

            goto inicio;
        };
    };

    // PREVIO AL NIVEL
    infonivelIntro:
    {
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
                    infonivel.movement();

                    if (infonivel.fin()) {
                        goto juegoIntro;
                    }

                    break;

                case ALLEGRO_EVENT_DISPLAY_RESIZE:

                    al_identity_transform(&camera);

                    int prev_disp;
                    prev_disp = al_get_display_width(disp);
                    al_acknowledge_resize(disp);
                    scale += ((float) al_get_display_width(disp) - (float) prev_disp) * 0.001f;
                    al_scale_transform(&camera, scale, scale);
                    al_use_transform(&camera);

                    WIDTH = al_get_display_width(disp);
                    HEIGHT = al_get_display_height(disp);

                    // RESIZE ALL ITEMS
                    infonivel.resize(WIDTH / scale, HEIGHT / scale);

                    break;

                case ALLEGRO_EVENT_DISPLAY_CLOSE:
                    goto close;
                    break;

            }
            if (redraw && al_is_event_queue_empty(queue)) {
                //REDRAW THE IMAGE WITH EVERYTHING
                al_clear_to_color(al_map_rgb(0, 0, 0));

                infonivel.drawBitmap();

                al_flip_display();
                redraw = false;
            }
        };

        goto infonivel;
    };

    // GAME LOOP
    juegoIntro:
    {
        escena.load(WIDTH/scale, HEIGHT/scale);

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
                    escena.movementAll();
                    escena.generarEnemigos();
                    break;

                case ALLEGRO_EVENT_KEY_DOWN:

                    if (!escena.qbert.isJumping()) {
                        switch (event.keyboard.keycode) {
                            case ALLEGRO_KEY_LEFT:
                                escena.setMoveQBert(TOPLEFT);
                                break;
                            case ALLEGRO_KEY_RIGHT:
                                escena.setMoveQBert(DOWNRIGHT);
                                break;
                            case ALLEGRO_KEY_UP:
                                escena.setMoveQBert(TOPRIGHT);
                                break;
                            case ALLEGRO_KEY_DOWN:
                                escena.setMoveQBert(DOWNLEFT);
                                break;
                            case ALLEGRO_KEY_PAD_PLUS:
                            case ALLEGRO_KEY_EQUALS:
                                escena.qbert.setLives(escena.qbert.getLives()+1);
                                break;
                        }
                    }
                    break;

                case ALLEGRO_EVENT_DISPLAY_RESIZE:

                    al_identity_transform(&camera);

                    int prev_disp;
                    prev_disp = al_get_display_width(disp);
                    al_acknowledge_resize(disp);
                    scale += ((float) al_get_display_width(disp) - (float) prev_disp) * 0.001f;
                    al_scale_transform(&camera, scale, scale);
                    al_use_transform(&camera);

                    WIDTH = al_get_display_width(disp);
                    HEIGHT = al_get_display_height(disp);

                    // RESIZE ALL ITEMS
                    escena.resizAll(WIDTH/scale, HEIGHT/scale);

                    break;


                case ALLEGRO_EVENT_DISPLAY_CLOSE:
                    goto close;
                    break;

            }
            if (redraw && al_is_event_queue_empty(queue)) {
                //REDRAW THE IMAGE WITH EVERYTHING
                al_clear_to_color(al_map_rgb(0, 0, 0));

                escena.drawAll();

                al_flip_display();
                redraw = false;
            }

            goto juego;
        };
    };

    // CREDITOS
    creditosIntro:
    {
        credit.loadPantalla(escena.getPuntuacion(), WIDTH/scale, HEIGHT/scale);
        creditos:
        {
            if(credit.isFinish()) goto inicioIntro;

            al_wait_for_event(queue, &event);
            al_get_keyboard_state(&keyState);

            switch (event.type) {
                case ALLEGRO_EVENT_TIMER:

                    redraw = true;
                    credit.movement();
                    break;

                case ALLEGRO_EVENT_KEY_DOWN:

                    //std::cout << event.keyboard.keycode << std::endl;
                    credit.write(event.keyboard.keycode);

                    break;

                case ALLEGRO_EVENT_DISPLAY_RESIZE:

                    al_identity_transform(&camera);

                    int prev_disp;
                    prev_disp = al_get_display_width(disp);
                    al_acknowledge_resize(disp);
                    scale += ((float) al_get_display_width(disp) - (float) prev_disp) * 0.001f;
                    al_scale_transform(&camera, scale, scale);
                    al_use_transform(&camera);

                    WIDTH = al_get_display_width(disp);
                    HEIGHT = al_get_display_height(disp);

                    // RESIZE ALL ITEMS
                    credit.resize(WIDTH / scale, HEIGHT / scale);

                    break;

                case ALLEGRO_EVENT_DISPLAY_CLOSE:
                    goto close;

            }
            if (redraw && al_is_event_queue_empty(queue)) {
                //REDRAW THE IMAGE WITH EVERYTHING
                al_clear_to_color(al_map_rgb(0, 0, 0));

                credit.drawBitmap();

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

    return 0;
}

void must_init(bool test, const char *description) {
    if (test) return;

    printf("No se ha podido inicializar: %s\n", description);
    exit(1);
}