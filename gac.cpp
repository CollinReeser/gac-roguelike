#include <stdlib.h>
#include <time.h>

#include "game_context.h"

using namespace std;

int main(int argc, char** argv)
{
    srand(time(NULL));

    GameContext* game_context = new GameContext();
    game_context->game_loop();

    return 0;
}
