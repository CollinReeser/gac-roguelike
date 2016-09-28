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

using namespace std;

void game_loop()
{
    Display* display = new Display();
    Dungeon* dungeon = new Dungeon();
    Creature* player = new Creature(true, true, 50, '@');

    display->draw_dungeon(50, 30, 30, 20, dungeon, {player});

    al_rest(5.0);

    delete player;
    delete dungeon;
    delete display;
}

int main(int argc, char** argv)
{
    srand(time(NULL));

    game_loop();

    return 0;
}
