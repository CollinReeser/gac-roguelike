#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "dungeon.h"
#include "entity.h"

Dungeon::Dungeon():
    width(DEFAULT_WIDTH),
    height(DEFAULT_HEIGHT)
{
    generate_dungeon();
}

void Dungeon::generate_dungeon()
{
    for (int64_t i = 0; i < height; i++)
    {
        dungeon.push_back(std::vector<Entity>());
        for (int64_t j = 0; j < width; j++)
        {
            dungeon[i].push_back(Entity('#', false, j, i));
        }
    }

    for (int64_t i = 0; i < 10; i++)
    {
        uint64_t startx;
        uint64_t starty;
        uint64_t endx;
        uint64_t endy;

        do {
            startx = 1 + rand() % (width - 2);
            starty = 1 + rand() % (height - 2);
            endx = 1 + rand() % (width - 2);
            endy = 1 + rand() % (height - 2);
        } while (startx >= endx || starty >= endy);

        if (endx > startx + 30)
        {
            endx = startx + 30;
        }
        if (endy > starty + 15)
        {
            endy = starty + 15;
        }

        for (int64_t j = starty; j < endy; j++)
        {
            for (int64_t k = startx; k < endx; k++)
            {
                dungeon[j][k] = Entity('.', true, k, j);
            }
        }
    }
}

uint64_t Dungeon::get_width() const
{
    return width;
}

uint64_t Dungeon::get_height() const
{
    return height;
}

const Entity& Dungeon::entity_at_index(uint64_t x, uint64_t y) const
{
    return dungeon[y][x];
}

void Dungeon::print_dungeon()
{
    for (int64_t i = 0; i < dungeon.size(); i++)
    {
        for (int64_t j = 0; j < dungeon[i].size(); j++)
        {
            printf("%c", dungeon[i][j].get_symbol());
        }
        printf("\n");
    }
}

Dungeon::~Dungeon()
{
}
