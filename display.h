#ifndef DISPLAY_H
#define DISPLAY_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#include <unordered_map>
#include <stdexcept>
#include <stdint.h>
#include <string>
#include <vector>

#include "dungeon.h"
#include "entity.h"

class Display
{
public:
    Display();
    ~Display();

    void draw_dungeon(
        int width, int height, int center_x, int center_y,
        const Dungeon* dungeon, const std::vector<const Entity*> entities
    );

    ALLEGRO_DISPLAY* display;

private:
    ALLEGRO_BITMAP* tileset;

    std::unordered_map<char, ALLEGRO_BITMAP*> tilemap;

    const std::string tileset_filename;

    const uint64_t DISP_WIDTH = 640;
    const uint64_t DISP_HEIGHT = 480;

    void init() throw (std::runtime_error);
    void init_tilemap();
};

#endif
