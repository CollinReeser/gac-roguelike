#include <allegro5/allegro.h>

#include <sstream>
#include <vector>
#include <utility>

#include "animation.h"
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

    auto gobber = config->load_creature("gobber");
    gobber->set_position(free_pos_x + 2, free_pos_y - 2);

    creatures.push_back(gobber);
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
    else if (target->get_health() >= target->get_max_health() * 0.2) {
        stream << " is heavily wounded.";
    }
    else {
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
            target->set_position(creature->get_x(), creature->get_y());
            creature->set_position(x, y);
        }
        else
        {
            conduct_melee_attack(creature, target);
        }
    }
    else if (dungeon->entity_at_index(x, y).is_passable())
    {
        creature->set_position(x, y);
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

void GameContext::ai_turn(Creature* player, Creature* creature) {

    //check to see if player is nearby
    if (creature_nearby(creature, player, 5)) {
        if (creature_nearby(creature, player, 1)) {
            conduct_melee_attack(creature, player);
        }
        else {
            //move towards player
            move_towards(creature, player);
        }
    }
    else {
        wander(creature);
    }
}

//Check to see if Creature A is within thresh of Creature B
bool GameContext::creature_nearby(Creature* creat_a, Creature* creat_b, int thresh) {
    if (
        creat_a->get_x() + thresh >= creat_b->get_x() &&
        creat_a->get_x() - thresh <= creat_b->get_x() &&
        creat_a->get_y() + thresh >= creat_b->get_y() &&
        creat_a->get_y() - thresh <= creat_b->get_y()
    ) {
        //printf("NEAR\n");
        return true;
    }
    return false;
}

bool GameContext::index_is_unoccupied(int x, int y) {
    if (
        dungeon->entity_at_index(x, y).is_passable() &&
        creature_at_index(x, y) == NULL
    ) {
        return true;
    }
    return false;
}

//Check to see what diretion to go
void GameContext::move_towards(Creature* creat_a, Creature* creat_b) {
    if (creat_a->get_x() > creat_b->get_x()) {
        if (index_is_unoccupied(creat_a->get_x() - 1, creat_a->get_y())) {
            creat_a->set_position(creat_a->get_x() - 1, creat_a->get_y());
        }
    }
    else if (creat_a->get_x() < creat_b->get_x()) {
        if (index_is_unoccupied(creat_a->get_x() + 1, creat_a->get_y())) {
            creat_a->set_position(creat_a->get_x() + 1, creat_a->get_y());
        }
    }
    else if (creat_a->get_y() > creat_b->get_y()) {
        if (index_is_unoccupied(creat_a->get_x(), creat_a->get_y() - 1)) {
            creat_a->set_position(creat_a->get_x(), creat_a->get_y() - 1);
        }
    }
    else if (creat_a->get_y() < creat_b->get_y()) {
        if (index_is_unoccupied(creat_a->get_x(), creat_a->get_y() + 1)) {
            creat_a->set_position(creat_a->get_x(), creat_a->get_y() + 1);
        }
    }
}

/*
    have a Creature wander into a new space

    1 2 3
    4 X 5   Pick one of the numbered spots to move into
    6 7 8
*/
void GameContext::wander(Creature* creature) {

    std::vector<std::pair<int,int>> spaces;

    int x = -1;
    while (x < 2) {
        int y = -1;
        while (y < 2) {
            if (index_is_unoccupied(creature->get_x() + x, creature->get_y() + y)) {
                spaces.push_back(std::make_pair(creature->get_x() + x, creature->get_y() + y));
            }
            y++;
        }
        x++;
    }

    if (!spaces.empty()) {
        int r = rand() % spaces.size();
        creature->set_position(spaces[r].first, spaces[r].second);
    }
}

void GameContext::game_loop()
{
    while (1)
    {
        for (auto it = creatures.begin(); it != creatures.end(); it++)
        {
            Creature* player;
            if (!(player = get_player()))
            {
                goto PLAYER_DIED;
            }

            if (!(*it)->is_turn(clock_time))
            {
                continue;
            }

            if (!(*it)->is_controllable())
            {
                ai_turn(player, *it);
                continue;
            }

            std::vector<Animation*> animations;

            display->draw_dungeon(
                (*it)->get_x(), (*it)->get_y(),
                dungeon, creatures.cbegin(), creatures.cend(),
                animations
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

PLAYER_DIED:

    player_died();

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

void GameContext::player_died() {
    display->draw_game_over();
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
