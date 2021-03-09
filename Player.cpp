//
// Created by diego on 6/3/21.
//

#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>

class Player {
    ALLEGRO_BITMAP *bmp;

    Player() {
        bmp = al_load_bitmap("./sprites/Arcade - QBert - General Sprites.bmp");
        al_convert_mask_to_alpha(bmp, (al_map_rgb(255, 0, 255)));

        animationWidth = al_get_bitmap_width(bmp) / 32; //the tiles are 32/32
        animationHeight = al_get_bitmap_height(bmp) / 32;


        for (int h = 0; h < animationHeight; h++) {
            for (int w = 0; w < animationWidth; w++) {
                pallet.push_back(al_create_sub_bitmap(bmp, w * TILE_SIZE, h * TILE_SIZE, TILE_SIZE, TILE_SIZE));
            }
        }

        bmp = pallet[0];
    }


    void animate(int state, int cycleTime) {

        animationCount++;
        if (animationCount >= cycleTime) {
            animationCount = 0;

        }
        animationState = state;

        if (state >= 0 && state <= 3) {
            direction = state;
        }

        bmp = pallet[state * animationWidth + (animationCount / (cycleTime / 4)) + 1];

    }
};