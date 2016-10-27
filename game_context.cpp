#include <allegro5/allegro.h>

#include <sstream>
#include <unordered_map>
#include <utility>
#include <vector>

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

    auto player = new Creature(
        '@', "Glorzak", al_map_rgb(255, 255, 255),
        true, true, true,
        50, 100,
        1, 1,
        true,
        {},
        {}
    );

    player->set_position(free_pos_x, free_pos_y);

    creatures.push_back(player);
}

Creature* GameContext::creature_at_index(uint64_t x, uint64_t y)
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

// Do not call this on the current creature iterator of the main game loop.
// std::list::erase will keep all iterators valid _except_ for the iterator
// which is being erased (which makes sense; it's just removing a link in a
// linked list)
void GameContext::kill_creature_at_index(uint64_t x, uint64_t y) {
    for (auto it = creatures.begin(); it != creatures.end(); it++) {
        if ((*it)->get_x() == x && (*it)->get_y() == y) {
            delete *it;
            creatures.erase(it);
            break;
        }
    }
}

void GameContext::conduct_melee_attack(Creature* source, Creature* target) {
    auto damage = source->get_strength();

    target->take_damage(damage);

    std::ostringstream stream;

    if (source->is_player()) {
        stream << "You hit the " << target->get_name() << " with your fists. ";
    }
    else {
        stream << "The " << source->get_name() << " ";
        stream << source->get_attack_flavor() << " ";
        if (target->is_player()) {
            stream << "you ";
        }
        else {
            stream << "the " << target->get_name() << " ";
        }
        stream << "with its " << source->get_weapon_name() << ". ";
    }

    stream << damage_flavor(source, target);
    Display::add_event_message(stream.str());

    if (target->get_health() <= 0) {
        kill_creature_at_index(target->get_x(), target->get_y());
    }

    for (auto event : source->generate_on_melee_attack_events(target)) {
        events.push_back(event);
    }
}

void GameContext::conduct_throw_attack(Creature* source, Creature* target) {
    auto damage = source->get_dexterity();

    auto player = get_player();

    auto animation = new Animation(AnimationType::PROJECTILE, source, target);
    display->draw_animation(
        player->get_x(), player->get_y(),
        dungeon, creatures.cbegin(), creatures.cend(),
        animation
    );
    delete animation;

    target->take_damage(damage);

    auto thrown_item = source->consume_throwable();

    std::ostringstream stream;

    if (source->is_player()) {
        stream << "You throw a " << thrown_item->get_name()
               << " at the " << target->get_name() << ". ";
    }
    else {
        stream << "The " << source->get_name()
               << " throws a " << thrown_item->get_name() << " at ";
        if (target->is_player()) {
            stream << "you";
        }
        else {
            stream << "the " << target->get_name();
        }
        stream << ". ";
    }

    delete thrown_item;

    stream << damage_flavor(source, target);
    Display::add_event_message(stream.str());

    if (target->get_health() <= 0) {
        kill_creature_at_index(target->get_x(), target->get_y());
    }
}

std::string GameContext::damage_flavor(Creature* source, Creature* target) {
    std::ostringstream stream;

    if (!target->is_player()) {
        stream << "The " << target->get_name();

        if (target->get_health() >= target->get_max_health() * 0.8) {
            stream << " is lightly wounded.";
        }
        else if (target->get_health() >= target->get_max_health() * 0.5) {
            stream << " is moderately wounded.";
        }
        else if (target->get_health() >= target->get_max_health() * 0.2) {
            stream << " is heavily wounded.";
        }
        else if (target->get_health() > 0) {
            stream << " is gravely wounded.";
        }
        else {
            stream << " dies.";
        }
    }

    return stream.str();
}

void GameContext::process_movement(uint64_t x, uint64_t y, Creature* creature) {
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

    bool event_is_populated = false;

    if (!al_is_event_queue_empty(queue)) {
        al_get_next_event(queue, &event);

        if (
            event.type == ALLEGRO_EVENT_DISPLAY_CLOSE ||
            event.type == ALLEGRO_EVENT_KEY_CHAR
        ) {
            al_flush_event_queue(queue);
            event_is_populated = true;
        }
    }

    while (
        !event_is_populated ||
        (
            event.type != ALLEGRO_EVENT_DISPLAY_CLOSE &&
            event.type != ALLEGRO_EVENT_KEY_CHAR
        )
    ) {
        event_is_populated = true;

        al_wait_for_event(queue, &event);
    }

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
        if (creature->get_can_throw() && creature->has_throwable()) {
            conduct_throw_attack(creature, player);
        }
        else if (creature_nearby(creature, player, 1)) {
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
bool GameContext::creature_nearby(
    Creature* creat_a, Creature* creat_b, uint64_t thresh
) {
    if (
        creat_a->get_x() + thresh >= creat_b->get_x() &&
        creat_a->get_x() - thresh <= creat_b->get_x() &&
        creat_a->get_y() + thresh >= creat_b->get_y() &&
        creat_a->get_y() - thresh <= creat_b->get_y()
    ) {
        return true;
    }
    return false;
}

bool GameContext::index_is_unoccupied(uint64_t x, uint64_t y) {

    if (x >= dungeon->get_width() || y >= dungeon->get_height()) {
        return false;
    }

    if (
        dungeon->entity_at_index(x, y).is_passable() &&
        creature_at_index(x, y) == NULL
    ) {
        return true;
    }
    return false;
}

//Check to see what direction to go
void GameContext::move_towards(Creature* creat_a, Creature* creat_b) {
    path_node* dest = find_path(creat_a->get_x(), creat_a->get_y(), creat_b->get_x(), creat_b->get_y());

    if (dest->x_coord == creat_a->get_x() && dest->y_coord == creat_a->get_y()) {
        wander(creat_a);
    }
    else {
        creat_a->set_position(dest->x_coord, dest->y_coord);
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

    for (int x = -1; x < 2; x++) {
        for (int y = -1; y < 2; y++) {
            if (index_is_unoccupied(creature->get_x() + x, creature->get_y() + y)) {
                spaces.push_back(std::make_pair(creature->get_x() + x, creature->get_y() + y));
            }
        }
    }

    if (!spaces.empty()) {
        int r = rand() % spaces.size();
        creature->set_position(spaces[r].first, spaces[r].second);
    }
}

path_node* GameContext::find_path(uint64_t x_start, uint64_t y_start, uint64_t x_finish, uint64_t y_finish) {

    path_node* start_node = new path_node({x_start, y_start, 0, 0, NULL});
    path_node* finish_node = new path_node({x_finish, y_finish, 0, 0, NULL});

    bool** explored = new bool*[dungeon->get_height()];

    for (int i = 0; i < dungeon->get_height(); i++) {
        explored[i] = new bool[dungeon->get_width()]();
    }

    std::list<path_node*> finished;

    std::list<path_node*> candidates;

    finished.push_back(start_node);

    candidates.push_back(start_node);

    path_node* candidate;

    while (1) {

        if (candidates.size() == 0) {
            // ran out of candidates (no path)
            return start_node;
        }

        candidate = candidates.front();
        candidates.pop_front();

        if (candidate_is_end_node(candidate, finish_node)) {
            break;
        }

        finished.push_back(candidate);

        std::list<path_node*> adj = get_adjacent_nodes(candidate, start_node, finish_node, explored);

        if (adj.size() > 0) {
            for (auto it_adj = adj.begin(); it_adj != adj.end(); it_adj++){
                if (candidates.empty()) {
                    candidates.push_back((*it_adj));
                }
                else {
                    bool inserted = false;
                    for (auto it = candidates.begin(); it != candidates.end(); it++) {
                        if ((*it_adj)->score_start + (*it_adj)->score_end < (*it)->score_start + (*it)->score_end){
                            candidates.insert(it, (*it_adj));
                            inserted = true;
                            break;
                        }
                    }
                    if (!inserted) {
                        candidates.push_back(*it_adj);
                    }
                }
            }
        }
    }

    // found end node, loop through previous node
    // NULL<-S<-a<-b<-E
    // NULL<-S<-E
    while (candidate->prev_point->prev_point != NULL) {
        candidate = candidate->prev_point;
    }

    //found next node. return it.
    return candidate;

}

bool GameContext::candidate_is_end_node(path_node* node, path_node* finish_node) {
    return (node->x_coord == finish_node->x_coord && node->y_coord == finish_node->y_coord);
}

std::list<path_node*> GameContext::get_adjacent_nodes(path_node* node, path_node* start, path_node* finish, bool** explored) {
    std::list<path_node*> adj_nodes;

    for (int x = -1; x < 2; x++) {
        for (int y = -1; y < 2; y++) {
            if (x == 0 && y == 0) {
                continue;
            }

            uint64_t new_x = node->x_coord + x;
            uint64_t new_y = node->y_coord + y;

            if (
                (
                    new_x == finish->x_coord && new_y == finish->y_coord
                ) ||
                (
                    index_is_unoccupied(new_x, new_y) &&
                    valid_index(new_x, new_y) &&
                    !explored[new_y][new_x]
                )
            ) {

                //create new node using new coords and set previous node to "start" node
                path_node* new_node = new path_node({new_x, new_y, 0, 0, node});

                new_node->score_start = node->score_start + 1;
                new_node->score_end = std::max(
                    std::abs((int)new_node->x_coord - (int)finish->x_coord),
                    std::abs((int)new_node->y_coord - (int)finish->y_coord)
                );

                adj_nodes.push_back(new_node);
            }

            // whether we added it to adj_nodes or not, we explored the node.
            if (valid_index(new_x, new_y)) {
                explored[new_y][new_x] = true;
            }
        }
    }

    return adj_nodes;
}

bool GameContext::valid_index(uint64_t x, uint64_t y) {
    if (x >= dungeon->get_width() || y >= dungeon->get_height()) {
        return false;
    }
    else {
        return true;
    }
}

void GameContext::game_loop()
{
    while (1)
    {
        if (clock_time == 5) {

            while (1){

                uint64_t x = rand() % dungeon->get_width();
                uint64_t y = rand() % dungeon->get_height();

                if (index_is_unoccupied(x, y)) {
                    auto creature = config->load_random_creature();
                    creature->set_position(x, y);

                    creatures.push_back(creature);
                    break;
                }
            }
        }

        {
            auto it = events.begin();
            while (it != events.end()) {
                if ((*it)->is_turn(clock_time)) {
                    if ((*it)->apply_event()) {
                        delete *it;
                        it = events.erase(it);

                        continue;
                    }
                }

                it++;
            }
        }

        {
            for (auto it = creatures.begin(); it != creatures.end(); it++)
            {
                bool creature_died = false;
                // Check if the creature died from an event
                if ((*it)->get_health() <= 0) {
                    delete *it;
                    // "Kill" the creature
                    it = creatures.erase(it);

                    // Set up iterator to then be incremented by the for loop
                    // back to the position we actually want
                    if (it != creatures.begin()) {
                        it--;
                    }

                    creature_died = true;
                }

                Creature* player;
                if (!(player = get_player())) {
                    goto PLAYER_DIED;
                }

                if (creature_died) {
                    continue;
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

                display->draw_basic_screen(
                    (*it)->get_x(), (*it)->get_y(),
                    dungeon, creatures.cbegin(), creatures.cend()
                );

                bool exit_request = take_input(*it);

                if (exit_request)
                {
                    goto BREAK_GAME_LOOP;
                }
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
    delete config;
    delete display;
    delete dungeon;

    for (auto it = creatures.begin(); it != creatures.end(); it++)
    {
        delete *it;
    }

    al_destroy_event_queue(queue);
}
