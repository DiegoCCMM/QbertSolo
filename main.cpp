#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <vector>

#define framePixels 16
#define airTime 9
enum Direction {
    TOPRIGHT, TOPLEFT, DOWNRIGHT, DOWNLEFT
};

struct Player {
    float x;
    float y;
    Direction dir;
};

void must_init(bool test, const char *description) {
    if (test) return;

    printf("couldn't initialize %s\n", description);
    exit(1);
}

int main() {
    must_init(al_init(), "allegro");
    must_init(al_install_keyboard(), "keyboard");

    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 30.0);
    must_init(timer, "timer");

    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    must_init(queue, "queue");

    ALLEGRO_DISPLAY *disp = al_create_display(640, 480);
    must_init(disp, "display");

    ALLEGRO_FONT *font = al_create_builtin_font();
    must_init(font, "font");

    ALLEGRO_KEYBOARD_STATE keyState;

    must_init(al_init_image_addon(), "image addon");
    ALLEGRO_BITMAP *player = al_load_bitmap("/home/diego/Desktop/info/4-2/Videojuegos/Qbert/qbert.png");
    must_init(player, "player");
    Player player1{100, 100, DOWNRIGHT};
    int sourceX = 0, sourceY = 2;

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    std::vector<int> source;
    std::vector<int> width;

    ALLEGRO_COLOR pixel, lastPixel, separatorColor;

    source.push_back(0);

    for (int i = 0; i < al_get_bitmap_width(player); i++) {
        separatorColor = al_map_rgba(0, 0, 255, 255);
        pixel = al_get_pixel(player, i, 0);

        if (memcmp(&pixel, &lastPixel, sizeof(ALLEGRO_COLOR))) {
            if (!memcmp(&pixel, &separatorColor, sizeof(ALLEGRO_COLOR))) {
                source.push_back(i);
                if (source.size() == 2)
                    width.push_back(i);
                else
                    width.push_back(i - width[width.size() - 1]);
            }
        } else if (i == al_get_bitmap_width(player) - 1) {
            width.push_back(i - width[width.size() - 1]);
            lastPixel = pixel;
        }
    }

    bool done = false;
    bool redraw = true;
    bool jumping = false;
    int airTimer = 0;
    ALLEGRO_EVENT event;

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
                        if(player1.dir == TOPRIGHT || player1.dir == DOWNRIGHT)
                            player1.x += 5.0f;
                        else
                            player1.x -= 5.0f;
                        if(player1.dir != DOWNRIGHT && player1.dir != DOWNLEFT)
                            player1.y -= 5.0f;
                    }else if (airTimer > airTime/2 && airTimer < airTime){
                        //GO DOWN AND DIRECTION
                        if(player1.dir == TOPRIGHT || player1.dir == DOWNRIGHT)
                            player1.x += 5.0f;
                        else
                            player1.x -= 5.0f;
                        if(player1.dir == DOWNRIGHT || player1.dir == DOWNLEFT)
                            player1.y += 5.0f;
                    }else if (airTimer > airTime) {
                        //WE LANDED
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
                            player1.dir = TOPLEFT;
                            break;
                        case ALLEGRO_KEY_RIGHT:
                            player1.dir = DOWNRIGHT;
                            break;
                        case ALLEGRO_KEY_UP:
                            player1.dir = TOPRIGHT;
                            break;
                        case ALLEGRO_KEY_DOWN:
                            player1.dir = DOWNLEFT;
                            break;
                    }
                    jumping = true;
                    sourceX += 16;
                }
                break;
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                done = true;
                break;
        }

        if (redraw && al_is_event_queue_empty(queue)) {
            al_clear_to_color(al_map_rgb(0, 0, 0));

            al_draw_bitmap_region(player, sourceX + (player1.dir * 2 * framePixels), 0, framePixels, framePixels,
                                  player1.x, player1.y, 0);

            al_flip_display();

            redraw = false;
        }
    }

    al_destroy_bitmap(player);
    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}