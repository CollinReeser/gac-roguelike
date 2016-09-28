#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#include <iostream>
#include <stdexcept>
#include <stdlib.h>
#include <time.h>

#include "creature.h"
#include "display.h"
#include "dungeon.h"
#include "entity.h"

using namespace std;

bool take_input(
    ALLEGRO_EVENT_QUEUE* queue, Dungeon* dungeon, Creature* controllable
) {
    ALLEGRO_EVENT event;
    al_wait_for_event(queue, &event);

    if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
    {
        return true;
    }

    int cur_x = controllable->get_x();
    int cur_y = controllable->get_y();

    if (event.type == ALLEGRO_EVENT_KEY_CHAR)
    {
        if (event.keyboard.keycode == ALLEGRO_KEY_LEFT)
        {
            if (dungeon->entity_at_index(cur_x - 1, cur_y).is_passable())
            {
                controllable->set_x(cur_x - 1);
            }
        }
        if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT)
        {
            if (dungeon->entity_at_index(cur_x + 1, cur_y).is_passable())
            {
                controllable->set_x(cur_x + 1);
            }
        }
        if (event.keyboard.keycode == ALLEGRO_KEY_UP)
        {
            if (dungeon->entity_at_index(cur_x, cur_y - 1).is_passable())
            {
                controllable->set_y(cur_y - 1);
            }
        }
        if (event.keyboard.keycode == ALLEGRO_KEY_DOWN)
        {
            if (dungeon->entity_at_index(cur_x, cur_y + 1).is_passable())
            {
                controllable->set_y(cur_y + 1);
            }
        }

        if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
        {
            return true;
        }
        if (event.keyboard.unichar == 'q')
        {
            return true;
        }
    }

    return false;
}

void game_loop()
{
    Display* display = new Display();

    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    al_register_event_source(
        queue, al_get_keyboard_event_source()
    );
    al_register_event_source(
        queue, al_get_display_event_source(display->display)
    );

    Dungeon* dungeon = new Dungeon();

    int free_pos_x;
    int free_pos_y;
    while (1)
    {
        int rand_x = rand() % dungeon->get_width();
        int rand_y = rand() % dungeon->get_height();

        if (dungeon->entity_at_index(rand_x, rand_y).is_passable())
        {
            free_pos_x = rand_x;
            free_pos_y = rand_y;

            break;
        }
    }

    Creature* player = new Creature(
        '@', true, true, free_pos_x, free_pos_y, 50
    );

    while (1)
    {
        display->draw_dungeon(
            50, 20,
            player->get_x(), player->get_y(),
            dungeon, {player}
        );

        bool exit_request = take_input(queue, dungeon, player);

        if (exit_request)
        {
            break;
        }
    }

    delete player;
    delete dungeon;
    delete display;
}

int main(int argc, char** argv)
{
    srand(time(NULL));

    game_loop();

    return 0;
}
