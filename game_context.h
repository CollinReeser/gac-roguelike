#ifndef GAME_CONTEXT_H
#define GAME_CONTEXT_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#include <stdint.h>
#include <vector>

#include "display.h"

class GameContext
{
public:
    GameContext();
    ~GameContext();

    void game_loop();

private:
    Display* display;
    ALLEGRO_EVENT_QUEUE* queue;
    Dungeon* dungeon;
};

#endif
