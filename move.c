#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

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

    const int DISP_WIDTH = 640;
    const int DISP_HEIGHT = 480;

    display = al_create_display(DISP_WIDTH, DISP_HEIGHT);
    if(!display) {
        fprintf(stderr, "failed to create display!\n");
        return -1;
    }

    al_install_keyboard();

    al_clear_to_color(al_map_rgb(0,0,0));

    ALLEGRO_BITMAP* tileset = al_load_bitmap("tileset.png");

    al_draw_tinted_bitmap_region(
        tileset,
        al_map_rgba(255, 255, 255, 255),
        0, 64,
        16, 16,
        0, 0,
        0
    );

    al_flip_display();

    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));

    int x = 0;
    int y = 0;
    int x_max = DISP_WIDTH / 16 - 1;
    int y_max = DISP_HEIGHT / 16 - 1;
    int color_red = 255;
    int color_green = 255;
    int color_blue = 255;

    while (1)
    {
        ALLEGRO_EVENT event;
        al_wait_for_event(queue, &event);

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            break;
        }

        if (event.type == ALLEGRO_EVENT_KEY_CHAR)
        {
            if (event.keyboard.keycode == ALLEGRO_KEY_LEFT)
            {
                x = (x > 0) ? x - 1 : 0;
            }
            if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT)
            {
                x = (x < x_max) ? x + 1 : x_max;
            }
            if (event.keyboard.keycode == ALLEGRO_KEY_UP)
            {
                y = (y > 0) ? y - 1 : 0;
            }
            if (event.keyboard.keycode == ALLEGRO_KEY_DOWN)
            {
                y = (y < y_max) ? y + 1 : y_max;
            }

            if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
            {
                break;
            }
            if (event.keyboard.unichar == 'q')
            {
                break;
            }
            if (event.keyboard.unichar == 'r')
            {
                x = 0;
                y = 0;
            }
            if (event.keyboard.unichar == 't')
            {
                x = rand() % x_max;
                y = rand() % y_max;
            }
            if (event.keyboard.unichar == 'c')
            {
                color_red = rand() % 255;
                color_green = rand() % 255;
                color_blue = rand() % 255;
            }
        }

        if (al_is_event_queue_empty(queue))
        {
            al_clear_to_color(al_map_rgb_f(0, 0, 0));

            al_draw_tinted_bitmap_region(
                tileset,
                al_map_rgba(color_red, color_green, color_blue, 255),
                0, 64,
                16, 16,
                x * 16, y * 16,
                0
            );

            al_flip_display();
        }
    }

    al_destroy_display(display);

    return 0;
}
