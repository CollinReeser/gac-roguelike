#include <allegro5/allegro.h>

#include <sstream>
#include <vector>

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

    creatures.push_back(
        new Creature(
            '@', true, true, true, free_pos_x, free_pos_y, 50
        )
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

bool GameContext::take_input(Creature* controllable)
{
    ALLEGRO_EVENT event;

    do {
        al_wait_for_event(queue, &event);
    } while(
        event.type != ALLEGRO_EVENT_DISPLAY_CLOSE &&
        event.type != ALLEGRO_EVENT_KEY_CHAR
    );

    if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
    {
        return true;
    }

    int cur_x = controllable->get_x();
    int cur_y = controllable->get_y();

    if (event.type == ALLEGRO_EVENT_KEY_CHAR)
    {
        std::ostringstream stream;
        stream << "The key " << event.keyboard.keycode << " was pressed!";

        event_messages.push_back(stream.str());

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
    auto player = get_player();

    creatures.push_back(
        new Creature(
            'B', false, true, true, player->get_x() + 1, player->get_y() - 1, 30
        )
    );

    creatures.push_back(
        new Creature(
            'C', false, true, true, player->get_x() - 1, player->get_y() + 1, 30
        )
    );

    while (1)
    {
        for (auto it = creatures.begin(); it != creatures.end(); it++)
        {
            if ((*it)->is_controllable())
            {
                display->draw_dungeon(
                    (*it)->get_x(), (*it)->get_y(),
                    dungeon, creatures.cbegin(), creatures.cend()
                );

                display->draw_event_messages(event_messages);

                display->update_screen();

                bool exit_request = take_input(*it);

                if (exit_request)
                {
                    goto BREAK_GAME_LOOP;
                }
            }
        }
    }

BREAK_GAME_LOOP:

    delete player;
}

Creature* GameContext::get_player()
{
    for (auto it = creatures.begin(); it != creatures.end(); it++)
    {
        if ((*it)->is_player())
        {
            return *it;
        }
    }
}

GameContext::~GameContext()
{
    delete display;
    delete dungeon;
}
