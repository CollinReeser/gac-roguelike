
#include "creature.h"
#include "game_context.h"

GameContext::GameContext():
    display(new Display()),
    dungeon(new Dungeon()),
    queue(al_create_event_queue())
{
    al_register_event_source(
        queue, al_get_keyboard_event_source()
    );
    al_register_event_source(
        queue, al_get_display_event_source(display->display)
    );
}

void process_movement(int x, int y, Dungeon* dungeon, Creature* creature)
{
    if (dungeon->entity_at_index(x, y).is_passable())
    {
        creature->set_x(x);
        creature->set_y(y);
    }
}

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
        switch (event.keyboard.keycode)
        {
        case ALLEGRO_KEY_LEFT:
            process_movement(cur_x - 1, cur_y, dungeon, controllable);
            break;
        case ALLEGRO_KEY_RIGHT:
            process_movement(cur_x + 1, cur_y, dungeon, controllable);
            break;
        case ALLEGRO_KEY_UP:
            process_movement(cur_x, cur_y - 1, dungeon, controllable);
            break;
        case ALLEGRO_KEY_DOWN:
            process_movement(cur_x, cur_y + 1, dungeon, controllable);
            break;
        case ALLEGRO_KEY_PAD_1:
            process_movement(cur_x - 1, cur_y + 1, dungeon, controllable);
            break;
        case ALLEGRO_KEY_PAD_2:
            process_movement(cur_x, cur_y + 1, dungeon, controllable);
            break;
        case ALLEGRO_KEY_PAD_3:
            process_movement(cur_x + 1, cur_y + 1, dungeon, controllable);
            break;
        case ALLEGRO_KEY_PAD_4:
            process_movement(cur_x - 1, cur_y, dungeon, controllable);
            break;
        case ALLEGRO_KEY_PAD_6:
            process_movement(cur_x + 1, cur_y, dungeon, controllable);
            break;
        case ALLEGRO_KEY_PAD_7:
            process_movement(cur_x - 1, cur_y - 1, dungeon, controllable);
            break;
        case ALLEGRO_KEY_PAD_8:
            process_movement(cur_x, cur_y - 1, dungeon, controllable);
            break;
        case ALLEGRO_KEY_PAD_9:
            process_movement(cur_x + 1, cur_y - 1, dungeon, controllable);
            break;
        }

        if (
            event.keyboard.keycode == ALLEGRO_KEY_ESCAPE ||
            event.keyboard.unichar == 'q'
        ) {
            return true;
        }
    }

    return false;
}

void GameContext::game_loop()
{
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
}

GameContext::~GameContext()
{
    delete display;
    delete dungeon;
}
