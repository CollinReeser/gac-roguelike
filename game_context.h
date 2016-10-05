#ifndef GAME_CONTEXT_H
#define GAME_CONTEXT_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#include <stdint.h>
#include <vector>

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

    Creature* creature_at_index(int x, int y);

    Display* display;
    Dungeon* dungeon;

    ALLEGRO_EVENT_QUEUE* queue;

    std::vector<Creature*> creatures;

    uint64_t clock_time;

    std::vector<std::string> event_messages;
};

#endif
