#include <allegro5/allegro.h>

#include <sstream>
#include <vector>

#include "config.h"
#include "creature.h"
#include "game_context.h"

GameContext::GameContext():
    config(new Config()),
    display(new Display()),
    dungeon(new Dungeon()),
    queue(al_create_event_queue()),
    clock_time(0)
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
            '@', "Glorzak", true, true, true, free_pos_x, free_pos_y, 50, 10
        )
    );

    config->dump();
}

Creature* GameContext::creature_at_index(int x, int y)
{
    for (auto it = creatures.cbegin(); it != creatures.cend(); it++)
    {
        if ((*it)->get_x() == x && (*it)->get_y() == y)
        {
            return *it;
        }
    }

    return NULL;
}

void GameContext::kill_creature_at_index(int x, int y) {
    for (auto it = creatures.begin(); it != creatures.end(); it++) {
        if ((*it)->get_x() == x && (*it)->get_y() == y) {
            creatures.erase(it);

            break;
        }
    }
}

void GameContext::conduct_melee_attack(Creature* source, Creature* target) {
    auto damage = source->get_strength();

    target->take_damage(damage);

    std::ostringstream stream;

    stream << "You hit the " << target->get_name() << " with your fists.";

    event_messages.push_back(stream.str());
    // Clear the sstream
    stream.str(std::string());

    stream << "The " << target->get_name();

    if (target->get_health() <= 0) {
        kill_creature_at_index(target->get_x(), target->get_y());

        stream << " dies.";
    }
    else if (target->get_health() >= target->get_max_health() * 0.8) {
        stream << " is lightly wounded.";
    }
    else if (target->get_health() >= target->get_max_health() * 0.5) {
        stream << " is moderately wounded.";
    }
    else if (target->get_health() >= target->get_max_health() * 0.3) {
        stream << " is heavily wounded.";
    }
    else if (target->get_health() >= target->get_max_health() * 0.1) {
        stream << " is gravely wounded.";
    }

    event_messages.push_back(stream.str());
}

void GameContext::process_movement(int x, int y, Creature* creature)
{
    if (auto target = creature_at_index(x, y))
    {
        // Swap positions with the friendly creature
        if (target->is_friendly())
        {
            target->set_x(creature->get_x());
            target->set_y(creature->get_y());
            creature->set_x(x);
            creature->set_y(y);
        }
        else
        {
            conduct_melee_attack(creature, target);
        }
    }
    else if (dungeon->entity_at_index(x, y).is_passable())
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
        switch (event.keyboard.keycode)
        {
        case ALLEGRO_KEY_LEFT:
            process_movement(cur_x - 1, cur_y, controllable);
            break;
        case ALLEGRO_KEY_RIGHT:
            process_movement(cur_x + 1, cur_y, controllable);
            break;
        case ALLEGRO_KEY_UP:
            process_movement(cur_x, cur_y - 1, controllable);
            break;
        case ALLEGRO_KEY_DOWN:
            process_movement(cur_x, cur_y + 1, controllable);
            break;
        case ALLEGRO_KEY_PAD_1:
            process_movement(cur_x - 1, cur_y + 1, controllable);
            break;
        case ALLEGRO_KEY_PAD_2:
            process_movement(cur_x, cur_y + 1, controllable);
            break;
        case ALLEGRO_KEY_PAD_3:
            process_movement(cur_x + 1, cur_y + 1, controllable);
            break;
        case ALLEGRO_KEY_PAD_4:
            process_movement(cur_x - 1, cur_y, controllable);
            break;
        case ALLEGRO_KEY_PAD_6:
            process_movement(cur_x + 1, cur_y, controllable);
            break;
        case ALLEGRO_KEY_PAD_7:
            process_movement(cur_x - 1, cur_y - 1, controllable);
            break;
        case ALLEGRO_KEY_PAD_8:
            process_movement(cur_x, cur_y - 1, controllable);
            break;
        case ALLEGRO_KEY_PAD_9:
            process_movement(cur_x + 1, cur_y - 1, controllable);
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
    Creature* player;
    if (!(player = get_player()))
    {
        return;
    }

    creatures.push_back(
        new Creature(
            'g', "gobber", false, false, false,
            player->get_x() + 2, player->get_y() - 2,
            100, 10
        )
    );

    while (1)
    {
        for (auto it = creatures.begin(); it != creatures.end(); it++)
        {
            if (!(*it)->is_turn(clock_time))
            {
                continue;
            }

            if (!(*it)->is_controllable())
            {
                continue;
            }

            display->draw_dungeon(
                (*it)->get_x(), (*it)->get_y(),
                dungeon, creatures.cbegin(), creatures.cend()
            );

            display->draw_event_messages(event_messages);

            display->draw_borders();

            display->update_screen();

            bool exit_request = take_input(*it);

            if (exit_request)
            {
                goto BREAK_GAME_LOOP;
            }
        }

        clock_time += 5;
    }

BREAK_GAME_LOOP:

    return;
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

    return NULL;
}

GameContext::~GameContext()
{
    delete display;
    delete dungeon;

    for (auto it = creatures.begin(); it != creatures.end(); it++)
    {
        delete *it;
    }

    al_destroy_event_queue(queue);
}
