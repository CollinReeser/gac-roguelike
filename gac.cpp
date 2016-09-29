#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#include <iostream>
#include <stdexcept>
#include <stdlib.h>
#include <time.h>

#include "creature.h"
#include "display.h"
#include "dungeon.h"
#include "entity.h"
#include "game_context.h"

using namespace std;

int main(int argc, char** argv)
{
    srand(time(NULL));

    GameContext* game_context = new GameContext();
    game_context->game_loop();

    return 0;
}
