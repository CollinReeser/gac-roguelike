#ifndef GAME_CONTEXT_H
#define GAME_CONTEXT_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#include <stdint.h>
#include <vector>
#include <list>
#include <utility>
#include <cmath>

#include "config.h"
#include "creature.h"
#include "display.h"
#include "dungeon.h"

struct path_node;
struct path_node {
    uint64_t x_coord;
    uint64_t y_coord;
    uint64_t score_start;
    uint64_t score_end;
    struct path_node* prev_point;
};

class GameContext
{
public:
    GameContext();
    ~GameContext();

    void game_loop();

private:
    bool take_input(Creature* creature);
    Creature* get_player();

    void process_movement(uint64_t x, uint64_t y, Creature* creature);
    void conduct_melee_attack(Creature* source, Creature* target);
    void conduct_throw_attack(Creature* source, Creature* target);
    void ai_turn(Creature* player, Creature* creature);
    bool creature_nearby(Creature* creat_a, Creature* creat_b, uint64_t thresh);
    void move_towards(Creature* creat_a, Creature* creat_b);
    void wander(Creature* creature);

    path_node* find_path(uint64_t x_start, uint64_t y_start, uint64_t x_finish, uint64_t y_finish);
    bool candidate_is_end_node(path_node* node, path_node* finish_node);
    void score_node(path_node* node, path_node* start, path_node* goal, uint64_t D);
    std::list<path_node*> get_adjacent_nodes(path_node* node, path_node* start, path_node* finish, bool** explored);
    bool valid_index(uint64_t x, uint64_t y);

    Creature* creature_at_index(uint64_t x, uint64_t y);
    void kill_creature_at_index(uint64_t x, uint64_t y);
    bool index_is_unoccupied(uint64_t x, uint64_t y);

    std::string damage_flavor(Creature* source, Creature* target);

    void player_died();

    Config* config;
    Display* display;
    Dungeon* dungeon;

    ALLEGRO_EVENT_QUEUE* queue;

    std::vector<Creature*> creatures;

    uint64_t clock_time;
};

#endif
