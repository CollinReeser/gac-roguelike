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

    Display* display;
    ALLEGRO_EVENT_QUEUE* queue;
    Dungeon* dungeon;
    std::vector<Creature*> creatures;

    std::vector<std::string> event_messages;
};

#endif
