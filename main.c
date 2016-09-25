#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

void fireplace(ALLEGRO_BITMAP* tileset, ALLEGRO_BITMAP* fire_bmp)
{
    int x = 0;
    int y = 0;
    int ticks = 0;

    al_draw_bitmap_region(tileset, 8 * 16, 12 * 16, 16, 16, 16, 412, 0);

    al_draw_bitmap_region(tileset, 13 * 16, 12 * 16, 16, 16, 32, 412, 0);
    al_draw_bitmap_region(tileset, 13 * 16, 12 * 16, 16, 16, 48, 412, 0);
    al_draw_bitmap_region(tileset, 13 * 16, 12 * 16, 16, 16, 64, 412, 0);
    al_draw_bitmap_region(tileset, 13 * 16, 12 * 16, 16, 16, 80, 412, 0);
    al_draw_bitmap_region(tileset, 13 * 16, 12 * 16, 16, 16, 96, 412, 0);

    al_draw_bitmap_region(tileset, 12 * 16, 11 * 16, 16, 16, 112, 412, 0);

    al_draw_bitmap_region(tileset, 10 * 16, 11 * 16, 16, 16, 16, 396, 0);
    al_draw_bitmap_region(tileset, 10 * 16, 11 * 16, 16, 16, 16, 380, 0);
    al_draw_bitmap_region(tileset, 10 * 16, 11 * 16, 16, 16, 16, 364, 0);

    al_draw_bitmap_region(tileset, 10 * 16, 11 * 16, 16, 16, 112, 396, 0);
    al_draw_bitmap_region(tileset, 10 * 16, 11 * 16, 16, 16, 112, 380, 0);
    al_draw_bitmap_region(tileset, 10 * 16, 11 * 16, 16, 16, 112, 364, 0);

    al_draw_bitmap_region(tileset, 11 * 16, 11 * 16, 16, 16, 112, 348, 0);
    al_draw_bitmap_region(tileset, 9 * 16, 12 * 16, 16, 16, 16, 348, 0);

    al_draw_bitmap_region(tileset, 13 * 16, 12 * 16, 16, 16, 32, 348, 0);
    al_draw_bitmap_region(tileset, 13 * 16, 12 * 16, 16, 16, 48, 348, 0);
    al_draw_bitmap_region(tileset, 13 * 16, 12 * 16, 16, 16, 80, 348, 0);
    al_draw_bitmap_region(tileset, 13 * 16, 12 * 16, 16, 16, 96, 348, 0);

    al_draw_bitmap_region(tileset, 10 * 16, 12 * 16, 16, 16, 64, 348, 0);

    al_draw_bitmap_region(tileset, 10 * 16, 11 * 16, 16, 16, 64, 332, 0);
    al_draw_bitmap_region(tileset, 10 * 16, 11 * 16, 16, 16, 64, 316, 0);
    al_draw_bitmap_region(tileset, 10 * 16, 11 * 16, 16, 16, 64, 300, 0);

    while (1)
    {
        int i;
        for (i = 0; i < 5; i++) {
            if (rand() % 100 < 95) {
                al_draw_tinted_bitmap_region(
                    fire_bmp,
                    al_map_rgba(255, 0, 0, 255),
                    0, 0,
                    16, 16,
                    32 + (i * 16), 396,
                    0
                );
            }
        }

        for (i = 1; i < 4; i++) {
            if (rand() % 100 < 60) {
                al_draw_tinted_bitmap_region(
                    fire_bmp,
                    al_map_rgba(255, 0, 0, 255),
                    0, 0,
                    16, 16,
                    32 + (i * 16), 380,
                    0
                );
            }
        }

        for (i = 2; i < 3; i++) {
            if (rand() % 100 < 30) {
                al_draw_tinted_bitmap_region(
                    fire_bmp,
                    al_map_rgba(255, 0, 0, 255),
                    0, 0,
                    16, 16,
                    32 + (i * 16), 364,
                    0
                );
            }
        }

        al_rest(0.05);

        al_flip_display();

        al_draw_scaled_bitmap(tileset, 0, 0, 16, 16, 32, 364, 80, 48, 0);
    }
}

int main(int argc, char** argv)
{
    srand(time(NULL));

    ALLEGRO_DISPLAY *display = NULL;

    if(!al_init()) {
        fprintf(stderr, "failed to initialize allegro!\n");
        return -1;
    }

    if (!al_init_image_addon())
    {
        fprintf(stderr, "failed to initialize image addon!\n");
        return -1;
    }

    display = al_create_display(640, 480);
    if(!display) {
        fprintf(stderr, "failed to create display!\n");
        return -1;
    }

    al_install_mouse();
    al_install_keyboard();

    al_clear_to_color(al_map_rgb(0,0,0));

    ALLEGRO_BITMAP* tileset = al_load_bitmap("tileset.png");
    ALLEGRO_BITMAP* fire_bmp = al_create_sub_bitmap(tileset, 160, 32, 16, 16);

    al_draw_bitmap(tileset, 0, 0, 0);

    al_draw_bitmap_region(tileset, 16, 0, 16, 16, 300, 300, 0);
    al_draw_bitmap_region(tileset, 16, 0, 16, 16, 316, 316, 0);
    al_draw_bitmap_region(tileset, 32, 0, 16, 16, 316, 300, 0);
    al_draw_bitmap_region(tileset, 32, 0, 16, 16, 300, 316, 0);

    al_draw_tinted_bitmap_region(tileset, al_map_rgba(255, 0, 0, 255), 16, 0, 16, 16, 332, 332, 0);
    al_draw_tinted_bitmap_region(tileset, al_map_rgba(0, 255, 0, 255), 16, 0, 16, 16, 348, 332, 0);
    al_draw_tinted_bitmap_region(tileset, al_map_rgba(0, 0, 255, 255), 16, 0, 16, 16, 364, 332, 0);
    al_draw_tinted_bitmap_region(tileset, al_map_rgba(255, 0, 255, 255), 16, 0, 16, 16, 380, 332, 0);
    al_draw_tinted_bitmap_region(tileset, al_map_rgba(50, 0, 50, 50), 16, 0, 16, 16, 396, 332, 0);

    al_draw_tinted_bitmap_region(tileset, al_map_rgba(255, 255, 255, 255), 16, 0, 16, 16, 412, 332, 0);
    al_draw_tinted_bitmap_region(tileset, al_map_rgba(196, 0, 0, 196), 160, 32, 16, 16, 412, 332, 0);

    fireplace(tileset, fire_bmp);

    al_flip_display();

    al_rest(10.0);

    al_destroy_display(display);

    return 0;
}
