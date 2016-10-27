#ifndef GAME_CONTEXT_H
#define GAME_CONTEXT_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#include <list>
#include <stdint.h>

#include "config.h"
#include "creature.h"
#include "display.h"
#include "dungeon.h"
#include "event.h"

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

    Creature* creature_at_index(uint64_t x, uint64_t y);
    void kill_creature_at_index(uint64_t x, uint64_t y);
    bool index_is_unoccupied(uint64_t x, uint64_t y);

    std::string damage_flavor(Creature* source, Creature* target);

    void player_died();

    Config* config;
    Display* display;
    Dungeon* dungeon;

    ALLEGRO_EVENT_QUEUE* queue;

    std::list<Creature*> creatures;
    std::list<Event*> events;

    uint64_t clock_time;
};

#endif
