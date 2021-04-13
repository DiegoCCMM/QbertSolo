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
#include "Enemy.hpp"
#include "Coily.hpp"
#include "Platillo.hpp"
#include "PantallaInicial.hpp"
#include "PantallaInfoNivel.hpp"

#define periodoEnemigos 12
float scale = 1.0f;
float WIDTH = 330, HEIGHT = 280;

void generarEnemigos(int & timer, std::list<Enemy> &enemies, Piramide pir);
void checkRandMovementEnemies(std::list<Enemy> &enemies);
void movementAll(Piramide &piramide, QBert &qbert, std::list<Enemy> &enemies, std::list<Platillo> &platillos);
void drawAll(Piramide &piramide, QBert &qbert, std::list<Enemy> &enemies, std::list<Platillo> &platillos);
void resizAll(Piramide &piramide, QBert &qbert, std::list<Enemy> &enemies, std::list<Platillo> &platillos);
void destroyAll(Piramide &piramide, QBert &qbert, std::list<Enemy> &enemies, std::list<Platillo> &platillos);

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

//********************************************************************************************************
// TODO Si creamos la clase escena se quitarian todas estas funciones
    // TODO: crear el resto de .txt de todos los niveles y sus rondas
    // CARGAR MAPA
    Piramide piramide;
    piramide.loadMap(2, 3, WIDTH, HEIGHT);
    // END MAPA

    // CARGAR PERSONAJES
    QBert qbert = QBert(piramide);
    std::list<Enemy> enemies;
    // FIN PERSONAJES

    // CARGAR COMPONENTES RESTANTES
    // TODO: guardarlo todos en una lista (como enemies)
    std::list<Platillo> platillos;
    Platillo plato = Platillo(piramide, 3, IZQ);
    platillos.push_back(plato);
    // platillos
    // letras
    // puntos
    // vidas
    // etc
    // FIN COMPONENTES RESTANTES
//********************************************************************************************************

    int level = 1, round = 1;

    bool redraw = true;
    ALLEGRO_EVENT event;
    al_start_timer(timer);
    int periodEnemies = 0;

    /*************************
     *       GAME LOOP       *
     *************************/

    // Pantallas:
    // INICIO, INFONIVEL, JUEGO, REGNOM(registro nombre), HIGHSCORES, CLOSE

    // Pantalla inicial e instrucciones
    PantallaInicial init = PantallaInicial(WIDTH/scale, HEIGHT/scale);
    inicio:
    {
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

                    //goto juegoIntro;
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

    infonivelIntro:
    {
        PantallaInfoNivel infonivel = PantallaInfoNivel(WIDTH/scale, HEIGHT/scale);
        if(level == 1) infonivel.sonidoMoneda();
        infonivel.startLevel(level);

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

    juegoIntro:
    {
        // Crear el juego, escena (en caso de que creemos una clase ESCENA y metamos tod0 ahi--> diria que mejor)
        // Por tanto no se necesitaria hacer el resize --> habria un constructor al que pasar width y height
        // Y tod0 se crearia respecto a esos datos
        resizAll(piramide, qbert, enemies, platillos);

        juego:
        {
            al_wait_for_event(queue, &event);
            al_get_keyboard_state(&keyState);

            switch (event.type) {
                case ALLEGRO_EVENT_TIMER:

                    redraw = true;
                    movementAll(piramide, qbert, enemies, platillos);
                    generarEnemigos(periodEnemies, enemies, piramide);
                    break;

                case ALLEGRO_EVENT_KEY_DOWN:

                    if (!qbert.isJumping()) {
                        switch (event.keyboard.keycode) {
                            case ALLEGRO_KEY_LEFT:
                                qbert.setMove(TOPLEFT);
                                break;
                            case ALLEGRO_KEY_RIGHT:
                                qbert.setMove(DOWNRIGHT);
                                break;
                            case ALLEGRO_KEY_UP:
                                qbert.setMove(TOPRIGHT);
                                break;
                            case ALLEGRO_KEY_DOWN:
                                qbert.setMove(DOWNLEFT);
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
                    resizAll(piramide, qbert, enemies, platillos);

                    break;


                case ALLEGRO_EVENT_DISPLAY_CLOSE:
                    goto close;
                    break;

            }
            if (redraw && al_is_event_queue_empty(queue)) {
                //REDRAW THE IMAGE WITH EVERYTHING
                al_clear_to_color(al_map_rgb(0, 0, 0));

                drawAll(piramide, qbert, enemies, platillos);

                al_flip_display();
                redraw = false;
            }

            goto juego;
        };
    };


    close:
    /*************************
     *     END GAME LOOP     *
     *************************/

    // DESTRUIR OBJETOS ESCENA
    destroyAll(piramide, qbert, enemies, platillos); // TODO Si creamos la clase escena se quitaria de aqui

    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}

// TODO Si creamos la clase escena se quitarian todas estas funciones
void generarEnemigos(int & timer, std::list<Enemy> &enemies, Piramide pir){

    if(timer == periodoEnemigos){
        //genera un enemigo aleatorio
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_int_distribution<int> dist(0, 60);
        int eleccion = dist(mt);
        if(eleccion >= 0 && eleccion <= 14){
            //redblob o poder
            Enemy redblob = Enemy(pir, "Redblob", 1, 0, 9, 0); // X e Y (pixeles) posicion respecto al cubo[i,j]
            enemies.push_back(redblob);
        }else if(eleccion >= 15 && eleccion <= 29){
            //coily

        }else if(eleccion >= 30 && eleccion <= 44){
            //ugg o wrong way
        }else if(eleccion >= 45 && eleccion <= 60){
            //slick o sam
        }


        timer = 0;
    }else{
        timer++;
    }
}

// TODO: en referencia a un todo de arriba, probar a meter todo en una lista y pasarla?
void movementAll(Piramide &piramide, QBert &qbert, std::list<Enemy> &enemies, std::list<Platillo> &platillos) {
    qbert.movement(&piramide, HEIGHT, platillos);
    
    if(!qbert.isFalling()) {
        checkRandMovementEnemies(enemies);
        for (std::_List_iterator<Enemy> it = enemies.begin(); it != enemies.end(); it++) {
            it->movement(&piramide, HEIGHT);
        }

        for (std::_List_iterator<Platillo> it = platillos.begin(); it != platillos.end(); it++) {
            it->movement();
        }
    }
}

void drawAll(Piramide &piramide, QBert &qbert, std::list<Enemy> &enemies, std::list<Platillo> &platillos) {

    if (qbert.isFalling()) { // Si QBert esta cayendo primero se dibuja a QBert y luego la piramide
        qbert.drawBitmap();
        piramide.drawBitmap();
        // TODO: dibujar resto de componentes de la pantalla MENOS los enemigos
        // Recordar que mientras cae el resto de elementos dejan de moverse
        enemies.clear();
        for (std::_List_iterator<Platillo> it = platillos.begin(); it != platillos.end(); it++) {
            it->drawBitmap();
        }
    } else {
        piramide.drawBitmap();
        for (std::_List_iterator<Platillo> it = platillos.begin(); it != platillos.end(); it++) {
            it->drawBitmap();
        }
        qbert.drawBitmap();
        for (std::_List_iterator<Enemy> it = enemies.begin(); it != enemies.end(); it++) {
            it->drawBitmap();
        }

        // TODO: dibujar resto de componentes de la pantalla
    }
}

void resizAll(Piramide &piramide, QBert &qbert, std::list<Enemy> &enemies, std::list<Platillo> &platillos){
    piramide.resizeMap(WIDTH/scale, HEIGHT/scale);

    for (std::_List_iterator<Enemy> it = enemies.begin(); it != enemies.end(); it++) {
        it->resize(&piramide);
    }

    for (std::_List_iterator<Platillo> it = platillos.begin(); it != platillos.end(); it++) {
        it->resize(&piramide);
    }

    qbert.resize(&piramide);
}

void destroyAll(Piramide &piramide, QBert &qbert, std::list<Enemy> &enemies, std::list<Platillo> &platillos){
    piramide.destroy();
    qbert.destroy();

    for (std::_List_iterator<Enemy> it = enemies.begin(); it != enemies.end(); it++){
        it->destroy();
    }

    for (std::_List_iterator<Platillo> it = platillos.begin(); it != platillos.end(); it++) {
        it->destroy();
    }
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

void must_init(bool test, const char *description) {
    if (test) return;

    printf("No se ha podido inicializar: %s\n", description);
    exit(1);
}