#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

int main(int argc, char **argv){

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

    al_draw_bitmap(tileset, 0, 0, 0);

    al_draw_bitmap_region(tileset, 16, 0, 16, 16, 300, 300, 0);
    al_draw_bitmap_region(tileset, 16, 0, 16, 16, 316, 316, 0);
    al_draw_bitmap_region(tileset, 32, 0, 16, 16, 316, 300, 0);
    al_draw_bitmap_region(tileset, 32, 0, 16, 16, 300, 316, 0);

    al_flip_display();

    al_rest(10.0);

    al_destroy_display(display);

    return 0;
}
