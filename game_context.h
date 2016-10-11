#ifndef GAME_CONTEXT_H
#define GAME_CONTEXT_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#include <stdint.h>
#include <vector>

#include "config.h"
#include "creature.h"
#include "display.h"
#include "dungeon.h"

class GameContext
{
public:
    GameContext();
    ~GameContext();

    void game_loop();

private:
    bool take_input(Creature* creature);
    Creature* get_player();

    void process_movement(int x, int y, Creature* creature);
    void conduct_melee_attack(Creature* source, Creature* target);
    void ai_turn(Creature* player, Creature* creature);
    bool creature_nearby(Creature* creat_a, Creature* creat_b, int thresh);
    void move_towards(Creature* creat_a, Creature* creat_b);
    void wander(Creature* creature);

    Creature* creature_at_index(int x, int y);
    void kill_creature_at_index(int x, int y);
    bool index_is_unoccupied(int x, int y);

    void player_died();

    Config* config;
    Display* display;
    Dungeon* dungeon;

    ALLEGRO_EVENT_QUEUE* queue;

    std::vector<Creature*> creatures;

    uint64_t clock_time;

    std::vector<std::string> event_messages;
};

#endif
